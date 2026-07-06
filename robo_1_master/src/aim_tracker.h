#ifndef AIM_TRACKER_H
#define AIM_TRACKER_H

// #include "opencv2/opencv.hpp"

int run(cv::Mat img);

int test();
cv::Point2d getPoint();

std::pair<float, float> get_Prospective(cv::Mat img, cv::Point2d point);

#endif