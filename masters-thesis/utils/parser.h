#ifndef VSB_SEMESTRAL_PROJECT_PARSER_H
#define VSB_SEMESTRAL_PROJECT_PARSER_H

#include <string>
#include <utility>
#include <memory>
#include "../core/template.h"
#include "../core/classifier_criteria.h"
#include "../core/scene.h"

namespace tless {
    /**
     * @brief  Utility class to parse templates downloaded from http://cmp.felk.cvut.cz/t-less/.
     */
    class Parser {
    private:
        static int idCounter;
        cv::Ptr<ClassifierCriteria> criteria;
        std::vector<int> objEdgels;

        /**
         * @brief Parses template images and generates gray, hue and gradient images for each template.
         *
         * @param[in,out] t    Template object, loaded from info.yml meta to load images and additional data for
         * @param[in] basePath Base path to object folder
         */
        void parseTemplate(Template &t, const std::string &basePath);

        /**
         * @brief Parses criteria for given template + generates quantized normals.
         *
         * @param[in,out] t Template to extract criteria and normals for
         */
        void parseCriteriaAndNormals(Template &t);

        /**
         * @brief Creates one level of scene pyramid, by scaling src images and updating camera intristics
         *
         * @param[in] scale Current scale of the image pyramid
         * @param[in] rgb   Input RGB image
         * @param[in] depth Input Depth Image (16-bit)
         * @param[in] K     Camera intristic params
         * @param[in] R     Camera rotation matrix
         * @param[in] t     Camera translation vector
         * @return          New level of Scene pyramid at given scale
         */
        ScenePyramid createPyramid(float scale, const cv::Mat &rgb, const cv::Mat &depth, const cv::Mat &gray, const cv::Mat &hue,
                                           const cv::Mat &K, const cv::Mat &R, const cv::Mat &t);

    public:
        Parser(cv::Ptr<ClassifierCriteria> criteria) : criteria(criteria) {};

        /**
         * @brief Parses templates for one object in given path.
         *
         * Function expects rgb/, depth/ folders and gt.yml and info.yml files in root folder of basePath.
         *
         * @param[in]     basePath  Path to object folder
         * @param[out]    templates Output vector containing all parsed templates
         */
        void parseObject(const std::string &basePath, std::vector<Template> &templates);

        /**
         * @brief Parses scene info, images, computes quantized normals and gradients.
         *
         * @param[in]     basePath Base path to scene folder with info.yml and rgb, depth folders
         * @param[in]     index    Current index of a scene image
         * @param[in]     scaleFactor    Current scale of image scale pyramid
         * @return                 Parsed scene object
         */
        Scene parseScene(const std::string &basePath, int index, float scaleFactor, int levelsUp, int levelsDown);
    };
}

#endif
