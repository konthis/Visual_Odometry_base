#include "vo.hpp"

void get_calib_data(fs::path dataset_path, double &focal, Point2d &p){
    fs::path calib_path = dataset_path;
    calib_path += "calib.txt";
    std::ifstream file(calib_path);
    if(file.is_open()){
        std::string line;
        while(getline(file,line)){
            std::istringstream iss(line);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
            focal = std::stod(results[1]);
            p.x = std::stod(results[3]);
            p.y = std::stod(results[7]);
            break;
        }
        file.close();
    }
}

void feature_detection(Mat img, std::vector<Point2f> &feat_points){
    std::vector<KeyPoint> keypoints;
    int fast_threshold = 20;
    bool non_max_suppression = true;
    FAST(img, keypoints, fast_threshold, non_max_suppression);
    KeyPoint::convert(keypoints, feat_points, std::vector<int>());
}

void feature_tracking(Mat img_1, Mat img_2, std::vector<Point2f> &points1, std::vector<Point2f> &points2, std::vector<uchar> &status)
{

  //copy
  std::vector<float> err;
  Size window_size = Size(21, 21);
  TermCriteria term_criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
  calcOpticalFlowPyrLK(img_1, img_2, points1, points2, status, err, window_size, 3, term_criteria, 0, 0.001);

  int index_correction = 0;
  for (int i = 0; i < status.size(); i++)
  {
    Point2f pt = points2.at(i - index_correction);
    if ((status.at(i) == 0) || (pt.x < 0) || (pt.y < 0))
    {
      if ((pt.x < 0) || (pt.y < 0))
      {
        status.at(i) = 0;
      }
      points1.erase(points1.begin() + (i - index_correction));
      points2.erase(points2.begin() + (i - index_correction));
      index_correction++;
    }
  }
}


int VisualOdometry::run(const fs::path dataset_path){
    cv::Mat img1, img2;

    std::vector<Point2f> points1,points2;
    feature_detection(img1,points1);
    std::vector<uchar> status;
    feature_tracking(img1,img2,points1,points2,status);
    
}

