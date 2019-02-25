#ifndef FACEEXTRACTOR_H
#define FACEEXTRACTOR_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/face/facemark.hpp>
#include <opencv2/face/facemarkLBF.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace cv;
using namespace cv::face;

class FaceExtractor
{
    public:
        FaceExtractor(std::string cascadeClassifierPath, std::string LBFmodelPath);
        std::vector<Mat> extractFaces(Mat frame, Size faceSize);
        virtual ~FaceExtractor();
        static Point2f averagePoints(std::vector<Point2f> points);
        static Point2f averagePoints(Point2f pointA, Point2f pointB);
        std::vector<Point2f> getEyePoints(std::vector<Point2f> points, unsigned short side);

    protected:
        CascadeClassifier faceDetector;
        std::vector<int> leftEyeIndex;
        std::vector<int> rightEyeIndex;
        FacemarkLBF::Params params;
        Ptr<Facemark> facemark;

    private:
        Mat gray;
        Mat cropped;
        Mat warp_matrix;
        Point2f destTriangle[3];
        Point2f srcTriangle[3];
        std::vector<std::vector<Point2f>> landmarks;
        std::vector<Rect> faces;
        std::vector<int> detectionCount;
};

#endif // FACEEXTRACTOR_H
