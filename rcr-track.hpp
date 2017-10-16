/*
 * superviseddescent: A C++11 implementation of the supervised descent
 *                    optimisation method
 * File: apps/rcr/rcr-track.cpp
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
#include "superviseddescent/superviseddescent.hpp"
#include "superviseddescent/regressors.hpp"

#include "rcr/landmarks_io.hpp"
#include "rcr/landmark.hpp"
#include "rcr/landmarks_detect.hpp"
#include "rcr/model.hpp"
#include "rcr/pre_frame_params.hpp"


#include "cereal/cereal.hpp"

#include"opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include <fstream>

using namespace superviseddescent;
using namespace cv;
using namespace std;

template<class T = int>
cv::Rect_<T> get_enclosing_bbox(cv::Mat landmarks)
{
    auto num_landmarks = landmarks.cols / 2;
    double min_x_val, max_x_val, min_y_val, max_y_val;
    cv::minMaxLoc(landmarks.colRange(0, num_landmarks), &min_x_val, &max_x_val);
    cv::minMaxLoc(landmarks.colRange(num_landmarks, landmarks.cols), &min_y_val, &max_y_val);
    return cv::Rect_<T>(min_x_val, min_y_val, max_x_val - min_x_val, max_y_val - min_y_val);
};

string facedetector="/home/clark/Workspace/C++/CMake_workspace/FaceAR_Detect_StaticLib_Use/bin/data/haarcascade_frontalface_alt2.xml",
       modelfile="/home/clark/Workspace/C++/CMake_workspace/FaceAR_Detect_StaticLib_Use/bin/data/model_9.bin";
rcr::detection_model rcr_model;
cv::CascadeClassifier face_cascade;
Pre_frame_params pre_frame;///to save params of pre frame

//int init();
//int dectectFace(Mat img,Rect &rect);
//vector<cv::Point> dectectLandmark(Mat img,Rect rect);
//void close();




