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
#include <chrono>

using namespace std;

const cv::String keys {
    "{help h         |          | print this message   }"
    "{index          |0         | Camera index (default = 0)}"
    "{seconds        |2         | Time during which the app will capture frames }"
    "{output         |live.avi  | Time during which the app will capture frames }"
    "{ts timestamp   |          | use time stamp       }"
};

int main(int argc, char **argv)
{
    cv::CommandLineParser cmdParser (argc, argv, keys);
    cmdParser.about("Capture video from webcam");

    if (cmdParser.has("help")) {
        cmdParser.printMessage();
        return EXIT_SUCCESS;
    }

    const int camIndex = cmdParser.get<int>("index");
    const int secondsToRecord  = cmdParser.get<int>("seconds");
    const cv::String outputPath  = cmdParser.get<cv::String>("output");

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
    writer.open(outputPath, codec, fps, frame.size(), isColor);

    if (!writer.isOpened()) {
        cerr << "Could not open the output video file for write\n";
        return -1;
    }

    cout << "recording video during: " << secondsToRecord << " seconds " << endl;
    cout << "Output video: " << outputPath << endl;

    const auto start = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds{0};

    while (elapsedSeconds.count() <= secondsToRecord) {
        if (!capture.read(frame)) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        writer.write(frame);
        elapsedSeconds = std::chrono::system_clock::now()-start;
    }
}
