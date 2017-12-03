/* Linux
 * -----
 *
 * To list the available cameras you can type:
 * v4l2-ctl --list-devices
 *
 * It needs the package v4l-utils.
 */

#include <opencv2/opencv.hpp>

//#include <opencv2/videoio.hpp>
//#include <opencv2/core/mat.hpp>

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    cout << "App start!" << endl;

    int camIndex = 0;
    if (argc == 2) {
        camIndex = std::atoi(argv[1]);
    }

    cout << "camera index: " << camIndex << endl;

    cv::VideoCapture capture(camIndex);

    if(!capture.isOpened()) {
        cout  << "Could not open camera" << endl;
        return EXIT_FAILURE;
    }

    cout << "Camera connected correctly" << endl;

    cv::Mat frame;

    // get one frame from camera to know frame size and type
    capture >> frame;

    // check if we succeeded
    if (frame.empty()) {
        cerr << "ERROR! empty frame grabbed\n";
        return EXIT_FAILURE;
    }

    const bool isColor = (frame.type() == CV_8UC3);

    //--- INITIALIZE VIDEOWRITER
    cv::VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = 25.0;                          // framerate of the created video stream
    string filename = "./live.avi";             // name of the output video file
    writer.open(filename, codec, fps, frame.size(), isColor);

    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return -1;
    }
    
    //--- GRAB AND WRITE LOOP
    cout << "Writing videofile: " << filename << endl
         << "Press any key to terminate" << endl;

    for (;;) {
        if (!capture.read(frame)) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        writer.write(frame);

        // show live and wait for a key with timeout long enough to show images
        // imshow("Live", frame);
        if (cv::waitKey(5) >= 0)
            break;
    }
}
