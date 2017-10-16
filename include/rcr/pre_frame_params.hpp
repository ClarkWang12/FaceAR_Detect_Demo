#pragma once

#ifndef PRE_FRAME_PARAMS_HPP_
#define PRE_FRAME_PARAMS_HPP_

#include<opencv2/opencv.hpp>
#include "rcr/landmarks_io.hpp"
using namespace std;

struct Pre_frame_params{
    bool detected=false;

    cv::Rect pre_rect;
    rcr::LandmarkCollection<cv::Vec2f> pre_landmarks;
    //cv::Mat mean;

    ///affine:
    cv::Mat pre_hist;
    bool isRot=false;
    float angle;
    cv::Rect pre_rect_affine;
    cv::Mat pre_img_affine;
};


#endif
