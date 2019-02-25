#include "FaceExtractor.h"
#define MIN_DETECTION 12

using namespace cv;
using namespace cv::face;

FaceExtractor::FaceExtractor(std::string cascadeClassifierPath, std::string LBFmodelPath)
{
    // Load Face Detector
    faceDetector = CascadeClassifier(cascadeClassifierPath);

    //Grab some information from the FacemarkLBF params so that we can extract the points describing eyes
    FacemarkLBF::Params params = FacemarkLBF::Params();
    for(int i : params.pupils[0]) {
        leftEyeIndex.push_back(i);
    }
    for(int i : params.pupils[1]) {
        rightEyeIndex.push_back(i);
    }

    // Create an instance of Facemark
    facemark = FacemarkLBF::create();

    // Load landmark detector
    facemark->loadModel(LBFmodelPath);
}

FaceExtractor::~FaceExtractor()
{
    //dtor
}

std::vector<Mat> FaceExtractor::extractFaces(Mat frame, Size faceSize) {
      std::vector<Mat> croppedFaces;
      faces.clear();
      detectionCount.clear();

      // Convert frame to grayscale because
      // faceDetector requires grayscale image.
      cvtColor(frame, gray, COLOR_BGR2GRAY);

      // Detect faces
      faceDetector.detectMultiScale(gray, faces, detectionCount);

      //Remove faces if they dont meet the detection count threshold
      bool done = false;
      while(!done) {
        done = true;
        for(unsigned short i = 0; i < faces.size(); i++) {
            if(detectionCount[i] < MIN_DETECTION) {
                done = false;
                faces.erase(faces.begin() + i);
                detectionCount.erase(detectionCount.begin() + i);
                break;
            }
        }
      }

      if(!faces.empty()) {
        landmarks.clear();
        if(facemark->fit(frame,faces,landmarks)) {
          destTriangle[0] = Point2f(faceSize.width * 0.2, faceSize.height * 0.7);
          destTriangle[1] = Point2f(faceSize.width * 0.8, faceSize.height *0.7);
          destTriangle[2] = Point2f(faceSize.width * 0.5, faceSize.height *0.5);

          // If successful, render the landmarks on the face
        for(unsigned int i = 0; i < landmarks.size(); i++)
        {
          std::vector<Point2f> leftEye = getEyePoints(landmarks[i], 0);
          std::vector<Point2f> rightEye = getEyePoints(landmarks[i], 1);
          Point2f leftEyeCenter = FaceExtractor::averagePoints(leftEye);
          Point2f rightEyeCenter = FaceExtractor::averagePoints(rightEye);
          //Point2f rotationCenter = averagePoints(leftEyeCenter, rightEyeCenter);

          //Create a triangle of points for the source end of our affine transformation
          srcTriangle[0] = leftEyeCenter;
          srcTriangle[1] = rightEyeCenter;
          srcTriangle[2] = averagePoints(landmarks[i]);

          warp_matrix = getAffineTransform(srcTriangle, destTriangle);
          warpAffine(frame, cropped, warp_matrix, faceSize);
          flip(cropped, cropped, -1);
          croppedFaces.push_back(cropped);
        }
        }
      }
    return croppedFaces;
}

/// Helper function. Returns a vector of points that outline where the selected eye is.
/// 0 = left eye, 1 = right eye
std::vector<Point2f> FaceExtractor::getEyePoints(std::vector<Point2f> points, unsigned short side) {
    std::vector<Point2f> eye;
    std::vector<int> sideIndex;

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

/// Helper function, averages the points together to get a rough estimate of the center of all the points
Point2f FaceExtractor::averagePoints(std::vector<Point2f> points) {
    double xSum = 0;
    double ySum = 0;
    for(Point p : points) {
        xSum += p.x;
        ySum += p.y;
    }
    return Point2f(float(xSum/points.size()), float(ySum/points.size()));
}

/// Helper function. Averages two points
Point2f FaceExtractor::averagePoints(Point2f a, Point2f b) {
    return Point2f(float((a.x+b.x)/2), float((a.y+b.y)/2));
}
