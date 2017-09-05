#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>

// Motion detection code taken from :
//  - http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/

using namespace std;

string writeVideo(cv::VideoCapture &cap);
bool   uploadVideo(const std::string &videoPath);
bool   checkTime();
void   sleepSoftware();

int main(int argc, char **argv)
{
    cout << "Let's detect Wyllis!" << endl;

    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout  << "Could not open camera" << endl;
        return EXIT_FAILURE;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cout << "Image size set correctly\n";

    cv::Mat frame, gray, blur, previousBlur, diff, thres;
    cap >> frame; // get a new frame from camera

    const int totalPixels = frame.size().area();
    cout  << "Total pixels: " << totalPixels << endl;
    cout  << "Camera dimensions: " << frame.cols << " x " << frame.rows << endl;

//    cv::namedWindow("image",1);
//    cv::namedWindow("gray",1);
//    cv::namedWindow("blur",1);
//    cv::namedWindow("thres",1);

//    cv::moveWindow("gray", 500, 0);
//    cv::moveWindow("blur", 0, 500);
//    cv::moveWindow("thres", 500, 500);

    // Read until we detect movement
    int initFrames = 0;
    for(;;)
    {
        if (!checkTime())
        {
            sleepSoftware();
            continue;
        }

        cap >> frame; // get a new frame from camera

        cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
        cv::GaussianBlur(gray, blur, {21, 21}, 0);

        if (initFrames == 0 || initFrames == 1000)
        {
            std::cout << "Background image updated!\n";
            previousBlur = blur.clone();
        }

        cv::absdiff(previousBlur, blur, diff);
        cv::threshold(diff, thres, 25, 255, cv::THRESH_BINARY);

        int whitePixels = cv::countNonZero(thres);

        /// \todo change this value
        if (whitePixels > 2000)
        {
            std::cout << "Movement detected. Starting to write video!\n";
            const string videoPath = writeVideo(cap);
            uploadVideo(videoPath);
            std::remove(videoPath.c_str());
            initFrames = 0;
            previousBlur.release();
        }
        else
        {
            initFrames++;
        }

//        cv::imshow("image", frame);
//        cv::imshow("gray", gray);
//        cv::imshow("blur", blur);
//        cv::imshow("thres", thres);
//        if(cv::waitKey(30) >= 0)
//            break;
    }


    return EXIT_FAILURE;
}

string writeVideo(cv::VideoCapture &cap)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");


//    const string NAME = "/tmp/video" + oss.str() + ".avi";   // Form the new name with container
    const string NAME = "/tmp/video" + oss.str() + ".mp4";   // Form the new name with container
    std::cout << "Writing file at " << NAME << endl;

//    int ex = CV_FOURCC('M','J','P','G');
//    int ex = CV_FOURCC('3','I','V','D');
//    int ex = CV_FOURCC('D','I','V','4');
//    int ex = CV_FOURCC('F','M','P','4');
//    int ex = CV_FOURCC('M','P','4','2'); // no
    int ex = CV_FOURCC('X','2','6','4'); // no
//    int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

    cv::Size S = cv::Size(640, 480);

    cv::VideoWriter outputVideo;                                        // Open the output
    outputVideo.open(NAME, ex, cap.get(CV_CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
       cout  << "Could not open the output video for write" << endl;
       exit(EXIT_FAILURE);
    }

    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
        << " of nr#: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << EXT << endl;

    cv::Mat src;

    /// \todo use another approach for the video duration
    for(int i = 0; i < 200; i++)
    {
        cap >> src;
        outputVideo << src;
    }

    return NAME;
}

bool   uploadVideo(const std::string &videoPath)
{
    cout << "Uploading video to ftp server ... ";
    string lftp("lftp sftp://user:pass@server -e ");
    string ftpCommand1("\"cd files/wyllis; put ");
    string ftpCommand2("; bye\"");
    string command = lftp + ftpCommand1 + videoPath + ftpCommand2;
    system(command.c_str());
    cout << "video uploaded... ";
}

bool   checkTime()
{
    auto now = std::time(nullptr);
    std::tm tmNow = *std::localtime(&now);

    std::tm tmInit = *std::localtime(&now);
    std::tm tmEnd = *std::localtime(&now);

    tmInit.tm_hour = 8;
    tmInit.tm_min = 20;
    tmInit.tm_sec = 0;

    tmEnd.tm_hour = 20;
    tmEnd.tm_min = 20;
    tmEnd.tm_sec = 0;

    std::ostringstream oss;
//    std::cout << "Now: " << std::put_time(&tmNow, "%d-%m-%Y_%H-%M-%S") << endl;
//    std::cout << "Init: " << std::put_time(&tmInit, "%d-%m-%Y_%H-%M-%S") << endl;
//    std::cout << "End: " << std::put_time(&tmEnd, "%d-%m-%Y_%H-%M-%S") << endl << endl;

    auto seconds1 = difftime(mktime(&tmNow), mktime(&tmInit));
    auto seconds2 = difftime(mktime(&tmNow), mktime(&tmEnd));

//    std::cout << "seconds1: " << seconds1 << " seconds2: " << seconds2 << endl;

    if (seconds1 > 0 && seconds2 < 0)
    {
        return true;
    }
    return false;
}

void   sleepSoftware()
{
    std::cout << "Putting software to sleep for 1 minutes\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::minutes(1));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
}
