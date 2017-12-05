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
    "{index          |0         | Camera index }"
    "{seconds s      |2         | Time during which the app will capture frames }"
    "{output         |live.avi  | Time during which the app will capture frames }"
    "{brightness br  |160.      | Brightness value }"
    "{contrast cr    |42.       | Constrast value }"
    "{exposure ex    |-4.       | Exposure value }"
    "{focus f        |200       | Focus value }"
    "{width          |1280      | Width size in pixels}"
    "{height         |720       | Height size in pixels}"
    "{fps            |15        | FPS for the output video}"
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
    cout << "Initial Frame width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Initial Frame height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;

    capture.set(cv::CAP_PROP_BRIGHTNESS, cmdParser.get<double>("brightness"));
    capture.set(cv::CAP_PROP_CONTRAST, cmdParser.get<double>("contrast"));
    capture.set(cv::CAP_PROP_EXPOSURE, cmdParser.get<double>("exposure"));
    capture.set(cv::CAP_PROP_FOCUS, cmdParser.get<double>("focus"));
    capture.set(cv::CAP_PROP_FRAME_WIDTH, cmdParser.get<double>("width"));
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, cmdParser.get<double>("height"));

    cout << "BRIGHTNESS: " << capture.get(cv::CAP_PROP_BRIGHTNESS) << endl;
    cout << "Constrast: " << capture.get(cv::CAP_PROP_CONTRAST) << endl;
    cout << "Saturation: " << capture.get(cv::CAP_PROP_SATURATION) << endl;
    cout << "Exposure: " << capture.get(cv::CAP_PROP_EXPOSURE) << endl;
    cout << "Focus: " << capture.get(cv::CAP_PROP_FOCUS) << endl;
    cout << "Frame width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Frame height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;

    cv::Mat frame;

    // get one frame from camera to know frame size and type
    capture >> frame;

    if (capture.get(cv::CAP_PROP_FOCUS) == 0) {
        cout << "Focus is 0: It seems that the auto-focus is enabled. Please disable it" << endl;
        return EXIT_FAILURE;
    }

    // check if we succeeded
    if (frame.empty()) {
        cerr << "ERROR! empty frame grabbed\n";
        return EXIT_FAILURE;
    }

    const bool isColor = (frame.type() == CV_8UC3);

    //--- INITIALIZE VIDEOWRITER
    cv::VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    writer.open(outputPath, codec, cmdParser.get<double>("fps"), frame.size(), isColor);

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
