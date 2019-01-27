#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{


    int camera_device = 0;
    VideoCapture capture;

    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }

    Mat referenceFrame;
    Mat currentFrame;
    Mat maskFrame;
    Mat absoluteMat;
    Mat greyMask;
    Scalar white (255,255,255);
    //MatSize s = referenceFrame.size;
    Mat whiteMat (referenceFrame.rows, referenceFrame.cols, referenceFrame.type(), white);
    bool done = false;
    cout << "Please leave the scene and press a key to calibrate" << endl;

    while(!done) {
        capture >> currentFrame;
        imshow("Calibrating", currentFrame);
        done = waitKey(1) != -1;
    }

    capture >> referenceFrame;

    if(referenceFrame.empty()) {
        cout << "Error: Could not capture reference frame!" << endl;
        return -1;
    }
    blur(referenceFrame, referenceFrame, Size(2,2));

    done = false;
    Mat smallerStructure = getStructuringElement(MORPH_ELLIPSE, Size(4,4));
    Mat structure = getStructuringElement(MORPH_ELLIPSE, Size(10,10));
    Mat largerStructure = getStructuringElement(MORPH_RECT, Size(20,20));
    while(!done) {
        capture >> currentFrame;
        if(currentFrame.empty()) {
            cout << "Error: Failed to capture current frame!" << endl;
            done = true;
            return -1;
        }
        //equalizeHist(currentFrame, currentFrame);
        blur(currentFrame, currentFrame, Size(2,2));
        absdiff(currentFrame, referenceFrame, maskFrame);


        maskFrame = maskFrame.mul(Scalar(255,255,255));
        //morphologyEx(maskFrame, maskFrame, MORPH_OPEN, smallerStructure);
        //morphologyEx(maskFrame, maskFrame, MORPH_OPEN, structure);
        //morphologyEx(maskFrame, maskFrame, MORPH_CLOSE, smallerStructure);

        cvtColor( maskFrame, greyMask, COLOR_BGR2GRAY );
        threshold(greyMask, maskFrame, 253, 255, THRESH_BINARY_INV);
        cvtColor( maskFrame, maskFrame, COLOR_GRAY2BGR );
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, smallerStructure);
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, structure);
        morphologyEx(maskFrame, maskFrame, MORPH_CLOSE, largerStructure);
        //maskFrame = maskFrame.inv();
        subtract(currentFrame, maskFrame, currentFrame);

        //currentFrame = currentFrame.mul(maskFrame);

        imshow("Output", currentFrame);
        done = waitKey(1) != -1;
    }

    return 0;
}
