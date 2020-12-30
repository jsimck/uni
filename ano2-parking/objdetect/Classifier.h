#ifndef VSB_ANO2_PARKING_CLASSIFIER_H
#define VSB_ANO2_PARKING_CLASSIFIER_H

#include <opencv2/opencv.hpp>
#include <utility>
#include "../core/Place.h"

class Classifier {
private:
    cv::Ptr<cv::ml::SVM> svm;
    cv::Ptr<cv::ml::ANN_MLP> machineBrain;
    cv::Size neuralSize{40, 40};
    std::string geometryDataPath;

    float featureColorGradient(const cv::Mat &place, std::vector<float> &features);
    float featureHOG(const cv::Mat &place, std::vector<float> &features);
    float featureCanny(const cv::Mat &place, uchar t1 = 80, uchar t2 = 60);
    float featureSobel(const cv::Mat &place, int minThreshold = 65);
    float featureMorphologyGradient(const cv::Mat &place, float minThreshold = 0.2f);

    void extractFeatures(Place &parkingPlace, std::vector<float> &features, int type);
    void persistTrainVector(std::vector<std::vector<float>> &samples, std::vector<float> &results, std::string trainVectorPath, bool clear = false);

    void loadTrainVector(cv::Mat &samples, cv::Mat &responses, std::string trainVectorPath, int type);
    void generateTrainVector(std::string &filesListPath, std::string trainVectorPath, float expectedResult, int type, bool clear = false);

    void evaluate(std::string groundTruthFile, std::string resultsFilePath);
public:
    static enum Type : int { NEURAL = 0, SVM = 1 };
    static void drawParkingPlaces(cv::Mat &scene, std::vector<Place> &parkingPlaces);

    explicit Classifier(std::string geometryDataPath) : geometryDataPath(std::move(geometryDataPath)) {}

    void train(std::string posFilesListPath, std::string negFilesListPath, std::string trainVectorPath, std::string trainedClassifierPath, int type);
    void recognize(std::string testFilesListPath, std::string trainedClassifierPath, std::string resultsFilePath,
                   std::string groundTruthFile, int type = SVM);
};

#endif //VSB_ANO2_PARKING_CLASSIFIER_H
