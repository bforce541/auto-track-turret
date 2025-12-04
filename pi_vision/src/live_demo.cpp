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

        int cx = frame.cols / 2;
        int cy = frame.rows / 2;

        // crosshair
        int size = 20; 

        cv::line(frame,
                 cv::Point(cx - size, cy),
                 cv::Point(cx + size, cy),
                 cv::Scalar(0, 0, 255),
                 2);
                 
                 cv::line(frame,
                 cv::Point(cx, cy - size),
                 cv::Point(cx, cy + size),
                 cv::Scalar(0, 0, 255),
                 2);

        cv::imshow("Live Camera Demo", frame);

        if (cv::waitKey(1) == 'q') break;
    }

    return 0;
}
