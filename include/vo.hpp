#pragma once
#include "helper.hpp"
#include <filesystem>
#include <iterator>
#include <iostream>
#include <fstream>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
namespace fs = std::filesystem;

void get_calib_data(fs::path dataset_path, double &focal, Point2d &p);
void feature_detection(Mat img, std::vector<Point2f> &feat_points);
void feature_tracking(Mat img_1, Mat img_2, std::vector<Point2f> &points1, std::vector<Point2f> &points2, std::vector<uchar> &status);

class VisualOdometry{
    public:
    VisualOdometry() = default;
    int run(const fs::path dataset_path);
};


