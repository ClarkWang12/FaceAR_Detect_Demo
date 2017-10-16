#pragma once

#ifndef CAMSHIFT_TFY_HPP_
#define CAMSHIFT_TFY_HPP_

#include <opencv2/opencv.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Mat hist_make(Mat image,Rect trackWindow)
{
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;

//    namedWindow( "Histogram", 0 );
    int vmin = 10, vmax = 256, smin = 30;
    Mat hsv, hue, mask, hist;//, histimg = Mat::zeros(200, 320, CV_8UC3);
    //bool paused = false;

    ///start detecting----------------------
    cvtColor(image, hsv, COLOR_BGR2HSV);
    int _vmin = vmin, _vmax = vmax;
    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    mixChannels(&hsv, 1, &hue, 1, ch, 1);
    Mat roi(hue, trackWindow), maskroi(mask, trackWindow);

    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
    normalize(hist, hist, 0, 255, NORM_MINMAX);

    //trackWindow = selection;
    //trackObject = 1;

//    histimg = Scalar::all(0);
//    int binW = histimg.cols / hsize;
//    Mat buf(1, hsize, CV_8UC3);
//    for( int i = 0; i < hsize; i++ )
//        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
//    cvtColor(buf, buf, COLOR_HSV2BGR);

//    for( int i = 0; i < hsize; i++ )
//    {
//        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
//        rectangle( histimg, Point(i*binW,histimg.rows),
//                   Point((i+1)*binW,histimg.rows - val),
//                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
//    }
//    imshow( "Histogram", histimg );
    return hist;
}


RotatedRect camshift_tfy(Mat &image,Rect trackWindow,Mat hist)
{

    float hranges[] = {0,180};
    const float* phranges = hranges;


//    namedWindow( "Histogram", 0 );
    int vmin = 10, vmax = 256, smin = 30;
    Mat hsv, hue, mask, backproj;
    //bool paused = false;

    ///start detecting----------------------
    cvtColor(image, hsv, COLOR_BGR2HSV);
    int _vmin = vmin, _vmax = vmax;

    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    mixChannels(&hsv, 1, &hue, 1, ch, 1);


    calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
    backproj &= mask;
    RotatedRect trackBox =cv::CamShift(backproj, trackWindow, TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));

//    if( trackWindow.area() <= 1 )
//    {
//        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
//        trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
//                   trackWindow.x + r, trackWindow.y + r) &Rect(0, 0, cols, rows);
//    }

//    ellipse( image, trackBox, Scalar(0,0,255), 1, LINE_AA );
//    //cv::rectangle(image,trackWindow,Scalar(0,255,255),1);
//    waitKey(5);

    return trackBox;
}
#endif
