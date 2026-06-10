#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

cv::Size mat_shape(const cv::Mat &m);
std::vector<fs::path> get_filenames(const fs::path &folder_path);
void load_img(const fs::path &img_path, cv::Mat &img);
void show_img(const std::string win_name, const cv::Mat img);
void show_2_imgs(const std::string win_name, const cv::Mat img1, const cv::Mat img2);
void drawPoints(cv::Mat &img, const std::vector<cv::Point2f> &points);
std::string create_img_postfix(int idx);

template <typename T>
void print_vector(const std::vector<T> &v){
    for(T e:v){
        std::cout << e << "\n";
    }
}

