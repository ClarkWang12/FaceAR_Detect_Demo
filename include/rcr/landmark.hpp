/*
 * superviseddescent: A C++11 implementation of the supervised descent
 *                    optimisation method
 * File: rcr/landmark.hpp
 *
 * Copyright 2015 Patrik Huber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#ifndef RCR_LANDMARK_HPP_
#define RCR_LANDMARK_HPP_

#include<opencv2/opencv.hpp>
#include <string>
#include <vector>
using namespace cv;
namespace rcr {

/**
 * Representation of a landmark, consisting of a landmark name and
 * coordinates of the given type. Usually, the type would be cv::Vec2f.
 */
template<class LandmarkType>
struct Landmark
{
	std::string name;
	LandmarkType coordinates;
};

/**
 * A trivial collection of landmarks that somehow belong together.
 */
template<class LandmarkType> using LandmarkCollection = std::vector<Landmark<LandmarkType>>;

/**
 * Filters the given LandmarkCollection and returns a new LandmarkCollection
 * containing all landmarks whose name matches the one given by \p filter.
 *
 * @param[in] landmarks The input LandmarkCollection to be filtered.
 * @param[in] filter A landmark name (identifier) by which the given LandmarkCollection is filtered.
 * @return A new, filtered LandmarkCollection.
 */
template<class T>
LandmarkCollection<T> filter(const LandmarkCollection<T>& landmarks, const std::vector<std::string>& filter)
{
	LandmarkCollection<T> filtered_landmarks;
	using std::begin;
	using std::end;
	std::copy_if(begin(landmarks), end(landmarks), std::back_inserter(filtered_landmarks),
		[&](const Landmark<T>& lm) { return std::find(begin(filter), end(filter), lm.name) != end(filter); }
	);
	return filtered_landmarks;
};
///TODO:TFY-Computing the bounding-box of the landmarks
cv::Rect get_bounding_box(LandmarkCollection<cv::Vec2f> landmarks)
{
    cv::Rect box;
    float max_x,max_y,min_x,min_y;
    max_x=landmarks[0].coordinates[0];min_x=landmarks[0].coordinates[0];
    max_y=landmarks[0].coordinates[1];min_y=landmarks[0].coordinates[1];
    for(int i=0;i<landmarks.size();i++)
    {
        if(landmarks[i].coordinates[0]>max_x)
            max_x=landmarks[i].coordinates[0];
        if(landmarks[i].coordinates[0]<min_x)
            min_x=landmarks[i].coordinates[0];
        if(max_y<landmarks[i].coordinates[1])
            max_y=landmarks[i].coordinates[1];
        if(min_y>landmarks[i].coordinates[1])
            min_y=landmarks[i].coordinates[1];
    }
    box=cv::Rect(min_x,min_y,max_x-min_x,max_y-min_y);
    return box;
}
/////TODO:TFY--affine-point
bool pointAffine(rcr::LandmarkCollection<cv::Vec2f> &landmarks,cv::Mat rot_mat)
{
    //std::cout<<"before:"<< landmarks[0].coordinates[0]<<","<<landmarks[0].coordinates[1]<<std::endl;
    cv::Mat points=cv::Mat(2,landmarks.size(),CV_64F);
    for(int i=0;i<landmarks.size();i++)
    {
        points.at<double>(0,i)=landmarks[i].coordinates[0];
        points.at<double>(1,i)=landmarks[i].coordinates[1];
    }
    for(int i=0;i<points.cols;i++)
    {
        landmarks[i].coordinates[0]=points.at<double>(0,i)*rot_mat.at<double>(0,0)+points.at<double>(1,i)*rot_mat.at<double>(0,1);
        landmarks[i].coordinates[1]=points.at<double>(0,i)*rot_mat.at<double>(1,0)+points.at<double>(1,i)*rot_mat.at<double>(1,1);
    }
    //std::cout<<"rot_mat:"<<rot_mat<<std::endl;
    for(int i=0;i<landmarks.size();i++)
    {
        landmarks[i].coordinates[0]+=rot_mat.at<double>(0,2);
        landmarks[i].coordinates[1]+=rot_mat.at<double>(1,2);
    }
    //std::cout<<"after:"<< landmarks[0].coordinates[0]<<","<<landmarks[0].coordinates[1]<<std::endl;

}
bool pointAffine(cv::Point &landmarks,cv::Mat rot_mat)
{
    //std::cout<<"before_rect:"<< landmarks.x<<","<<landmarks.y<<std::endl;
   double t;
        t=landmarks.x*rot_mat.at<double>(0,0)+landmarks.y*rot_mat.at<double>(0,1);
        landmarks.y=landmarks.x*rot_mat.at<double>(1,0)+landmarks.y*rot_mat.at<double>(1,1);
   landmarks.x=t;
    //std::cout<<"rot_mat:"<<rot_mat<<std::endl;
        landmarks.x+=rot_mat.at<double>(0,2);
        landmarks.y+=rot_mat.at<double>(1,2);
    //std::cout<<"after_rect:"<< landmarks.x<<","<<landmarks.y<<std::endl;

}
void getRotMat(cv::Point center,float degree,Mat &mm)
{
    double angle = degree*CV_PI/180;
    double alpha = cos(angle);
    double beta = sin(angle);

    double m[6];
    m[0] = alpha;
    m[1] = beta;
    m[2] = (1-alpha)*center.x - beta*center.y;
    m[3] = -beta;
    m[4] = alpha;
    m[5] = beta*center.x + (1-alpha)*center.y;

    mm.at<double>(0,0)=m[0];
    mm.at<double>(0,1)=m[1];
    mm.at<double>(0,2)=m[2];
    mm.at<double>(1,0)=m[3];
    mm.at<double>(1,1)=m[4];
    mm.at<double>(1,2)=m[5];
}
void areaLimit(cv::Rect &rect,cv::Mat image)
{
    if(rect.x<0)
        rect.x=0;
    if(rect.y<0)
        rect.y=0;
    if((rect.x+rect.width)>(image.cols-1))
    {
        rect.x=image.cols-1-rect.width;
        if(rect.x<0)
        {rect.x=0;rect.width=image.cols-1;}
    }
    if((rect.y+rect.height)>(image.rows-1))
    {
        rect.y=image.rows-1-rect.height;
        if(rect.y<0)
        {rect.y=0;rect.height=image.rows-1;}
    }

}

} /* namespace rcr */

#endif /* RCR_LANDMARK_HPP_ */
