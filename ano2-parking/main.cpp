#include <iostream>
#include "objdetect/Classifier.h"

int main() {
    Classifier classifier("strecha1_map.txt");

    // Neural
////    classifier.train("full.txt", "free.txt", "train_neural.txt", "neural.xml", Classifier::NEURAL);
    classifier.recognize("test.txt", "neural.xml", "results_neural.txt", "groundtruth.txt", Classifier::NEURAL);

    // SVM
////    classifier.train("full.txt", "free.txt", "train_svm.txt", "svm.xml", Classifier::SVM);
    classifier.recognize("test.txt", "svm.xml", "results_svm.txt", "groundtruth.txt", Classifier::SVM);

    return 0;
}