#include <opencv2/opencv.hpp>
#include <iostream>

//ENSURE build is in gitignore check
int main() {

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Camera issue!\n";
        return -1;
    }

    int mode = 2;  // default mode = tracking
    int sweepPos = 0;
    int sweepDir = 1;

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        cv::Mat display = frame.clone();

        char key = cv::waitKey(1);

/*chat code for mode switching 
        // ---- MODE SWITCHING ----
        if (key == '1') mode = 1;   // idle
        if (key == '2') mode = 2;   // tracking
        if (key == '3') mode = 3;   // patrol
        if (key == 'q') break;

        // --------------------------------------------------
        // MODE 1: IDLE (just show live feed, nothing else)
        // --------------------------------------------------
        if (mode == 1) {
            cv::putText(display, "MODE: IDLE",
                        cv::Point(20,40),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0,
                        cv::Scalar(255,255,255), 2);

            cv::imshow("Camera Feed + Tracking", display);
            continue;
        }
*/
        // MODE 2: COLOR TRACKING 
        if (mode == 2) {

            cv::Mat hsv, mask;
            cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

            //strict green - 2 much bad input
            // Expanded green detection , darker and brither
            cv::Scalar lower_green(30, 40, 20);
            cv::Scalar upper_green(90, 255, 255);
            cv::inRange(hsv, lower_green, upper_green, mask);

            cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
            cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            double minArea = 300.0;
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
                cv::rectangle(display, box, cv::Scalar(0, 255, 0), 2);

                int cx = box.x + box.width / 2;
                int cy = box.y + box.height / 2;

                // crosshair no dot
                int size = 20;

                // Horizontal for crosshair
                cv::line(display,
                         cv::Point(cx - size, cy),
                         cv::Point(cx + size, cy),
                         cv::Scalar(0, 0, 255),
                         2);

                // Vertical line for crosshair
                cv::line(display,
                         cv::Point(cx, cy - size),
                         cv::Point(cx, cy + size),
                         cv::Scalar(0, 0, 255),
                         2);

                std::cout << "Center of object (green): (" << cx << ", " << cy << ")\n";

                // Create a blank img for minimap 
                cv::Mat mini = cv::Mat::zeros(150, 150, CV_8UC3);

                cv::rectangle(mini,
                              cv::Point(0, 0),
                              cv::Point(mini.cols - 1, mini.rows - 1),
                              cv::Scalar(0, 255, 0),
                              2);

                //position into mini map coordinates
                int miniX = (cx * mini.cols) / frame.cols;
                int miniY = (cy * mini.rows) / frame.rows;

                cv::circle(mini,
                           cv::Point(miniX, miniY),
                           5,
                           cv::Scalar(0, 0, 255),
                           -1);

                //minimap , SEPARATE window
                cv::imshow("Mini Map", mini);
            }

            cv::putText(display, "MODE: COLOR TRACK",
                        cv::Point(20,40),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0,
                        cv::Scalar(0,255,0), 2);

            cv::imshow("Camera Feed + Tracking", display);
            cv::imshow("Green Mask", mask);

            continue;
        }

/*
       // --------------------------------------------------
// MODE 3: PATROL — REAL SWEEP ANIMATION
// --------------------------------------------------
if (mode == 3) {

    cv::putText(display, "MODE: PATROL SWEEP",
                cv::Point(20,40),
                cv::FONT_HERSHEY_SIMPLEX, 1.0,
                cv::Scalar(255,0,0), 2);

    // smooth scan line moving horizontally
    sweepPos += sweepDir * 5; // speed

    if (sweepPos >= display.cols - 1) sweepDir = -1;
    if (sweepPos <= 0) sweepDir = 1;

    // draw vertical sweep line
    cv::line(display,
             cv::Point(sweepPos, 0),
             cv::Point(sweepPos, display.rows),
             cv::Scalar(255, 0, 0),
             2);

    // OPTIONAL: draw fake radar cone (FOV shape)
    int centerY = display.rows / 2;
    int coneWidth = 200;

    cv::line(display,
             cv::Point(sweepPos, centerY),
             cv::Point(sweepPos + coneWidth * sweepDir, centerY - 150),
             cv::Scalar(255, 0, 0), 2);

    cv::line(display,
             cv::Point(sweepPos, centerY),
             cv::Point(sweepPos + coneWidth * sweepDir, centerY + 150),
             cv::Scalar(255, 0, 0), 2);

    cv::imshow("Camera Feed + Tracking", display);
    continue;
} */

    }

    return 0;
}
