#include "helper.hpp"

cv::Size mat_shape(const cv::Mat &m){
    return m.size();
}

std::vector<fs::path> get_filenames(const fs::path &folder_path){
    std::vector<fs::path> filenames;
    for(const auto &f: fs::directory_iterator(folder_path)){
        filenames.push_back(f.path());
    }
    return filenames;
}

void load_img(const fs::path &img_path, cv::Mat &img){
    img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cout << "Could not open or find the image\n";
    }
}


void drawPoints(cv::Mat &img, const std::vector<cv::Point2f> &points){
    for (const auto &p : points)
        cv::circle(img, p, 3, cv::Scalar(10, 10, 10), -1);
}

std::string create_img_postfix(int idx){
    std::ostringstream ss;
    ss << std::setw(6) << std::setfill('0') << idx << ".png";
    return ss.str();
}

void show_img(const std::string win_name, const cv::Mat img){
    cv::imshow(win_name,img);
    if(cv::waitKey(0)==27){ exit(0);}
}

void show_2_imgs(const std::string win_name, const cv::Mat img1, const cv::Mat img2){
    cv::Mat both; cv::vconcat(img1, img2, both);
    cv::imshow(win_name,both);
    if(cv::waitKey(0)==27){ exit(0);}
}