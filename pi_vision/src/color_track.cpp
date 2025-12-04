#include <opencv2/opencv.hpp>
#include <iostream>
//CHECK build folder not upload gihtub important

int main() {

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: CAMEra issue!\n";
        return -1;
    }

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        cv::Mat hsv, mask;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);


// stricter green detection (so no blob lol)
cv::Scalar lower_green(35, 80, 60);
cv::Scalar upper_green(85, 255, 255);

cv::inRange(hsv, lower_green, upper_green, mask);

        // reduction (blobbatron)
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1,-1), 2);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double minArea = 300.0; // fr small objects
        int bestIndex = -1;
        double bestArea = 0;

        for (int i = 0; i < contours.size(); i++) {
            double a = cv::contourArea(contours[i]);
            if (a > bestArea && a > minArea) {
                bestArea = a;
                bestIndex = i;
            }
        }

        if (bestIndex != -1) {
            cv::Rect box = cv::boundingRect(contours[bestIndex]);
            cv::rectangle(frame, box, cv::Scalar(0,255,0), 2);


            int cx = box.x + box.width/2;
            int cy = box.y + box.height/2;
            cv::circle(frame, cv::Point(cx, cy), 5, cv::Scalar(0,0,255), -1);

            std::cout << "Center of obj (detect green color) (" << cx << ", " << cy << ")\n";
        }

        // 2 window - mask + cam w bound box n red dot
        cv::imshow("Camera Feed + Tracking", frame);
        cv::imshow("Green Mask", mask);

        if (cv::waitKey(1) == 'q') break;
    }







    return 0;

}


