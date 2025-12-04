#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);  


    if (!cap.isOpened()) {
        std::cerr << "ERROR: Cannot open camera\n";
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame; 

        if (frame.empty()) {
            std::cerr << "ERROR: Empty frame\n";
            break;
        }

        // red dot center - ONLY INITIAL TEST , TESTING CV WORKING 
        int cx = frame.cols / 2;
        int cy = frame.rows / 2;
        cv::circle(frame, cv::Point(cx, cy), 10, cv::Scalar(0, 0, 255), -1);

        cv::imshow("Live Camera Demo", frame);

        //quit
        if (cv::waitKey(1) == 'q') break;
    }

    return 0;
}
