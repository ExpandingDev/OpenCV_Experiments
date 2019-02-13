#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/face/facemark.hpp>
#include <opencv2/face/facemarkLBF.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <FaceExtractor.h>

using namespace std;
using namespace cv;
using namespace cv::face;

vector<int> leftEyeIndex;
vector<int> rightEyeIndex;

/// Based upon: https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/

///Averages the points together to get a rough estimate of the center of all the points
Point2f averagePoints(vector<Point2f> points) {
    double xSum = 0;
    double ySum = 0;
    for(Point p : points) {
        xSum += p.x;
        ySum += p.y;
    }
    return Point2f(float(xSum/points.size()), float(ySum/points.size()));
}

Point2f averagePoints(Point2f a, Point2f b) {
    return Point2f(float((a.x+b.x)/2), float((a.y+b.y)/2));
}

///Extracts the points that describe the eyes from a vector of landmarks from the Facemark API
vector<Point2f> getEye(vector<Point2f> points, unsigned short side) {
    vector<Point2f> eye;
    vector<int> sideIndex;

    // 0 = leftSide and 1 = rightSide
    if(side == 0) {
        sideIndex = leftEyeIndex;
    }
    else {
        sideIndex = rightEyeIndex;
    }

    for(unsigned int x = 0; x < points.size(); x++) {
        if(count(sideIndex.begin(),sideIndex.end(), x) != 0) {
            eye.push_back(points[x]);
        }
    }
    return eye;
}

int main(int argc,char** argv)
{
    VideoCapture cam(0);
    FaceExtractor fe("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml", "lbfmodel.yaml");
    Mat frame;
    while(cam.read(frame)) {
        for(Mat face : fe.extractFaces(frame, Size(200,200))) {
            imshow("c", face);
        }

        if (waitKey(1) == 27) break;
    }
    return 0;
}
