#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat frame; // Matrix to store our current frame (image) in
    VideoCapture captureSource(0); // Get the image source. Passing 0 means accessing the first camera device

    if(!captureSource.isOpened()) { // Check to make sure the capture device was opened
        cout << "ERROR: Failed to open capture device!" << endl;
        return -1;
    }

    namedWindow( "test", WINDOW_AUTOSIZE ); // Create a window named "test" to display the video

    bool done = false;
    while(done != true) {
        captureSource >> frame; // Grab a frame from the capture device and store it in the frame variable
        imshow("test", frame); // Show the current frame on the window
        done = waitKey(1) != -1; // If the user presses a key, exit the loop
    }

    return 0;
}



