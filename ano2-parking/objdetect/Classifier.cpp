#include "Classifier.h"
#include <opencv2/ml/ml.hpp>
#include <utility>
#include "../utils/Loader.h"

void Classifier::drawParkingPlaces(cv::Mat &scene, std::vector<Place> &parkingPlaces) {
    int occupied = 0;

    for (auto &place : parkingPlaces) {
        cv::Scalar color = (place.occupied) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
        occupied = (place.occupied) ? occupied + 1 : occupied;

        // Draw boundaries
        cv::line(scene, cv::Point(place.x1, place.y1), cv::Point(place.x2, place.y2), color);
        cv::line(scene, cv::Point(place.x2, place.y2), cv::Point(place.x3, place.y3), color);
        cv::line(scene, cv::Point(place.x3, place.y3), cv::Point(place.x4, place.y4), color);

        // Avg out center
        cv::Point center((place.x1 + place.x2 + place.x3 + place.x4) / 4, (place.y1 + place.y2 + place.y3 + place.y4) / 4);
        cv::circle(scene, center, 6, color, -1);
    }

    // Draw info text in top left corner
    cv::rectangle(scene, cv::Point(0, 0), cv::Point(130, 55), cv::Scalar(0, 0, 0), -1);
    std::stringstream ss;
    ss << "Occupied: " << occupied;
    cv::putText(scene, ss.str(), cv::Point(10, 20), CV_FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 1, CV_AA);
    ss.str("");
    ss << "Free: " << parkingPlaces.size() - occupied;
    cv::putText(scene, ss.str(), cv::Point(10, 40), CV_FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 255, 0), 1, CV_AA);
}

float Classifier::featureMorphologyGradient(const cv::Mat &place, float minThreshold) {
    // Morphology gradient - sobel alternative
    cv::Mat placeClone = place.clone();
    cv::resize(placeClone, placeClone, cv::Size(80, 80), 0, 0, CV_INTER_CUBIC);

    // Blur
    cv::medianBlur(placeClone, placeClone, 3);
    cv::Mat gradMat;
    int morph_elem = 2;
    int morph_size = 2;

    cv::Mat element = cv::getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
    cv::morphologyEx(placeClone, gradMat, cv::MORPH_GRADIENT, element);
    cv::cvtColor(gradMat, gradMat, CV_BGR2GRAY);

    cv::Mat gradFloat;
    gradMat.convertTo(gradFloat, CV_32FC1, 1.0f / 255.0f);
    float count = 0;

    for (int y = 0; y < gradFloat.rows; y++) {
        for (int x = 0; x < gradFloat.cols; x++) {
            if (gradFloat.at<float>(y, x) > minThreshold) {
                count++;
            }
        }
    }

    return count;
}

float Classifier::featureCanny(const cv::Mat &place, uchar t1, uchar t2) {
    cv::Mat srcGray, edges;

    // Convert and blur
    cv::cvtColor(place, srcGray, CV_BGR2GRAY);
    cv::medianBlur(srcGray, srcGray, 9);

    // Canny
    cv::Canny(srcGray, edges, t1, t2);

    // Count edges
    float n = 0;
    for (int y = 0; y < edges.rows; y++) {
        for (int x = 0; x < edges.cols; x++) {
            if (edges.at<uchar>(y, x) > 0) {
                n++;
            }
        }
    }

    return n;
}

float Classifier::featureColorGradient(const cv::Mat &place, std::vector<float> &features) {
    // Init matrices, convert BGR to HSV
    cv::Mat hsvPlace, hueHist;
    cv::cvtColor(place, hsvPlace, CV_BGR2HSV);
    std::vector<cv::Mat> hsvPlanes;
    cv::split(hsvPlace, hsvPlanes);

    /// Establish the number of bins
    int histSize = 180;

    /// Set the ranges ( for B,G,R) )
    float range[] = {0, 180};
    const float *histRange = {range};

    /// Compute the histograms:
    cv::calcHist(&hsvPlanes[0], 1, 0, cv::Mat(), hueHist, 1, &histSize, &histRange, true, false);

    /// Draw for each channel
    for (int y = 0; y < hueHist.rows; y++) {
        features.emplace_back(hueHist.at<uchar>(y));
    }
}

float Classifier::featureHOG(const cv::Mat &place, std::vector<float> &features) {
    std::vector<float> descriptors;

    // placesSize(160, 176)
    cv::HOGDescriptor hog;
    hog.winSize = place.size();
    hog.blockSize = cv::Size(128, 128);
    hog.cellSize = cv::Size(64, 64);
    hog.compute(place, descriptors);

    for (auto &d : descriptors) {
        features.emplace_back(d);
    }
}

