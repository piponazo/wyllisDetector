#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Motion detection code taken from :
//  - http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/

using namespace std;

void writeVideo(cv::VideoCapture &cap);

int main(int argc, char **argv)
{
    cout << "Let's detect Wyllis!" << endl;

    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout  << "Could not open camera" << endl;
        return EXIT_FAILURE;
    }

    cv::Mat frame, gray, blur, previousBlur, diff, thres;
    cap >> frame; // get a new frame from camera

    const int totalPixels = frame.size().area();
    cout  << "Total pixels: " << totalPixels << endl;
    cout  << "Camera dimensions: " << frame.cols << " x " << frame.rows << endl;

    cv::namedWindow("image",1);
    cv::namedWindow("gray",1);
    cv::namedWindow("blur",1);
    cv::namedWindow("thres",1);

    cv::moveWindow("gray", 500, 0);
    cv::moveWindow("blur", 0, 500);
    cv::moveWindow("thres", 500, 500);

    // Read until we detect movement
    for(;;)
    {
        cap >> frame; // get a new frame from camera

        cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
        cv::GaussianBlur(gray, blur, {21, 21}, 0);

        if (previousBlur.empty())
            previousBlur = blur.clone();

        cv::absdiff(previousBlur, blur, diff);
        cv::threshold(diff, thres, 25, 255, cv::THRESH_BINARY);

        int whitePixels = cv::countNonZero(thres);

        /// \todo change this value
        if (whitePixels > 2000)
        {
            writeVideo(cap);
            return EXIT_SUCCESS;
        }

        cv::imshow("image", frame);
        cv::imshow("gray", gray);
        cv::imshow("blur", blur);
        cv::imshow("thres", thres);

        if(cv::waitKey(30) >= 0)
            break;
    }


    return EXIT_FAILURE;
}

void writeVideo(cv::VideoCapture &cap)
{
    const string NAME = "/tmp/video.avi";   // Form the new name with container
    int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

    cv::Size S = cv::Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                 (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

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


}
