#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "visualizer.h"
#include "../objdetect/hasher.h"
#include "../processing/processing.h"
#include "../core/classifier_criteria.h"
#include "../core/template.h"
#include "../glcore/frame_buffer.h"
#include "../glcore/shader.h"
#include "glutils.h"
#include "../objdetect/fine_pose.h"

namespace tless {
    Visualizer::Visualizer(cv::Ptr<ClassifierCriteria> criteria, const std::string &templatesPath)
            : criteria(criteria), templatesPath(templatesPath) {
        // Initialize settings
        settings[SETTINGS_GRID] = true;
        settings[SETTINGS_TITLE] = true;
        settings[SETTINGS_INFO] = true;
        settings[SETTINGS_FEATURE_POINT_STYLE] = true;
        settings[SETTINGS_FEATURE_POINT] = true;
        settings[SETTINGS_TPL_OVERLAY] = true;
    }

    cv::Mat Visualizer::loadTemplateSrc(const Template &tpl, int flags) {
        std::ostringstream oss;
        oss << templatesPath;
        oss << std::setw(2) << std::setfill('0') << tpl.objId;

        if (flags == CV_LOAD_IMAGE_UNCHANGED) {
            oss << "/depth/" << tpl.fileName << ".png";
        } else {
            oss << "/rgb/" << tpl.fileName << ".png";
        }

        return cv::imread(oss.str(), flags);
    }

    void Visualizer::label(cv::Mat &dst, const std::string &label, const cv::Point &origin, double scale,
                           int padding, int thickness, cv::Scalar fColor, cv::Scalar bColor, int fontFace) {
        cv::Size text = cv::getTextSize(label, fontFace, scale, thickness, nullptr);
        rectangle(dst, origin + cv::Point(-padding - 1, padding + 2),
                  origin + cv::Point(text.width + padding, -text.height - padding - 2), bColor, CV_FILLED);
        putText(dst, label, origin, fontFace, scale, fColor, thickness, CV_AA);
    }

#ifdef VIZ_HASHING
    void Visualizer::windowCandidates(const cv::Mat &src, cv::Mat &dst, Window &window) {
        std::ostringstream oss;
        dst = src.clone();

        // Create template mosaic of found candidates
        if (!window.candidates.empty()) {
            // Define grid, offsets and initialize tpl mosaic matrix
            const int offset = 8, topOffset = 25;
            int x, y, width = window.candidates[0]->objBB.width;
            int sizeX = width + 2 * offset, sizeY = width + offset + topOffset;
            auto gridSize = static_cast<int>(std::ceil(std::sqrt(window.candidates.size())));
            cv::Mat tplMosaic = cv::Mat::zeros(gridSize * sizeY, gridSize * sizeX, CV_8UC3);

            for (int i = 0; i < window.candidates.size(); ++i) {
                Template *candidate = window.candidates[i];

                // Calculate x, y and rect inside defined mosaic grid
                x = (i % gridSize);
                y = (i / gridSize);
                cv::Rect rect(x * sizeX + offset, y * sizeY + offset, width, width);

                // Load template src image
                cv::Mat tplSrc = loadTemplateSrc(*candidate);
                tplSrc.copyTo(tplMosaic(rect));

                // Draw triplets
                for (auto &triplet : window.triplets[i]) {
                    cv::line(tplMosaic, rect.tl() + triplet.c, rect.tl() + triplet.p1, cv::Scalar(0, 180, 0), 1, CV_AA);
                    cv::line(tplMosaic, rect.tl() + triplet.c, rect.tl() + triplet.p2, cv::Scalar(0, 180, 0), 1, CV_AA);
                    cv::circle(tplMosaic, rect.tl() + triplet.c, 2, cv::Scalar(0, 255, 0), -1, CV_AA);
                    cv::circle(tplMosaic, rect.tl() + triplet.p1, 2, cv::Scalar(255, 0, 0), -1, CV_AA);
                    cv::circle(tplMosaic, rect.tl() + triplet.p2, 2, cv::Scalar(0, 0, 255), -1, CV_AA);
                }

                // Annotate templates in mosaic
                cv::rectangle(tplMosaic, rect, cv::Scalar(200, 200, 200), 1);
                oss << "Votes: " << window.votes[i];
                label(tplMosaic, oss.str(), cv::Point(rect.x, rect.y + rect.height + 15));
                oss.str("");
            }

            cv::imshow("Candidate templates", tplMosaic);
        }

        // Annotate scene
        cv::rectangle(dst, window.rect(), cv::Scalar(0, 255, 0), 1, CV_AA);

        // Set labels
        if (settings[SETTINGS_INFO]) {
            oss << "candidates: " << window.candidates.size();
            label(dst, oss.str(), window.tl() + cv::Point(5, 16));
            oss.str("");
            oss << "edgels: " << window.edgels;
            label(dst, oss.str(), window.tl() + cv::Point(5, 34));
        }
    }

