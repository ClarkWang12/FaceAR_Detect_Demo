#ifndef FACEAR_DETECT_HPP
#define FACEAR_DETECT_HPP

#include<vector>
#include"opencv2/opencv.hpp"
using namespace cv;
using namespace std;

class FaceAR_Detect
{

public:
    FaceAR_Detect();

int init(cv::VideoCapture &cap);
int dectectFace(Mat &img,Rect &rect);
vector<cv::Point> dectectLandmark(Mat &img);//use this
vector<cv::Point> dectectLandmark(Mat &image,cv::Rect rect);
void close(cv::VideoCapture &cap);
void draw_Face(Mat &img,Rect rect);
void draw_landmark(Mat &img,vector<cv::Point> landmarks);


};

#endif // FACEAR_DETECT_HPP
