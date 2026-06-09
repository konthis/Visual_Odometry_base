#include "helper.hpp"

std::vector<fs::path> get_filenames(const fs::path &folder_path){
    std::vector<fs::path> filenames;
    for(const auto &f: fs::directory_iterator(folder_path)){
        filenames.push_back(f.path());
    }
    return filenames;
}

void load_img(const fs::path &img_path, cv::Mat &img){
    img = cv::imread(img_path, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Could not open or find the image\n";
    }
}


void drawPoints(cv::Mat &img, const std::vector<cv::Point2f> &points){
    for (const auto &p : points)
        cv::circle(img, p, 3, cv::Scalar(255, 100, 100), -1);
}