    void Visualizer::windowsCandidates(const ScenePyramid &scene, std::vector<Window> &windows, int wait,
                                       const char *title) {
        const auto winSize = static_cast<const int>(windows.size());
        std::ostringstream oss;
        cv::Mat result;

        for (int i = 0; i < winSize; ++i) {
            result = scene.srcRGB.clone();

            // Draw all other windows in gray
            if (settings[SETTINGS_GRID]) {
                for (auto &win : windows) {
                    cv::rectangle(result, win.rect(), cv::Scalar(90, 90, 90), 1);
                }
            }

            // Vizualize window candidates
            windowCandidates(result, result, windows[i]);

            // Title
            if (settings[SETTINGS_TITLE]) {
                cv::Point textTl(-4, 12);
                oss.str("");
                oss << " Locations: " << winSize;
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                textTl.y += 17;
            }

            // Show results
            cv::imshow(title == nullptr ? "Window candidates" : title, result);

            // Get key pressed
            int key = cv::waitKey(wait);

            // Navigation using arrow keys and spacebar
            if (key == KEY_RIGHT) {
                i = (i + 1 < winSize) ? i : winSize - 2;
            } else if (key == KEY_UP) {
                i = (i + 10 < winSize) ? i + 9 : winSize - 2;
            } else if (key == KEY_ENTER) {
                i = (i + 100 < winSize) ? i + 99 : winSize - 2;
            } else if (key == KEY_LEFT) {
                i = (i - 1 > 0) ? i - 2 : -1;
            } else if (key == KEY_DOWN) {
                i = (i - 10 > 0) ? i - 11 : -1;
            } else if (key == KEY_SPACEBAR) {
                i = (i - 100 > 0) ? i - 99 : -1;
            } else if (key == KEY_G) {
                settings[SETTINGS_GRID] = !settings[SETTINGS_GRID];
                i = i - 1;
            } else if (key == KEY_T) {
                settings[SETTINGS_TITLE] = !settings[SETTINGS_TITLE];
                i = i - 1;
            } else if (key == KEY_I) {
                settings[SETTINGS_INFO] = !settings[SETTINGS_INFO];
                i = i - 1;
            } else if (key == KEY_S) {
                break;
            }
        }
    }
#endif

#ifdef VIZ_OBJECTNESS
    void Visualizer::objectness(const ScenePyramid &scene, std::vector<Window> &windows, int wait, const char *title) {
        const auto winSize = static_cast<const int>(windows.size());
        auto minMag = static_cast<int>(criteria->objectnessDiameterThreshold * criteria->info.smallestDiameter * criteria->info.depthScaleFactor);
        cv::Mat result, depth, edgels, resultDepth, resultEdgels;
        std::ostringstream oss;

        // Convert depth
        scene.srcDepth.convertTo(depth, CV_8UC1, 255.0f / 65535.0f);
        cv::cvtColor(depth, depth, CV_GRAY2BGR);

        // Edgels computation
        depthEdgels(scene.srcDepth, edgels, criteria->info.minDepth, criteria->info.maxDepth, static_cast<int>(minMag / scene.scale));
        cv::normalize(edgels, edgels, 0, 255, CV_MINMAX);
        cv::cvtColor(edgels, edgels, CV_GRAY2BGR);

        for (int i = 0; i < windows.size(); ++i) {
            result = scene.srcRGB.clone();
            resultEdgels = edgels.clone();
            resultDepth = depth.clone();

            // Draw all other windows in gray
            if (settings[SETTINGS_GRID]) {
                for (auto &win : windows) {
                    cv::rectangle(result, win.rect(), cv::Scalar(90, 90, 90), 1);
                    cv::rectangle(resultEdgels, win.rect(), cv::Scalar(46, 70, 48), 1);
                    cv::rectangle(resultDepth, win.rect(), cv::Scalar(46, 70, 48), 1);
                }
            }

            // Annotate scene
            cv::rectangle(result, windows[i].rect(), cv::Scalar(0, 255, 0), 1, CV_AA);
            cv::rectangle(resultDepth, windows[i].rect(), cv::Scalar(0, 255, 0), 1, CV_AA);
            cv::rectangle(resultEdgels, windows[i].rect(), cv::Scalar(0, 255, 0), 1, CV_AA);

            // Set labels
            if (settings[SETTINGS_INFO]) {
                oss.str("");
                oss << "edgels: " << windows[i].edgels;
                label(result, oss.str(), windows[i].tl() + cv::Point(5, 16));
                label(resultDepth, oss.str(), windows[i].tl() + cv::Point(5, 16));
                label(resultEdgels, oss.str(), windows[i].tl() + cv::Point(5, 16));
            }

            // Locations title
            if (settings[SETTINGS_TITLE]) {
                cv::Point textTl(-4, 12);
                oss.str("");
                oss << " Locations: " << windows.size();
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                label(resultDepth, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                label(resultEdgels, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                textTl.y += 17;

                oss.str("");
                oss << " Min edgels: " << (criteria->info.minEdgels * criteria->objectnessFactor);
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                label(resultDepth, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                label(resultEdgels, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                textTl.y += 17;
            }

            // Show results
            cv::imshow(title == nullptr ? "Objectness locations - rgb" : title, result);
            cv::imshow("Objectness locations - depth", resultDepth);
            cv::imshow("Objectness locations - edgels", resultEdgels);

            // Get key pressed
            int key = cv::waitKey(wait);

            // Navigation using arrow keys and spacebar
            if (key == KEY_RIGHT) {
                i = (i + 1 < winSize) ? i : winSize - 2;
            } else if (key == KEY_UP) {
                i = (i + 10 < winSize) ? i + 9 : winSize - 2;
            } else if (key == KEY_ENTER) {
                i = (i + 100 < winSize) ? i + 99 : winSize - 2;
            } else if (key == KEY_LEFT) {
                i = (i - 1 > 0) ? i - 2 : -1;
            } else if (key == KEY_DOWN) {
                i = (i - 10 > 0) ? i - 11 : -1;
            } else if (key == KEY_SPACEBAR) {
                i = (i - 100 > 0) ? i - 99 : -1;
            } else if (key == KEY_G) {
                settings[SETTINGS_GRID] = !settings[SETTINGS_GRID];
                i = i - 1;
            } else if (key == KEY_T) {
                settings[SETTINGS_TITLE] = !settings[SETTINGS_TITLE];
                i = i - 1;
            } else if (key == KEY_I) {
                settings[SETTINGS_INFO] = !settings[SETTINGS_INFO];
                i = i - 1;
            } else if (key == KEY_S) {
                break;
            }
        }
    }
#endif

#ifdef VIZ_TPL_FEATURES
    void Visualizer::tplFeaturePoints(const Template &t, int wait, const char *title) {
        // Dynamically load template
        cv::Mat tplRGB = loadTemplateSrc(t, CV_LOAD_IMAGE_COLOR);
        cv::Mat tplDepth = loadTemplateSrc(t, CV_LOAD_IMAGE_UNCHANGED);
        tplDepth.convertTo(tplDepth, CV_8UC1, 255.0f / 65535.0f);
        cv::cvtColor(tplDepth, tplDepth, CV_GRAY2BGR);

        // Compute edges
        cv::Mat gray, edges;
        cv::cvtColor(tplRGB, gray, CV_BGR2GRAY);
        filterEdges(gray, edges);
        cv::cvtColor(edges, edges, CV_GRAY2BGR);

        // ROIs
        const int offset = 10;
        cv::Rect rgbROI(offset, offset, tplRGB.cols, tplRGB.rows);
        cv::Rect depthROI(rgbROI.br().x + offset, offset, tplDepth.cols, tplDepth.rows);
        cv::Rect edgesROI(depthROI.br().x + offset, offset, tplDepth.cols, tplDepth.rows);

        // Result size
        cv::Size resultSize(rgbROI.width * 3 + offset * 4, rgbROI.height + offset * 2 + 130);
        cv::Mat result = cv::Mat::zeros(resultSize, CV_8UC3);

        // Copy to result roi
        tplRGB.copyTo(result(rgbROI));
        tplDepth.copyTo(result(depthROI));
        edges.copyTo(result(edgesROI));

        // Draw bounding box
        cv::rectangle(result, rgbROI.tl(), rgbROI.br(), cv::Scalar(90, 90, 90), 1);
        cv::rectangle(result, depthROI.tl(), depthROI.br(), cv::Scalar(90, 90, 90), 1);
        cv::rectangle(result, edgesROI.tl(), edgesROI.br(), cv::Scalar(90, 90, 90), 1);

        // Draw edge points
        if (!t.edgePoints.empty()) {
            for (auto &point : t.edgePoints) {
                cv::circle(result, point + rgbROI.tl(), 1, cv::Scalar(0, 0, 255), -1, CV_AA);
                cv::circle(result, point + edgesROI.tl(), 1, cv::Scalar(0, 0, 255), -1, CV_AA);
            }
        }

        // Draw stable points
        if (!t.stablePoints.empty()) {
            for (auto &point : t.stablePoints) {
                cv::circle(result, point + rgbROI.tl(), 1, cv::Scalar(255, 0, 0), -1, CV_AA);
                cv::circle(result, point + depthROI.tl(), 1, cv::Scalar(255, 0, 0), -1, CV_AA);
            }
        }

        // Put text data to template image
        std::ostringstream oss;
        cv::Point textTl(offset, rgbROI.height + offset + 4);

        oss.str("");
        oss << "Template: " << t.fileName << " (" << (t.objId) << ")";
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "mode: " << t.camera.mode;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "elev: " << t.camera.elev;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "diameter: " << std::fixed << std::setprecision(2) << t.diameter;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "rszRatio: " << std::fixed << std::setprecision(2) << t.resizeRatio;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "objArea: " << t.objArea;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "avgDepth: " << t.features.avgDepth;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        cv::imshow(title == nullptr ? "Template features" : title, result);
        cv::waitKey(wait);
    }
#endif

#if defined(VIZ_MATCHING) || defined(VIZ_NMS)
    void Visualizer::tplMatch(Template &t, const std::vector<std::pair<cv::Point, int>> &features,
                              int highlight, int patchOffset, int wait, const char *title) {
        const int offset = 15;
        cv::Mat result = cv::Mat::zeros(5 * offset + 4 * t.objBB.height, offset * 3 + t.objBB.width * 2, CV_8UC3);
        cv::Scalar cGreen(0, 255, 0), cRed(0, 0, 255), cBlue(255, 0, 0), cWhite(255, 255, 255), cGray(90, 90, 90);
        cv::Point offsetStart(-patchOffset, -patchOffset), offsetEnd(patchOffset, patchOffset);

        // Dynamically load template images
        cv::Mat gray;
        cv::Mat rgb = loadTemplateSrc(t, CV_LOAD_IMAGE_COLOR);
        cv::Mat depth = loadTemplateSrc(t, CV_LOAD_IMAGE_UNCHANGED);
        cv::cvtColor(rgb, gray, CV_BGR2GRAY);

        // Load orientation gradients
        cv::Mat gradients;
        quantizedGradients(gray, gradients, criteria->minMagnitude);

        // Load normals
        cv::Mat normals, normals3D;
        quantizedNormals(depth, normals, normals3D, t.camera.fx(), t.camera.fy(), t.maxDepth, static_cast<int>(criteria->maxDepthDiff / t.resizeRatio));

        // Load hue
        cv::Mat hue, hsv;
        cv::cvtColor(rgb, hsv, CV_BGR2HSV);
        normalizeHSV(hsv, hue);

        // Multiply normals and gradients to be better visible
        gradients *= 16;
        normals *= 2;

        // Normalize images to 8UC3
        depth.convertTo(depth, CV_8UC1, 255.0f / 65535.0f);
        cv::cvtColor(depth, depth, CV_GRAY2BGR);
        cv::cvtColor(gradients, gradients, CV_GRAY2BGR);
        cv::cvtColor(normals, normals, CV_GRAY2BGR);
        cv::cvtColor(hue, hue, CV_GRAY2BGR);

        // Define rois for every image
        cv::Rect rgbROI(rgb.cols + offset * 2, offset, rgb.cols, rgb.rows);
        cv::Rect depthROI(offset, offset, rgbROI.width, rgbROI.height);
        cv::Rect normalsROI(offset, offset + depthROI.br().y, rgbROI.width, rgbROI.height);
        cv::Rect gradientsROI(offset, offset + normalsROI.br().y, rgbROI.width, rgbROI.height);
        cv::Rect hueROI(offset, offset + gradientsROI.br().y, rgbROI.width, rgbROI.height);

        // Copy images to result
        rgb.copyTo(result(rgbROI));
        depth.copyTo(result(depthROI));
        normals.copyTo(result(normalsROI));
        gradients.copyTo(result(gradientsROI));
        hue.copyTo(result(hueROI));

        // Draw rectangle around all objects
        cv::rectangle(result, rgbROI.tl(), rgbROI.br(), cWhite, 1);
        cv::rectangle(result, depthROI.tl(), depthROI.br(), (highlight == 0 || highlight == 3) ? cGreen : cWhite, 1);
        cv::rectangle(result, normalsROI.tl(), normalsROI.br(), (highlight == 1) ? cGreen : cWhite, 1);
        cv::rectangle(result, gradientsROI.tl(), gradientsROI.br(), (highlight == 2) ? cGreen : cWhite, 1);
        cv::rectangle(result, hueROI.tl(), hueROI.br(), (highlight == 4) ? cGreen : cWhite, 1);

        // Draw features points in the active window
        if (settings[SETTINGS_FEATURE_POINT] && !features.empty()) {
            for (auto &feature : features) {
                cv::Scalar color = (feature.second == 1) ? cGreen : cRed;

                if (settings[SETTINGS_FEATURE_POINT_STYLE]) {
                    cv::Point tl = feature.first + offsetStart;
                    cv::Point br = feature.first + offsetEnd;

                    // Draw small rectangles around object sources
                    if (highlight == 0 || highlight == 3) { cv::rectangle(result, depthROI.tl() + tl, depthROI.tl() + br, color, 1); }
                    else if (highlight == 1) { cv::rectangle(result, normalsROI.tl() + tl, normalsROI.tl() + br, color, 1); }
                    else if (highlight == 2) { cv::rectangle(result, gradientsROI.tl() + tl, gradientsROI.tl() + br, color, 1); }
                    else if (highlight == 4) { cv::rectangle(result, hueROI.tl() + tl, hueROI.tl() + br, color, 1); }
                } else {
                    // Draw small circles around object sources
                    if (highlight == 0 || highlight == 3) { cv::circle(result, depthROI.tl() + feature.first, 1, color, -1); }
                    else if (highlight == 1) { cv::circle(result, normalsROI.tl() + feature.first, 1, color, -1); }
                    else if (highlight == 2) { cv::circle(result, gradientsROI.tl() + feature.first, 1, color, -1); }
                    else if (highlight == 4) { cv::circle(result, hueROI.tl() + feature.first, 1, color, -1); }
                }
            }
        }

        // Show template info
        std::ostringstream oss;
        cv::Point textTl(depthROI.width + offset * 2, depthROI.height + offset + 4);

        oss.str("");
        oss << "Tpl: " << t.fileName << " (" << t.objId << ")";
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "mode: " << t.camera.mode;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "elev: " << t.camera.elev;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "diameter: " << std::fixed << std::setprecision(2) << t.diameter;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "rszRatio: " << std::fixed << std::setprecision(2) << t.resizeRatio;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "objArea: " << t.objArea;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "avgDepth: " << t.features.avgDepth;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "distMin: " << t.features.avgDepth * criteria->depthDeviation;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        oss.str("");
        oss << "distMax: " << t.features.avgDepth / criteria->depthDeviation;
        textTl.y += 18;
        label(result, oss.str(), textTl);

        // Show results
        cv::imshow(title == nullptr ? "Template features" : title, result);
        cv::waitKey(wait);
    }
#endif

#ifdef VIZ_MATCHING
    bool Visualizer::matching(const ScenePyramid &scene, Template &candidate, std::vector<Window> &windows,
                              int &currentIndex, int &candidateIndex, const std::vector<std::vector<std::pair<cv::Point, int>>> &scores,
                              int patchOffset, int minThreshold, int wait, const char *title) {
        std::ostringstream oss;
        Window &window = windows[currentIndex];
        cv::Scalar cGreen(0, 255, 0), cRed(0, 0, 255), cBlue(255, 0, 0), cWhite(255, 255, 255), cGray(90, 90, 90);
        cv::Point offsetStart(-patchOffset, -patchOffset), offsetEnd(patchOffset, patchOffset);
        auto winSize = static_cast<int>(windows.size());
        auto canSize = static_cast<int>(window.candidates.size());

        // Load scene and define offsets
        const int offset = 15;
        const cv::Size slidingWinSize(window.width, window.height);
        const cv::Size stripSize(scene.srcRGB.cols + offset * 3, candidate.objBB.height * 4 + offset * 5);
        const cv::Size resultSize(stripSize.width + candidate.objBB.width, (scene.srcRGB.rows + 2 * offset > stripSize.height) ? scene.srcRGB.rows + 2 * offset : stripSize.height);

        // Define ROIs for scene sources
        cv::Rect sceneROI(cv::Rect(offset, offset, scene.srcRGB.cols,  scene.srcRGB.rows));
        cv::Rect depthROI(scene.srcRGB.cols + offset * 2, offset, slidingWinSize.width, slidingWinSize.height);
        cv::Rect normalsROI(depthROI.x, depthROI.y + slidingWinSize.height + offset, slidingWinSize.width, slidingWinSize.height);
        cv::Rect gradientsROI(normalsROI.x, normalsROI.y + slidingWinSize.height + offset, slidingWinSize.width, slidingWinSize.height);
        cv::Rect hueROI(gradientsROI.x, gradientsROI.y + slidingWinSize.height + offset, slidingWinSize.width, slidingWinSize.height);

        // Convert sources to 8UC3 so they can be copied
        cv::Mat depth = scene.srcDepth.clone(), normals = scene.srcNormals.clone();
        cv::Mat gradients = scene.srcGradients.clone(), hue = scene.srcHue.clone();
        depth.convertTo(depth, CV_8UC1, 255.0f / 65535.0f);
        cv::cvtColor(depth, depth, CV_GRAY2BGR);
        cv::cvtColor(hue, hue, CV_GRAY2BGR);

        // Multiply normals and gradients by 16 to be better visible
        gradients *= 16;
        normals *= 2;

        cv::cvtColor(normals, normals, CV_GRAY2BGR);
        cv::cvtColor(gradients, gradients, CV_GRAY2BGR);

        for (int i = 0; i < scores.size(); ++i) {
            // Copy scene to result
            cv::Mat result = cv::Mat::zeros(resultSize, CV_8UC3);
            scene.srcRGB.copyTo(result(sceneROI));
            depth(window.rect()).copyTo(result(depthROI));
            normals(window.rect()).copyTo(result(normalsROI));
            gradients(window.rect()).copyTo(result(gradientsROI));
            hue(window.rect()).copyTo(result(hueROI));

            // Offset sliding window
            cv::Rect offsetWindow(window.tl().x + offset, window.tl().y + offset, window.width, window.height);

            // Draw all windows with candidates
            if (settings[SETTINGS_GRID]) {
                for (auto &win : windows) {
                    cv::rectangle(result, win.tl() + sceneROI.tl(), win.br() + sceneROI.tl(), cv::Scalar(90, 90, 90), 1);
                }
            }

            // Draw points
            if (settings[SETTINGS_FEATURE_POINT]) {
                for (auto &score : scores[i]) {
                    cv::Scalar color = (score.second == 1) ? cGreen : cRed;

                    // Draw rectangles or points around feature points based on the settings
                    if (settings[SETTINGS_FEATURE_POINT_STYLE]) {
                        // Draw small rectangles around matched feature points
                        cv::Point tl = score.first + offsetStart;
                        cv::Point br = score.first + offsetEnd;
                        cv::rectangle(result, offsetWindow.tl() + tl, offsetWindow.tl() + br, color, 1);

                        // Draw small rectangles around object sources
                        if (i == 0 || i == 3) { cv::rectangle(result, depthROI.tl() + tl, depthROI.tl() + br, color, 1); }
                        else if (i == 1) { cv::rectangle(result, normalsROI.tl() + tl, normalsROI.tl() + br, color, 1); }
                        else if (i == 2) { cv::rectangle(result, gradientsROI.tl() + tl, gradientsROI.tl() + br, color, 1); }
                        else if (i == 4) { cv::rectangle(result, hueROI.tl() + tl, hueROI.tl() + br, color, 1); }
                    } else {
                        // Draw small circles around matched feature points
                        cv::circle(result, offsetWindow.tl() + score.first, 1, color, -1);

                        // Draw small circles around object sources
                        if (i == 0 || i == 3) { cv::circle(result, depthROI.tl() + score.first, 1, color, -1); }
                        else if (i == 1) { cv::circle(result, normalsROI.tl() + score.first, 1, color, -1); }
                        else if (i == 2) { cv::circle(result, gradientsROI.tl() + score.first, 1, color, -1); }
                        else if (i == 4) { cv::circle(result, hueROI.tl() + score.first, 1, color, -1); }
                    }
                }
            }

            // Draw rectangles around sources, highlight current test
            cv::rectangle(result, sceneROI.tl(), sceneROI.br(), cWhite, 1);
            cv::rectangle(result, depthROI.tl(), depthROI.br(), (i == 0 || i == 3) ? cGreen : cWhite, 1);
            cv::rectangle(result, normalsROI.tl(), normalsROI.br(), (i == 1) ? cGreen : cWhite, 1);
            cv::rectangle(result, gradientsROI.tl(), gradientsROI.br(), (i == 2) ? cGreen : cWhite, 1);
            cv::rectangle(result, hueROI.tl(), hueROI.br(), (i == 4) ? cGreen : cWhite, 1);

            // Annotate scene
            cv::rectangle(result, offsetWindow.tl(), offsetWindow.br(), cGreen, 1);
            cv::Point textTl(offsetWindow.br().x + 5, offsetWindow.tl().y + 10);

            // Draw scores
            float finalScore = 0;
            for (int l = 0; l < scores.size(); ++l) {
                int score = 0;

                for (auto &point : scores[l]) {
                    score += point.second;
                }

                if (settings[SETTINGS_INFO]) {
                    finalScore += score;

                    // Highlight current score
                    cv::Scalar sGreen = (l == i) ? cGreen : cv::Scalar(0, 170, 0);
                    cv::Scalar sRed = (l == i) ? cRed : cv::Scalar(0, 0, 170);

                    oss.str("");
                    oss << "s" << (l + 1) << ": " << score << "/" << scores[l].size();

                    if (l == 0) {
                        auto sDepth = scene.srcDepth.at<ushort>(windows[currentIndex].tl() + scores[0][0].first);
                        oss << " (" << sDepth << ")";
                        label(result, oss.str(), textTl, 0.4, 1, 1, cv::Scalar(0, 0, 0), (score == 1) ? sGreen : sRed);
                    } else {
                        label(result, oss.str(), textTl, 0.4, 1, 1, cv::Scalar(0, 0, 0), (score < minThreshold) ? sRed : sGreen);
                    }

                    textTl.y += 18;
                }
            }

            // Draw info next to sliding window
            if (settings[SETTINGS_INFO]) {
                oss.str("");
                oss << "Score: " << (finalScore / 100.0f);
                label(result, oss.str(), textTl);
                textTl.y += 18;

                oss.str("");
                oss << "Candidate: " << (candidateIndex + 1) << "/" << canSize;
                label(result, oss.str(), textTl);
            }

            // Scene info in top left corner
            if (settings[SETTINGS_TITLE]) {
                oss.str("");
                textTl.x = sceneROI.x + 1;
                textTl.y = sceneROI.y + 12;
                oss << "Locations: " << winSize;
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));

                oss.str("");
                oss << "Scale: " << scene.scale;
                textTl.y += 18;
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));

                oss.str("");
                oss << "Index: " << currentIndex << "/" << winSize;
                textTl.y += 18;
                label(result, oss.str(), textTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
            }

            // Show results and save key press
            tplMatch(candidate, scores[i], i, patchOffset, 1);
            cv::imshow(title == nullptr ? "Matched feature points" : title, result);
            int key = cv::waitKey(wait);

            // Navigation using arrow keys and spacebar
            if (key == KEY_RIGHT) {
                currentIndex = (currentIndex + 1 < winSize) ? currentIndex : winSize - 2;
                return true;
            } else if (key == KEY_UP) {
                currentIndex = (currentIndex + 10 < winSize) ? currentIndex + 9 : winSize - 2;
                return true;
            } else if (key == KEY_ENTER) {
                currentIndex = (currentIndex + 100 < winSize) ? currentIndex + 99 : winSize - 2;
                return true;
            } else if (key == KEY_LEFT) {
                currentIndex = (currentIndex - 1 > 0) ? currentIndex - 2 : -1;
                return true;
            } else if (key == KEY_DOWN) {
                currentIndex = (currentIndex - 10 > 0) ? currentIndex - 11 : -1;
                return true;
            } else if (key == KEY_SPACEBAR) {
                currentIndex = (currentIndex - 100 > 0) ? currentIndex - 99 : -1;
                return true;
            } else if (key == KEY_K) {
                i = (i + 1 < scores.size()) ? i : -1; // Switch current test (scores)
            } else if (key == KEY_G) {
                settings[SETTINGS_GRID] = !settings[SETTINGS_GRID];
                i = i - 1;
            } else if (key == KEY_T) {
                settings[SETTINGS_TITLE] = !settings[SETTINGS_TITLE];
                i = i - 1;
            } else if (key == KEY_I) {
                settings[SETTINGS_INFO] = !settings[SETTINGS_INFO];
                i = i - 1;
            } else if (key == KEY_L) {
                settings[SETTINGS_FEATURE_POINT_STYLE] = !settings[SETTINGS_FEATURE_POINT_STYLE];
                i = i - 1;
            } else if (key == KEY_J) {
                settings[SETTINGS_FEATURE_POINT] = !settings[SETTINGS_FEATURE_POINT];
                i = i - 1;
            } else if (key == KEY_S) {
                currentIndex = winSize + 1;
                return true; // Skip all windows
            } else if (key == KEY_C) {
                candidateIndex = (candidateIndex - 1 > 0) ? candidateIndex - 2 : -1;
                return false;
            } else if (key == KEY_V) {
                candidateIndex = (candidateIndex + 1 < canSize) ? candidateIndex : canSize - 2;
                return false;
            }
        }

        return false;
    }
#endif

#ifdef VIZ_RESULTS
    void Visualizer::matches(const ScenePyramid &scene, std::vector<Match> &matches, int wait, const char *title) {
        std::ostringstream oss;
        const int offset = 15, textWidth = 160;
        const auto matchSize = static_cast<const int>(matches.size());
        const int perRow = (matchSize > 7) ? 7 : matchSize;

        while (true) {
            cv::Mat result = scene.srcRGB.clone();

            if (!matches.empty()) {
                // Define final mosaic image size
                cv::Size cellSize = matches[0].objBB.size();
                auto cols = static_cast<int>(std::ceil(matchSize / static_cast<float>(perRow)));
                cv::Mat tplMosaic = cv::Mat::zeros(matches[0].objBB.height * perRow + (perRow * 2) * offset,
                                                   matches[0].objBB.width * cols + textWidth * cols + (cols * 2 + 1) * offset, CV_8UC3);

                // Size of one column and row including text
                int sizeX = offset * 2 + cellSize.width + textWidth;
                int sizeY = offset * 2 + cellSize.height;

                // Go through each match and draw it on scene + tpl mosaic
                for (int i = 0; i < matchSize; ++i) {
                    Match &match = matches[i];
                    cv::Rect scaledBB = match.normObjBB;

                    // Calculate col, row and rect inside defined mosaic grid
                    int col = (i / perRow);
                    int row = (i % perRow);
                    cv::Rect rect(col * sizeX + offset, row * sizeY + offset, cellSize.width, cellSize.height);

                    // Draw rectangles around found matches on the scene
                    cv::rectangle(result, scaledBB.tl(), scaledBB.br(), cv::Scalar(0, 255, 0));

                    // Load template src image
                    cv::Mat tplSrc = loadTemplateSrc(*match.t);
                    tplSrc.copyTo(tplMosaic(rect));

                    // Draw green template overlay into scene
                    if (settings[SETTINGS_TPL_OVERLAY]) {
                        cv::Mat tplGray;
                        cv::cvtColor(tplSrc, tplGray, CV_BGR2GRAY);
                        cv::resize(tplGray, tplGray, cv::Size(), 1.0f / match.scale, 1.0f / match.scale);

                        for (int y = 0; y < tplGray.rows; y++) {
                            for (int x = 0; x < tplGray.cols; x++) {
                                uchar px = tplGray.at<uchar>(y, x);
                                if (px > 30) {
                                    result.at<cv::Vec3b>(y + scaledBB.y, x + scaledBB.x) = cv::Vec3b(0, px, 0);
                                }
                            }
                        }
                    }

                    // Annotate templates in mosaic
                    cv::rectangle(tplMosaic, rect, cv::Scalar(200, 200, 200), 1);

                    // Show text info next to each template
                    cv::Point mosaicTextTl(rect.x + rect.width + 5, rect.y - 8);
                    oss.str("");
                    oss << "Template: " << match.t->fileName << " (" << (match.t->objId) << ")";
                    mosaicTextTl.y += 18;
                    label(tplMosaic, oss.str(), mosaicTextTl);

                    oss.str("");
                    oss << "Score: " << match.score;
                    mosaicTextTl.y += 18;
                    label(tplMosaic, oss.str(), mosaicTextTl);

                    oss.str("");
                    oss << "Scale: " << match.scale;
                    mosaicTextTl.y += 18;
                    label(tplMosaic, oss.str(), mosaicTextTl);

                    // Text info on the scene
                    if (settings[SETTINGS_INFO]) {
                        cv::Point textTl(scaledBB.br().x + 5, scaledBB.tl().y - 8);
                        oss.str("");
                        oss << "Template: " << match.t->fileName << " (" << match.t->objId << ")";
                        textTl.y += 18;
                        label(result, oss.str(), textTl);

                        oss.str("");
                        oss << "Score: " << std::fixed << std::setprecision(2) << match.score;
                        textTl.y += 18;
                        label(result, oss.str(), textTl);

                        oss.str("");
                        oss << "Scale: " << std::fixed << std::setprecision(2) << match.scale;
                        textTl.y += 18;
                        label(result, oss.str(), textTl);
                    }
                }

                cv::imshow("Matched templates", tplMosaic);
            }

            // Draw info in the top left corner of the scene
            if (settings[SETTINGS_TITLE]) {
                cv::Point sceneTl(-4, 12);
                oss.str("");
                oss << " Matches: " << matchSize;
                label(result, oss.str(), sceneTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
            }

            cv::imshow(title == nullptr ? "Matched results" : title, result);
            int key = cv::waitKey(wait);

            // Controls using keys
            if (key == KEY_K) {
                settings[SETTINGS_TPL_OVERLAY] = !settings[SETTINGS_TPL_OVERLAY];
            } else if (key == KEY_I) {
                settings[SETTINGS_INFO] = !settings[SETTINGS_INFO];
            } else if (key == KEY_T) {
                settings[SETTINGS_TITLE] = !settings[SETTINGS_TITLE];
            } else {
                break;
            }
        }
    }
#endif

#ifdef VIZ_NMS
    void Visualizer::preNonMaxima(const ScenePyramid &scene, std::vector<Match> &matches, int wait, const char *title) {

        std::ostringstream oss;
        const auto matchSize = static_cast<const int>(matches.size());

        // Sort all matches by their highest score
        std::vector<Match> sortedMatches = matches;
        std::sort(sortedMatches.rbegin(), sortedMatches.rend());

        for (int i = 0; i < matchSize; ++i) {
            cv::Mat result = scene.srcRGB.clone();
            Match &currentMatch = sortedMatches[i];
            cv::Rect currentBB = currentMatch.normObjBB;

            // Vizualize matched template
            tplMatch(*currentMatch.t, {}, -1, 0, 1, "Matched template");

            if (!sortedMatches.empty()) {
                // Draw rectangles for other matches
                for (auto &match : sortedMatches) {
                    cv::Rect scaledBB = match.normObjBB;
                    cv::rectangle(result, scaledBB, cv::Scalar(90, 90, 90));
                }
            }

            // Draw current match
            cv::rectangle(result, currentBB, cv::Scalar(0, 255, 0));

            // Draw info in the top left corner of the scene
            if (settings[SETTINGS_TITLE]) {
                cv::Point sceneTl(-4, 12);
                oss.str("");
                oss << " Matches: " << matchSize;
                label(result, oss.str(), sceneTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
                sceneTl.y += 17;
            }

            // Text info on the scene
            if (settings[SETTINGS_INFO]) {
                cv::Point textTl(currentBB.br().x + 5, currentBB.tl().y - 8);
                oss.str("");
                oss << "Template: " << currentMatch.t->fileName << " (" << currentMatch.t->objId << ")";
                textTl.y += 18;
                label(result, oss.str(), textTl);

                oss.str("");
                oss << "Score: " << currentMatch.score;
                textTl.y += 18;
                label(result, oss.str(), textTl);

                oss.str("");
                oss << "Scale: " << currentMatch.scale;
                textTl.y += 18;
                label(result, oss.str(), textTl);

                oss.str("");
                oss << "Match: " << (i + 1) << "/" << matchSize;
                textTl.y += 18;
                label(result, oss.str(), textTl);
            }

            // Draw info in the top left corner of the scene
            if (settings[SETTINGS_TITLE]) {
                cv::Point sceneTl(-4, 12);
                oss.str("");
                oss << " Matches: " << matchSize;
                label(result, oss.str(), sceneTl, 0.4, 2, 1, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
            }

            cv::imshow(title == nullptr ? "Matched results" : title, result);
            int key = cv::waitKey(wait);

            // Controls using keys
            if (key == KEY_RIGHT) {
                i = (i + 1 < matchSize) ? i : matchSize - 2;
            } else if (key == KEY_UP) {
                i = (i + 10 < matchSize) ? i + 9 : matchSize - 2;
            } else if (key == KEY_ENTER) {
                i = (i + 100 < matchSize) ? i + 99 : matchSize - 2;
            } else if (key == KEY_LEFT) {
                i = (i - 1 > 0) ? i - 2 : -1;
            } else if (key == KEY_DOWN) {
                i = (i - 10 > 0) ? i - 11 : -1;
            } else if (key == KEY_SPACEBAR) {
                i = (i - 100 > 0) ? i - 99 : -1;
            } else if (key == KEY_G) {
                settings[SETTINGS_GRID] = !settings[SETTINGS_GRID];
            } else if (key == KEY_I) {
                settings[SETTINGS_INFO] = !settings[SETTINGS_INFO];
            } else if (key == KEY_T) {
                settings[SETTINGS_TITLE] = !settings[SETTINGS_TITLE];
            } else if (key == KEY_S) {
                break; // Skip
            }
        }
    }
#endif
}