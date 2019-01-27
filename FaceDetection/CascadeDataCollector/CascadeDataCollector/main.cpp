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

    namedWindow( "Preview", WINDOW_AUTOSIZE ); // Create a window named "test" to display the video

    bool done = false;
    while(done != true) {
        captureSource >> frame; // Grab a frame from the capture device and store it in the frame variable
        imshow("Preview", frame); // Show the current frame on the window
        int c = waitKey(1);

        if(c != -1) { // If the user presses a key
            if(c == 27) {
                done = true;
            }
        }
    }

    return 0;
}
