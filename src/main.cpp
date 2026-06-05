#include "helper.hpp"
#include "vo.hpp"
#include <filesystem>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

#define SOURCE_DATASET_FOLDER "/mnt/e/Datasets/KITTI/dataset/sequences/"

void print_mat(const cv::Mat &m){
    std::cout << "m = " << "\n" << m << "\n";
}

int test_img_preview(){
    std::string sequence    = "00";
    std::string imagen      = "image_0";

    
    std::vector<fs::path> fnames = get_filenames(fs::path((std::string)SOURCE_DATASET_FOLDER + sequence +"/"+ imagen));
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);

    
    for(auto f:fnames){
        cv::Mat image = cv::imread(f, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "Could not open or find the image\n";
            return -1;
        }
        cv::imshow("Image", image);
        if(cv::waitKey(0)==27) {return 0;}
    }
    return 0;
}


int test_feat_detect(){
    fs::path img_path = (std::string)SOURCE_DATASET_FOLDER + "00/image_0/000000.png";
    cv::Mat img;
    load_img(img_path,img);
    // cv::imshow("SKRT",img);
    // if(cv::waitKey(0)==27) {return 0;}

    std::vector<KeyPoint> feat_points;
    feature_detection(img,feat_points);
    cv::Mat img_keypoints;
    drawKeypoints(img, feat_points, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    cv::Mat both; cv::vconcat(img, img_keypoints,both);
 
    cv::imshow("SKRT",both);

    if(cv::waitKey(0)==27) {return 0;}
    

    return 0;
}

int main(int argc, char** argv){

    return test_feat_detect();
}