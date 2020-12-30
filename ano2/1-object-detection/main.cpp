#include <iostream>
#include <opencv2/opencv.hpp>

#define SQR(x) ((x) * (x))
#define RAD(x) ((x) * (M_PI / 180.0))
#define DEG(x) ((x) * (180.0 / M_PI))

static cv::CascadeClassifier cascadeClassifier;

void detectFace(cv::Mat &frame, std::vector<cv::Rect> &faceVector) {
    std::vector<cv::Rect> detectedFaces;
    cv::GaussianBlur(frame, frame, cv::Size(3, 3), 1);

    cascadeClassifier.detectMultiScale(frame, detectedFaces, 1.2, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(100, 100));
    faceVector.swap(detectedFaces);
}

int main() {
    cascadeClassifier.load("data/haarcascades/haarcascade_frontalface_default.xml");
    cv::namedWindow("detect", 0);
    bool run = true;

    cv::VideoCapture vc(0);
    vc.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    vc.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    vc.set(CV_CAP_PROP_FPS, 30.0);

    while (run) {
        std::vector<cv::Rect> faceVector;
        cv::Mat frame;

        // Get frame
        vc >> frame;
        detectFace(frame, faceVector);

        for (auto vector : faceVector) {
            cv::rectangle(frame, vector, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("detect", frame);
        cv::waitKey(5);
    }

    return 0;
}