float Classifier::featureSobel(const cv::Mat &place, int minThreshold) {
    cv::Mat src = place.clone();
    cv::Mat srcGray, gradX, gradY, grad;

    // Checks
    assert(src.type() == CV_8UC3); // 8UC3

    // Convert to gray and blur
    cv::cvtColor(src, srcGray, CV_BGR2GRAY);
    cv::medianBlur(srcGray, srcGray, 3);

    /// Gradient X & Y
    Sobel(srcGray, gradX, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    Sobel(srcGray, gradY, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

    // Convert to 8UC1
    convertScaleAbs(gradX, gradX);
    convertScaleAbs(gradY, gradY);

    // Checks
    assert(gradX.type() == CV_8UC1); // 8UC1
    assert(gradY.type() == CV_8UC1); // 8UC1
    assert(grad.type() == CV_8UC1); // 8UC1

    /// Total Gradient (approximate)
    addWeighted(gradX, 0.5, gradY, 0.5, 0, grad);

    // Count edges
    float edges = 0;
    for (int y = 0; y < grad.rows; y++) {
        for (int x = 0; x < grad.cols; x++) {
            if (grad.at<uchar>(y, x) > minThreshold) {
                edges++;
            }
        }
    }

    return edges;
}

void Classifier::extractFeatures(Place &parkingPlace, std::vector<float> &features, int type) {
    if (type == SVM) {
        // Blocks
        int step = 2; // overlap
        const cv::Size blockCount(8, 8);
        int blkWidth = (parkingPlace.frame.cols / blockCount.width), blkHeight = (parkingPlace.frame.rows / blockCount.height);
        int blkXStep = blkWidth / step, blkYStep = blkWidth / step;

        for (int y = 0; y + blkHeight < parkingPlace.frame.rows; y += blkYStep) {
            for (int x = 0; x + blkHeight < parkingPlace.frame.cols; x += blkXStep) {
                cv::Mat blk = parkingPlace.frame(cv::Rect(x, y, blkWidth, blkHeight));

                // False positive reduction
                if (parkingPlace.x1 == 149 && parkingPlace.y1 == 699 && x >= 60 && x <= 110 && y >= 60 && y <= 110) {
                    blk = parkingPlace.frame(cv::Rect(60 - blkWidth, 60 - blkHeight, blkWidth, blkHeight));
                }

//                features.emplace_back(featureSobel(blk, 70));
                features.emplace_back(featureCanny(blk, 80, 60));
            }
        }

        // Features for whole image
//        featureColorGradient(parkingPlace.frame, features);
        featureHOG(parkingPlace.frame, features);
    } else {
        // Resize to smaller block
        cv::Mat edges, srcGray = parkingPlace.frame.clone();
        cv::resize(srcGray, srcGray, neuralSize);

        // Run canny
        cv::Canny(srcGray, edges, 80, 60);
        for (int y = 0; y < edges.rows; y++) {
            for (int x = 0; x < edges.cols; x++) {
                features.emplace_back(static_cast<float>(edges.at<uchar>(y, x)));
            }
        }
    }
}

void Classifier::persistTrainVector(std::vector<std::vector<float>> &samples, std::vector<float> &results, std::string trainVectorPath, bool clear) {
    std::ofstream ofs;

    if (clear) {
        ofs.open(trainVectorPath);
    } else {
        ofs.open(trainVectorPath, std::ofstream::app);
    }

    // Checks
    assert(ofs.is_open());
    assert(samples.size() == results.size());

    const size_t resultsSize = results.size();
    for (int i = 0; i < resultsSize; i++) {
        for (auto &sample : samples[i]) {
            ofs << sample << ",";
        }

        ofs << results[i] << std::endl;
    }

    ofs.close();
}

void Classifier::generateTrainVector(std::string &filesListPath, std::string trainVectorPath, float expectedResult, int type, bool clear) {
    // Open file list
    std::ifstream ofs;
    ofs.open(filesListPath);
    assert(ofs.is_open());

    bool first = clear; // Clear file on first pass
    std::vector<std::vector<float>> samples;
    std::vector<float> results, features;

    // Extract features for each file
    while (!ofs.eof()) {
        std::string line;
        ofs >> line;
        
        std::cout << "Loading: " << line << std::endl;
        cv::Mat scene = cv::imread(line, CV_LOAD_IMAGE_COLOR);

        // Checks
        assert(scene.type() == CV_8UC3);

        // Load places geometry
        std::vector<Place> places;
        Loader::loadParkingPlaces(geometryDataPath, scene, places);

        for (auto &parkingPlace : places) {
            extractFeatures(parkingPlace, features, type);
            samples.push_back(features);
            results.push_back(expectedResult);
            features.clear();
        }

        // Persist trained data
        persistTrainVector(samples, results, trainVectorPath, first);
        first = false;

        // Clear vectors for other uses
        samples.clear();
        results.clear();
    }

    ofs.close();
}

void Classifier::loadTrainVector(cv::Mat &samples, cv::Mat &responses, std::string trainVectorPath, int type) {
    // Init train vectors
    std::vector<std::vector<float>> descriptor;
    std::vector<float> classes;

    // Open file with training samples
    std::ifstream ifs;
    ifs.open(trainVectorPath);
    assert(ifs.is_open());

    // Load and parse training samples
    while(!ifs.eof()) {
        std::string str;
        ifs >> str;

        std::vector<float> desc;
        size_t found = str.find(',');

        // Parse descriptors in format [d1, d2, d3, result]
        while (found != std::string::npos) {
            // Parse descriptor
            std::string t = str.substr(0, found);
            float d = std::stof(t);
            float nearest = floorf(d * 1000.0f + 0.5f) / 1000.0f;
            desc.push_back(nearest);

            // Find other descriptors
            str = str.substr(found + 1);
            found = str.find(',');
        }

        // Parse result
        if (!desc.empty()) {
            descriptor.push_back(desc);
            classes.push_back(std::stof(str));
        }

        // Checks
        assert(!classes.empty());
        assert(!descriptor.empty());
    }

    ifs.close();

    // Create samples and responses matrices
    auto numSamples = static_cast<int>(descriptor.size());
    auto numFeatures = static_cast<int>(descriptor[0].size());

    samples = cv::Mat(numSamples, numFeatures, CV_32F);
    if (type == SVM) {
        responses = cv::Mat(static_cast<int>(descriptor.size()), 1, CV_32S);
    } else {
        responses = cv::Mat(numSamples, 2, CV_32F);
    }

    // Fill responses and samples matrices with extracted trained data
    for (int i = 0; i < numSamples; i++) {
        for (int j = 0; j < numFeatures; j++) {
            samples.at<float>(i, j) = descriptor[i][j];

            if (type == SVM) {
                responses.at<int>(i, 0) = static_cast<int>(classes[i]);
            } else {
                responses.at<float>(i, 0) = 0;
                responses.at<float>(i, 0) = 0;
                responses.at<float>(i, static_cast<int>(classes[i])) = 1;
            }
        }
    }

    // Check for correct data format
    assert(samples.type() == CV_32F);
    assert(responses.type() == CV_32S || responses.type() == CV_32F);
}

void Classifier::recognize(std::string testFilesListPath, std::string trainedClassifierPath, std::string resultsFilePath,
                           std::string groundTruthFile, int type) {
    // Init classifier
    std::vector<Place> places;
    std::vector<float> features;

    if (type == SVM) {
        svm = cv::ml::SVM::load(trainedClassifierPath);
    } else {
        machineBrain = cv::ml::ANN_MLP::load(trainedClassifierPath);
    }

    // Init file stream to load test scenes
    std::ifstream ifs;
    ifs.open(testFilesListPath);
    assert(ifs.is_open());

    // Save results
    std::ofstream resultsStream;
    resultsStream.open(resultsFilePath);
    assert(resultsStream.is_open());

    // Recognize places for each test scene
    while (!ifs.eof()) {
        int occupied = 0;
        std::string line;
        ifs >> line;

        // Load scene
        cv::Mat scene = cv::imread(line, CV_LOAD_IMAGE_COLOR);

        // Load geometry
        Loader::loadParkingPlaces(geometryDataPath, scene, places);

        // Extract features and run detection for each parking place
        for (auto &place : places) {
            extractFeatures(place, features, type);
            auto numFeatures = static_cast<int>(features.size());
            cv::Mat resMat = cv::Mat(1, numFeatures, CV_32FC1);

            // Fill features mat
            for (int i = 0; i < numFeatures; i++) {
                resMat.at<float>(0, i) = features[i];
            }

            // Run prediction
            float response = (type == SVM) ? svm->predict(resMat) : machineBrain->predict(features);
            place.occupied = response > 0;
            occupied += place.occupied;

            // Save results
            resultsStream << place.occupied << std::endl;
            features.clear();
        }

        // Show results
//        cv::Mat results = scene.clone();
//        drawParkingPlaces(results, places);
//        cv::namedWindow("Results", CV_WINDOW_NORMAL);
//        cv::imshow("Results", results);
//        cv::waitKey(0);

        places.clear();
    }

    ifs.close();
    resultsStream.close();

    // Evaluate
    evaluate(std::move(groundTruthFile), resultsFilePath);
}

void Classifier::evaluate(std::string groundTruthFile, std::string resultsFilePath) {
    // Load files
    std::ifstream groundTruthStream, resultsStream;
    groundTruthStream.open(groundTruthFile);
    resultsStream.open(resultsFilePath);
    assert(groundTruthStream.is_open());
    assert(resultsStream.is_open());

    int detectorLine, groundTruthLine;
    int falsePositives = 0;
    int falseNegatives = 0;
    int truePositives = 0;
    int trueNegatives = 0;

    while (true) {
        if (!(resultsStream >> detectorLine)) break;
        groundTruthStream >> groundTruthLine;

        int detect = detectorLine;
        int ground = groundTruthLine;

        //false positives
        if ((detect == 1) && (ground == 0)) {
            falsePositives++;
        }

        //false negatives
        if ((detect == 0) && (ground == 1)) {
            falseNegatives++;
        }

        //true positives
        if ((detect == 1) && (ground == 1)) {
            truePositives++;
        }

        //true negatives
        if ((detect == 0) && (ground == 0)) {
            trueNegatives++;
        }
    }

    groundTruthStream.close();
    resultsStream.close();

    std::cout << "falsePositives " << falsePositives << std::endl;
    std::cout << "falseNegatives " << falseNegatives << std::endl;
    std::cout << "truePositives " << truePositives << std::endl;
    std::cout << "trueNegatives " << trueNegatives << std::endl;
    float acc = (float) (truePositives + trueNegatives) /
                (float) (truePositives + trueNegatives + falsePositives + falseNegatives);
    std::cout << "Accuracy " << acc << std::endl;
}

void Classifier::train(std::string posFilesListPath, std::string negFilesListPath, std::string trainVectorPath, std::string trainedClassifierPath, int type) {
    // Generate train vector for positive and negative data
    generateTrainVector(posFilesListPath, trainVectorPath, 1.0f, type, true);
    generateTrainVector(negFilesListPath, trainVectorPath, (type == SVM) ? -1.0f : 0.0f, type);

    // Load train vector
    cv::Mat samples, responses;
    loadTrainVector(samples, responses, std::move(trainVectorPath), type);

    if (type == SVM) {
        // Initialize Criteria
        cv::TermCriteria criteria;
        criteria.type = CV_TERMCRIT_EPS;
        criteria.epsilon = 1.0;
        criteria.maxCount = 2000;

        std::cout << "Initializing SVM..." << std::endl;

        // Initialize SVM
        svm = cv::ml::SVM::create();
        svm->setType(cv::ml::SVM::C_SVC);
        svm->setKernel(cv::ml::SVM::INTER);
        svm->setGamma(0.1);
        svm->setC(2.0);
        svm->setNu(0.1);
        svm->setTermCriteria(criteria);

        std::cout << "Training SVM...";

        // Train and save the result
        if (!svm->train(samples, cv::ml::ROW_SAMPLE, responses)) {
            std::cout << "ERROR: training failed" << std::endl;
        } else {
            svm->save(trainedClassifierPath);
            std::cout << "FINISHED" << std::endl;
        }
    } else {
        // Set layers
        cv::Mat layers = cv::Mat(3, 1, CV_32S);
        layers.at<int>(0, 0) = samples.cols; // Input neurons -> number of features in samples
        layers.at<int>(1, 0) = samples.cols; // Middle layer -> experiment
        layers.at<int>(2, 0) = responses.cols; // Number of results (0 - empty, 1 - full)

        // Init neural network
        machineBrain = cv::ml::ANN_MLP::create();
        machineBrain->setLayerSizes(layers);
        machineBrain->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);

        // Train and save the result
        std::cout << "Training neural network... ";
        if (!machineBrain->train(samples, cv::ml::ROW_SAMPLE, responses)) {
            std::cout << "ERROR: training failed" << std::endl;
        } else {
            machineBrain->save(trainedClassifierPath);
            std::cout << "FINISHED" << std::endl;
        }
    }
}