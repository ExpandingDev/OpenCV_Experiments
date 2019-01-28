#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
    /// Camera Device Number (0 is the first camera)
    int camera_device = 0;

    /// VideoCapture/camera object that we will be grabbing our frames from
    VideoCapture capture;

    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() ) // Check to make sure we were able to open the camera
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

    while(!done) { // Wait until the user presses a key
        capture >> currentFrame; // Grab the current frame
        imshow("Calibrating", currentFrame); // Display it
        done = waitKey(1) != -1; // Set done to true if the user pressed a key
    }

    capture >> referenceFrame; // Grab the reference frame from the camera

    if(referenceFrame.empty()) { // Check to make sure we actually got a frame
        cout << "Error: Could not capture reference frame!" << endl;
        return -1;
    }


    blur(referenceFrame, referenceFrame, Size(2,2)); // Apply a slight blur to help lighing noise

    done = false;
    Mat smallerStructure = getStructuringElement(MORPH_ELLIPSE, Size(4,4));
    Mat structure = getStructuringElement(MORPH_ELLIPSE, Size(10,10));
    Mat largerStructure = getStructuringElement(MORPH_RECT, Size(20,20));
    while(!done) { // Loop until user presses a key

        capture >> currentFrame; // Grab the current frame

        // Check to make sure we got a frame, error out if failed
        if(currentFrame.empty()) {
            cout << "Error: Failed to capture current frame!" << endl;
            done = true;
            return -1;
        }

        blur(currentFrame, currentFrame, Size(2,2)); // Blur the current frame just like we blurred the reference frame
        absdiff(currentFrame, referenceFrame, maskFrame); // Get the absolute difference of each color channel of each pixel compared to the reference frame

        maskFrame = maskFrame.mul(Scalar(255,255,255)); // Amplify the difference by 1

        // Do multiple erosions to get rid of noise
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, smallerStructure);
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, structure);

        // Do a close operation to make sure we didn't cut out the object that is different from the reference frame
        morphologyEx(maskFrame, maskFrame, MORPH_CLOSE, smallerStructure);

        // The next operation, threshold, requires grayscale, so convert the maskFrame to greyscale and store it in greyMask
        cvtColor( maskFrame, greyMask, COLOR_BGR2GRAY );

        // Inverse threshold, if pizel intensity is less than 253, set it to 255. If greater than 253, set to 0
        threshold(greyMask, maskFrame, 253, 255, THRESH_BINARY_INV);

        // Convert maskFrame back to color
        cvtColor( maskFrame, maskFrame, COLOR_GRAY2BGR );

        // Do erosions again after we've used the threshold operation
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, smallerStructure);
        morphologyEx(maskFrame, maskFrame, MORPH_OPEN, structure);
        morphologyEx(maskFrame, maskFrame, MORPH_CLOSE, largerStructure);

        //Subtract the current frame from the inverse mask we've created
        subtract(currentFrame, maskFrame, currentFrame);


        imshow("Output", currentFrame); // Display the output/currentFrame
        done = waitKey(1) != -1; // Loop until user presses a key
    }

    return 0;
}
