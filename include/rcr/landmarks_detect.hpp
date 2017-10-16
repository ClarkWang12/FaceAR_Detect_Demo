#pragma once

#ifndef LANDMARK_DETECT_HPP_
#define LANDMARK_DETECT_HPP_
#include "landmark.hpp"
#include"rcr/pre_frame_params.hpp"
#include"rcr/model.hpp"
#include"rcr/landmarks_stabilize.hpp"
#include "rcr/face_detect.hpp"
#include "rcr/camshift_tfy.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
using namespace std;
using namespace cv;
namespace rcr {

///TODO:TFY-----landmarks_detect
bool landmarks_detect(cv::Mat &image,Pre_frame_params &pre_frame,rcr::detection_model rcr_model,rcr::LandmarkCollection<cv::Vec2f> &current_landmarks,cv::CascadeClassifier face_cascade)
{
    int isDetectSucess=face_detect(image,pre_frame,face_cascade,rcr_model);
    ///TODO:TFY---landmarks detect,cam_success->2,success->1,failed->0,cam_lose->-1
    using namespace std::chrono;
    time_point<system_clock> start, end;
    start = system_clock::now();
    if(isDetectSucess==1)
    {
        current_landmarks = rcr_model.detect(image, pre_frame.pre_rect);
        cout<<"rect:"<<pre_frame.pre_rect.x<<","<<pre_frame.pre_rect.y<<","<<pre_frame.pre_rect.size()<<endl;
        cv::Rect bounding_box;
        bounding_box=get_bounding_box(current_landmarks);
        rcr::areaLimit(bounding_box,image);
        pre_frame.pre_hist= hist_make(image,bounding_box);
        //cv::rectangle( image, bounding_box, Scalar( 0, 255, 255 ), 2, 1 );
        cout<<"bounding_box:"<<bounding_box.x<<","<<bounding_box.y<<","<<bounding_box.size()<<endl;
    }
    else if(isDetectSucess==2)//rot
    {
        Mat rm(2, 3, CV_64F);//after re affine
        getRotMat(cv::Point(image.cols/2,image.rows/2),-1*pre_frame.angle,rm);
        current_landmarks = rcr_model.detect(pre_frame.pre_img_affine, pre_frame.pre_rect_affine);
        pointAffine(current_landmarks,rm);

        cv::Rect bounding_box;
        bounding_box=get_bounding_box(current_landmarks);
        rcr::areaLimit(bounding_box,image);
        pre_frame.pre_hist= hist_make(image,bounding_box);
    }
    else if(isDetectSucess==0)
    {
        pre_frame.detected=false;
        std::cout<<"not found"<<endl;
        return 0;
    }
    else//-1
    {
//        cv::Mat initialization=pre_frame.mean.clone();
//        int l=pre_frame.mean.cols/2;
//        cout<<"pre1:"<<pre_frame.pre_landmarks[0].coordinates[0]<<endl;
//        cout<<"l:"<<l<<endl;
//        for(int i=0;i<l;i++)
//        {
//            initialization.at<float>(0,i)=pre_frame.pre_landmarks[i].coordinates[0];
//            initialization.at<float>(0,i+l)=pre_frame.pre_landmarks[i].coordinates[1];
//        }
//        cout<<"in1:"<<initialization.at<float>(0,0)<<endl;



        //pre_frame.pre_rect=box;

        Mat rm(2, 3, CV_64F);
        getRotMat(cv::Point(image.cols/2,image.rows/2),-1*pre_frame.angle,rm);

        current_landmarks = rcr_model.detect(pre_frame.pre_img_affine,pre_frame.pre_rect_affine);
        imshow("image_aff",pre_frame.pre_img_affine);
        pointAffine(current_landmarks,rm);
        std::cout<<"-1"<<endl;
        //return 0;
    }

    ///TODO:TFY--stable the points
    if(pre_frame.detected)
    {
        current_landmarks=landmarks_stabilize(current_landmarks,pre_frame);
    }
    pre_frame.pre_landmarks=current_landmarks;
    pre_frame.detected=true;
    end = system_clock::now();
    auto t_fit = duration_cast<milliseconds>(end - start).count();
    cout << "landmarkDetect: " << t_fit << endl;
    return 1;
};



}
#endif /* LANDMARKS_IO_HPP_ */
