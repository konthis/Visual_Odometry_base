#include "vo.hpp"

#define MAX_FRAME 1000
#define MIN_NUM_FEAT 2000

void get_calib_data(const fs::path calib_path, double &focal, Point2d &p){
    // fs::path calib_path = dataset_path;
    // calib_path += "/calib.txt";
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

void feature_tracking(Mat img1, Mat img2, std::vector<Point2f> &points1, std::vector<Point2f> &points2, std::vector<uchar> &status)
{

    //copy
    std::vector<float> err;
    Size window_size = Size(21, 21);
    TermCriteria term_criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
    calcOpticalFlowPyrLK(img1, img2, points1, points2, status, err, window_size, 3, term_criteria, 0, 0.001);

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

double getAbsoluteScale(const fs::path pose_path, int frameIdx)
{
    // copied
    std::string line;
    int i = 0;
    std::ifstream myfile(pose_path);
    double x = 0, y = 0, z = 0;
    double x_prev, y_prev, z_prev;
    if (myfile.is_open())
    {
        while ((getline(myfile, line)) && (i <= frameIdx))
        {
            z_prev = z;
            x_prev = x;
            y_prev = y;
            std::istringstream in(line);
            for (int j = 0; j < 12; j++)
            {
                in >> z;
                if (j == 7)
                  y = z;
                if (j == 3)
                  x = z;
            }

            i++;
        }
        myfile.close();
    }

    else
    {
        std::cout << "Unable to open file";
        return 0;
    }

    return sqrt((x - x_prev) * (x - x_prev) + (y - y_prev) * (y - y_prev) + (z - z_prev) * (z - z_prev));
}

int VisualOdometry::run(const fs::path dataset_path, const std::string sequence){
  
    Mat img1, img2;
    double scale = 1.0;
    fs::path img_path = dataset_path / "sequences" / sequence / "image_0";
    fs::path calib_data_path = dataset_path / "sequences" / sequence / "calib.txt";
    fs::path poses_path = dataset_path / "poses" / "00.txt";
    std::string img_postfix = create_img_postfix(0);
    load_img(img_path / create_img_postfix(0), img1);
    load_img(img_path / create_img_postfix(1), img2);

    std::vector<Point2f> points1, points2; 
    feature_detection(img1, points1); 
    std::vector<uchar> status;
    feature_tracking(img1, img2, points1, points2, status);

    double focal;
    cv::Point2d pp;
    get_calib_data(calib_data_path, focal, pp);

    Mat E, R, t, mask;
    E = findEssentialMat(points2, points1, focal, pp, RANSAC, 0.999, 1.0, mask);
    recoverPose(E, points2, points1, R, t, focal, pp, mask);

    Mat prev_image = img2;
    Mat curr_image;
    std::vector<Point2f> prev_features = points2;
    std::vector<Point2f> curr_features;

    Mat R_f, t_f;
    R_f = R.clone();
    t_f = t.clone();

    clock_t begin = clock();

    // namedWindow("Road facing Egomotion camera", WINDOW_AUTOSIZE); 
    // namedWindow("Trajectory", WINDOW_AUTOSIZE);                   
    Mat traj = Mat::zeros(600, 600, CV_8UC3);

    char text[100];
    int font_face = FONT_HERSHEY_PLAIN;
    double font_scale = 1;
    int thickness = 1;
    cv::Point text_org(10, 50);


    for (int num_frame = 2; num_frame < MAX_FRAME; num_frame++){
        load_img(img_path / create_img_postfix(num_frame), curr_image);
        feature_tracking(prev_image, curr_image, prev_features, curr_features, status);
        E = findEssentialMat(curr_features, prev_features, focal, pp, RANSAC, 0.999, 1.0, mask);
        recoverPose(E, curr_features, prev_features, R, t, focal, pp, mask);


        scale = getAbsoluteScale(poses_path, num_frame);
        if ((scale > 0.1) && (t.at<double>(2) > t.at<double>(0)) && (t.at<double>(2) > t.at<double>(1)))
        {

          t_f = t_f + scale * (R_f * t); // t_final = t_previous + scale * (R_previous * t_current)
          R_f = R * R_f;                 // R_final = R_current * R_previous
        }

        if (prev_features.size() < MIN_NUM_FEAT)
        {
          feature_detection(prev_image, prev_features);
          feature_tracking(prev_image, curr_image, prev_features, curr_features, status);
        }
        prev_image = curr_image.clone();
        prev_features = curr_features;

        cv::Mat img_feat= curr_image;
        drawPoints(img_feat, curr_features);
        int x = int(t_f.at<double>(0)) + 300;      // offset for easier visualisation
        int y = int(-1 * t_f.at<double>(2)) + 500; // -1 inversion and offset for easier visualisation
        circle(traj, Point(x, y), 1, CV_RGB(255, 0, 0), 2);

        rectangle(traj, Point(10, 30), Point(550, 50), CV_RGB(0, 0, 0), cv::FILLED);
        sprintf(text, "Coordinates: x = %02fm y = %02fm z = %02fm", t_f.at<double>(0), t_f.at<double>(1), t_f.at<double>(2));
        putText(traj, text, text_org, font_face, font_scale, Scalar::all(255), thickness, 8);

        show_img("",img_feat);
        show_img(" ",traj);
    }

}

