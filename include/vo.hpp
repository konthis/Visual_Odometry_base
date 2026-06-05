#pragma once
#include "helper.hpp"
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
namespace fs = std::filesystem;

class VisualOdometry{
    public:
    VisualOdometry() = default;
    int run(const fs::path dataset_path);
};

void feature_detection(cv::Mat img, std::vector<KeyPoint> &feat_points){
    // std::vector<KeyPoint> kps;
    int fast_threshold = 20;
    bool non_max_suppression = true;
    FAST(img, feat_points, fast_threshold, non_max_suppression);
}