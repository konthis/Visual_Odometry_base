#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;


// std::vector<std::string> get_filenames(const std::string &folder_path){
//     std::vector<std::string> filenames;
//     for(const auto &f: fs::directory_iterator(folder_path)){
//         filenames.push_back(f.path());
//     }
//     return filenames;
// }

std::vector<fs::path> get_filenames(const fs::path &folder_path){
    std::vector<fs::path> filenames;
    for(const auto &f: fs::directory_iterator(folder_path)){
        filenames.push_back(f.path());
    }
    return filenames;
}

template <typename T>
void print_vector(const std::vector<T> &v){
    for(T e:v){
        std::cout << e << "\n";
    }
}

void load_img(const fs::path &img_path, cv::Mat &img){
    img = cv::imread(img_path, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not open or find the image\n";
    }
}

