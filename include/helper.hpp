#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

std::vector<fs::path> get_filenames(const fs::path &folder_path);
void load_img(const fs::path &img_path, cv::Mat &img);
void drawPoints(cv::Mat &img, const std::vector<cv::Point2f> &points);

template <typename T>
void print_vector(const std::vector<T> &v){
    for(T e:v){
        std::cout << e << "\n";
    }
}

