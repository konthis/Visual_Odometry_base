#include "helper.hpp"
#include "vo.hpp"
#include <filesystem>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

#define SOURCE_DATASET_FOLDER "/mnt/e/Datasets/KITTI/dataset/"

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

    std::vector<Point2f> feat_points;
    feature_detection(img,feat_points);
    cv::Mat img_keypoints = img;
    // drawKeypoints(img, feat_points, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    drawPoints(img_keypoints,feat_points);


    // cv::Mat both; cv::vconcat(img, img_keypoints,both);
 
    cv::imshow("SKRT",img_keypoints);

    if(cv::waitKey(0)==27) {return 0;}
    

    return 0;
}

int test_calib(){
    fs::path calib_p = (std::string)SOURCE_DATASET_FOLDER + "00/";
    cv::Mat img;
    Point2d p;
    double focal;
    get_calib_data(calib_p,focal,p);
    return 0;
}

int test_track(){
    fs::path img_path = (std::string)SOURCE_DATASET_FOLDER + "00/image_0/000007.png";
    cv::Mat img1, img2;
    load_img(img_path,img1);
    img_path = (std::string)SOURCE_DATASET_FOLDER + "00/image_0/000008.png";
    load_img(img_path,img2);

    std::vector<Point2f> points1,points2;
    feature_detection(img1,points1);
    std::vector<uchar> status;
    feature_tracking(img1,img2,points1,points2,status);

    cv::Mat img_keypoints1 = img1,img_keypoints2=img2;
    drawPoints(img_keypoints1, points1);
    drawPoints(img_keypoints2, points2);


    cv::Mat both; cv::vconcat(img_keypoints1, img_keypoints2,both);
 
    cv::imshow("SKRT",both);

    std::cout << points1.size() << "\n";
    std::cout << points2.size() << "\n";
    if(cv::waitKey(0)==27) {return 0;}
}

int main(int argc, char** argv){
    std::string sequence    = "00";
    fs::path dataset_path = SOURCE_DATASET_FOLDER;

    VisualOdometry vo;
    vo.run(dataset_path, sequence);
    return 0;
}