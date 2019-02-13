#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay( Mat frame );
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main( int argc, const char** argv )
{
    String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "/usr/local/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if( !eyes_cascade.load( eyes_cascade_name ) )
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };


    int camera_device = 0;
    VideoCapture capture;

    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }

    Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}

void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    Mat frame_blur;
    Mat frame_detect;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );

    equalizeHist( frame_gray, frame_gray );
    blur(frame, frame_blur, Size(2,2), Point(-1,-1));

    //-- Detect faces
    std::vector<Rect> faces;
    std::vector<int> detectionCount;
    face_cascade.detectMultiScale( frame_blur, faces, detectionCount );


    for ( size_t i = 0; i < faces.size(); i++ ) {
        rectangle(frame_blur, faces[i], Scalar(0, 255, 0, 0), 8, LINE_8, 0);
        putText(frame_blur, std::to_string(detectionCount[i]), Point(faces[i].x, faces[i].y), HersheyFonts::FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255), 5);
    }

    //-- Show what you got
    //imshow( "Grey scale original", frame_gray );
    imshow( "Detecting", frame_blur );
}
