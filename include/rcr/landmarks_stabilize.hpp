#pragma once

#ifndef LANDMARKS_STABILIZE_HPP_
#define LANDMARKS_STABILIZE_HPP_


#include <opencv2/opencv.hpp>
#include "rcr/landmarks_io.hpp"
#include "pre_frame_params.hpp"
using namespace std;
///TODO:TFY--distance computation and landmarks stabilize
float distance_TFY(cv::Point pre,cv::Point now)
{
    return sqrt((now.x-pre.x)*(now.x-pre.x)+(now.y-pre.y)*(now.y-pre.y));
}

rcr::LandmarkCollection<cv::Vec2f> landmarks_stabilize(rcr::LandmarkCollection<cv::Vec2f> current_landmarks,Pre_frame_params pre_frame)
{
    float v=2.5+(pre_frame.pre_rect.width-180)*0.005;
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator c_element=begin(current_landmarks);
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator p_element=begin(pre_frame.pre_landmarks);
    float sum_eye_nouse=0,sum_mouse=0;
    for(int i=0;i<9;i++)
    {
        cv::Point current_p=cv::Point(c_element->coordinates[0],c_element->coordinates[1]);
        cv::Point pre_p=cv::Point(p_element->coordinates[0],p_element->coordinates[1]);
        float dis=distance_TFY(current_p,pre_p);
        if(i<7)
            sum_eye_nouse+=dis;
        if(i>6)
            sum_mouse+=dis;
        c_element++;
        p_element++;
    }
    //std::cout<<"box.width:"<<box.width<<endl;
    //std::cout<<"sum_eye_nouse:"<<sum_eye_nouse/7<<",sum_mouse:"<<sum_mouse/2<<",v:"<<v<<endl;
    if(sum_eye_nouse/7<v)
    {
        for(int i=0;i<7;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    if(sum_mouse/2<v)
    {
        for(int i=7;i<9;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    return current_landmarks;

}
rcr::LandmarkCollection<cv::Vec2f> landmarks_stabilize_22(rcr::LandmarkCollection<cv::Vec2f> current_landmarks,Pre_frame_params pre_frame)
{
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator c_element=begin(current_landmarks);
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator p_element=begin(pre_frame.pre_landmarks);
    float sum_eye_nouse=0,sum_mouse=0;
    for(int i=0;i<18;i++)
    {
        cv::Point current_p=cv::Point(c_element->coordinates[0],c_element->coordinates[1]);
        cv::Point pre_p=cv::Point(p_element->coordinates[0],p_element->coordinates[1]);
        float dis=distance_TFY(current_p,pre_p);
        //0-8 chin;9-15 eye,nouse;16 17 mouse;
        if(i<9)
            sum_mouse+=dis;
        else if(i<16)
            sum_eye_nouse+=dis;
        else
            sum_mouse+=dis;
        c_element++;
        p_element++;
    }
    if(sum_eye_nouse/7<2)
    {
        for(int i=9;i<16;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    if(sum_mouse/15<2)
    {
        for(int i=0;i<9;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
        for(int i=16;i<22;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    return current_landmarks;

}

rcr::LandmarkCollection<cv::Vec2f> landmarks_stabilize_21(rcr::LandmarkCollection<cv::Vec2f> current_landmarks,Pre_frame_params pre_frame)
{
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator c_element=begin(current_landmarks);
    rcr::LandmarkCollection<cv::Vec2f>::const_iterator p_element=begin(pre_frame.pre_landmarks);
    float sum_eye_nouse=0,sum_mouse=0;
    for(int i=0;i<18;i++)
    {
        cv::Point current_p=cv::Point(c_element->coordinates[0],c_element->coordinates[1]);
        cv::Point pre_p=cv::Point(p_element->coordinates[0],p_element->coordinates[1]);
        float dis=distance_TFY(current_p,pre_p);
        //0-8 chin;9-15 eye,nouse;16 17 mouse;
        if(i<6)
            sum_mouse+=dis;
        else if(i<17)
            sum_eye_nouse+=dis;
        else
            sum_mouse+=dis;
        c_element++;
        p_element++;
    }
    if(sum_eye_nouse/12<2)
    {
        for(int i=5;i<17;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    if(sum_mouse/9<2)
    {
        for(int i=0;i<5;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
        for(int i=17;i<21;i++)
        {
            current_landmarks[i].coordinates[0]=pre_frame.pre_landmarks[i].coordinates[0];
            current_landmarks[i].coordinates[1]=pre_frame.pre_landmarks[i].coordinates[1];
        }
    }
    return current_landmarks;

}
#endif
