#pragma once

#ifndef FACE_DETECT_HPP_
#define FACE_DETECT_HPP_
#include "landmark.hpp"
#include"rcr/pre_frame_params.hpp"
#include"rcr/model.hpp"

#include "opencv2/core/core.hpp"

#include <opencv2/core/utility.hpp>
#include "rcr/camshift_tfy.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include<math.h>
#include <iostream>
#include <cstring>
using namespace std;
using namespace cv;
namespace rcr {

///TODO:TFY-----face_detect
int face_detect(cv::Mat &image,Pre_frame_params &pre_frame,cv::CascadeClassifier face_cascade,rcr::detection_model rcr_model)
{
    using namespace std::chrono;
    time_point<system_clock> start, end;

    start = system_clock::now();
    vector<cv::Rect> detected_faces;
    cv::Rect best_box;
    RotatedRect trackBox;

    if(!pre_frame.detected)///if hadn't detected any faces at pre frame
    {
        face_cascade.detectMultiScale(image, detected_faces, 1.2, 2, 0, cv::Size(150, 150));//haar
        if(!detected_faces.empty())
        {
            best_box=detected_faces[0];
            pre_frame.isRot=0;
            pre_frame.pre_rect=best_box;
        }
        else
            return 0;
    }
    else
    {
        ///if had--set roi
        cv::Rect rect=cv::Rect(pre_frame.pre_rect.x-pre_frame.pre_rect.width*0.25,pre_frame.pre_rect.y-pre_frame.pre_rect.height*0.25,pre_frame.pre_rect.width*1.5,pre_frame.pre_rect.height*1.5);

        areaLimit(rect,image);
        cv::Mat roi=image(rect);
        cv::rectangle( image, rect, Scalar( 255, 255, 255 ), 2, 1 );//red
        face_cascade.detectMultiScale(roi, detected_faces, 1.2, 2, 0, cv::Size(150, 150));
        if (detected_faces.empty())
        {
            ///camshift------
            cv::Rect trackWin=cv::Rect(pre_frame.pre_rect.x-rect.x,pre_frame.pre_rect.y-rect.y,pre_frame.pre_rect.width,pre_frame.pre_rect.height);
            trackBox= camshift_tfy(roi,trackWin,pre_frame.pre_hist);
            if(trackBox.angle>90)trackBox.angle=trackBox.angle-180;
            else trackBox.angle=trackBox.angle;

            Mat rot_mat(2, 3, CV_64F);
            getRotMat(cv::Point(image.cols/2,image.rows/2),trackBox.angle,rot_mat);

            Mat image_affine;
            warpAffine(image,image_affine,rot_mat,image.size());
            cv::Point p=cv::Point(trackBox.center.x+rect.x,trackBox.center.y+rect.y);
            pointAffine(p,rot_mat);
            cv::Rect rect_aff=cv::Rect(p.x-rect.width/2,p.y-rect.height/2,rect.width,rect.height);

            areaLimit(rect_aff,image);
            cv::Mat roi_aff=image_affine(rect_aff);
            //cv::rectangle( image_affine, rect_aff, Scalar( 255, 255, 255 ), 2, 1 );
            face_cascade.detectMultiScale(roi_aff, detected_faces, 1.2, 2, 0, cv::Size(150, 150));

            if(!detected_faces.empty())
            {
                best_box=detected_faces[0];
                best_box.x+=rect_aff.x;
                best_box.y+=rect_aff.y;
                pre_frame.isRot=1;
                pre_frame.angle=trackBox.angle;
                pre_frame.pre_rect_affine=best_box;
                pre_frame.pre_img_affine=image_affine;

                best_box.x=trackBox.center.x+rect.x-best_box.width/2;
                best_box.y=trackBox.center.y+rect.y-best_box.height/2;
                pre_frame.pre_rect=best_box;
            }///------camshift finished
            else
            {
                best_box=trackWin;

                best_box.x=trackBox.center.x+rect.x-trackWin.width/2;
                //best_box.y=trackBox.center.y+rect.y-trackWin.height/2;
                best_box.y=trackWin.y+rect.y;
                if(best_box.width<200||best_box.height<200)
                {
                    best_box.width=200;
                    best_box.height=200;
                }
                if(best_box.width>best_box.height)
                    best_box.height=best_box.width;
                else
                    best_box.width=best_box.height;
                areaLimit(best_box,image);
                pre_frame.isRot=0;
                cv::rectangle( image, best_box, Scalar( 255, 0, 255 ), 2, 1 );//zise

                std::vector<std::string> l_ids=rcr_model.get_landmark_ids();
                std::string re0=rcr_model.get_lefteye_id0();
                std::string le1=rcr_model.get_righteye_id1();
                int le,re;
                for(int i=0;i<l_ids.size();i++)
                {
                    if( le1.compare(l_ids[i])==0)
                        le=i;
                    else if(re0.compare(l_ids[i])==0)
                    {
                        re=i;
                        break;
                    }
                }
                float dx=(float)(pre_frame.pre_landmarks[re].coordinates[0]-pre_frame.pre_landmarks[le].coordinates[0]);
                float dy=(float)(pre_frame.pre_landmarks[re].coordinates[1]-pre_frame.pre_landmarks[le].coordinates[1]);
                float angle;
                if(dy!=0)
                {
                    angle=std::atan2(dx,dy);
                    angle=angle/M_PI*180;
                    if(angle>0)
                        angle=90-angle;
                    else
                        angle=-angle;
                }
                else
                    angle=0;
                std::cout<<"angle!!!!!!!!!!!!!!!!!!!!!!!!:"<<angle<<endl;

                pre_frame.angle=angle;
                Mat rm1(2, 3, CV_64F);
                getRotMat(cv::Point(image.cols/2,image.rows/2),angle,rm1);

                warpAffine(image,pre_frame.pre_img_affine,rm1,image.size());

                pointAffine(pre_frame.pre_landmarks,rm1);

                cv::Rect bounding_box;
                bounding_box=get_bounding_box(pre_frame.pre_landmarks);
                rcr::areaLimit(bounding_box,pre_frame.pre_img_affine);

                cv::Rect box;
                box.x=bounding_box.x+bounding_box.width/2-pre_frame.pre_rect.width/2;
                //box.y=bounding_box.y+bounding_box.height/2-pre_frame.pre_rect.width/2-20;
                box.y=bounding_box.y+bounding_box.height/2-pre_frame.pre_rect.width*3/5;
                std::cout<<".................!:"<<pre_frame.pre_rect.width<<endl;
                box.width=pre_frame.pre_rect.width;
                box.height=pre_frame.pre_rect.height;
                pre_frame.pre_rect_affine=box;
                rcr::areaLimit(box,pre_frame.pre_img_affine);
                cv::rectangle( pre_frame.pre_img_affine, box, Scalar( 0, 0, 255 ), 1, 1 );
                //imshow("image_aff",pre_frame.pre_img_affine);
                pre_frame.pre_rect=best_box;

                //pre_frame.pre_rect_affine=best_box;
                return -1;
            }
        }
        else
        {
            best_box=detected_faces[0];
            best_box.x+=rect.x;
            best_box.y+=rect.y;
            pre_frame.isRot=0;
            cv::rectangle(image, best_box, { 255, 0, 0 });
            pre_frame.pre_rect=best_box;
        }

    }

    ///start landmarks detecting----
    //areaLimit(best_box,image);


    end = system_clock::now();
    auto t_fd = duration_cast<milliseconds>(end - start).count();
    cout << "FaceDetect:" << t_fd << endl;
    if(pre_frame.isRot==1)
        return 2;
    return 1;
};

int face_detect1(cv::Mat &image,Pre_frame_params &pre_frame,cv::CascadeClassifier face_cascade,rcr::detection_model rcr_model)
{
    using namespace std::chrono;
    time_point<system_clock> start, end;

    start = system_clock::now();
    vector<cv::Rect> detected_faces;
    cv::Rect best_box;
    RotatedRect trackBox;

    if(!pre_frame.detected)///if hadn't detected any faces at pre frame
    {
        face_cascade.detectMultiScale(image, detected_faces, 1.2, 2, 0, cv::Size(150, 150));//haar
        if(!detected_faces.empty())
        {
            best_box=detected_faces[0];
            pre_frame.isRot=0;
            pre_frame.pre_rect=best_box;
            pre_frame.detected=1;
        }
        else
            return 0;
    }
    else
    {
        ///if had--set roi
        cv::Rect rect=cv::Rect(pre_frame.pre_rect.x-pre_frame.pre_rect.width*0.25,pre_frame.pre_rect.y-pre_frame.pre_rect.height*0.25,pre_frame.pre_rect.width*1.5,pre_frame.pre_rect.height*1.5);

        areaLimit(rect,image);
        cv::Mat roi=image(rect);
        cv::rectangle( image, rect, Scalar( 255, 255, 255 ), 2, 1 );//red
        face_cascade.detectMultiScale(roi, detected_faces, 1.2, 2, 0, cv::Size(150, 150));
        if (detected_faces.empty())
        {
            ///camshift------
            cv::Rect trackWin=cv::Rect(pre_frame.pre_rect.x-rect.x,pre_frame.pre_rect.y-rect.y,pre_frame.pre_rect.width,pre_frame.pre_rect.height);
            trackBox= camshift_tfy(roi,trackWin,pre_frame.pre_hist);
            if(trackBox.angle>90)trackBox.angle=trackBox.angle-180;
            else trackBox.angle=trackBox.angle;

            Mat rot_mat(2, 3, CV_64F);
            getRotMat(cv::Point(image.cols/2,image.rows/2),trackBox.angle,rot_mat);

            Mat image_affine;
            warpAffine(image,image_affine,rot_mat,image.size());
            cv::Point p=cv::Point(trackBox.center.x+rect.x,trackBox.center.y+rect.y);
            pointAffine(p,rot_mat);
            cv::Rect rect_aff=cv::Rect(p.x-rect.width/2,p.y-rect.height/2,rect.width,rect.height);

            areaLimit(rect_aff,image);
            cv::Mat roi_aff=image_affine(rect_aff);
            //cv::rectangle( image_affine, rect_aff, Scalar( 255, 255, 255 ), 2, 1 );
            face_cascade.detectMultiScale(roi_aff, detected_faces, 1.2, 2, 0, cv::Size(150, 150));

            if(!detected_faces.empty())
            {
                best_box=detected_faces[0];
                best_box.x+=rect_aff.x;
                best_box.y+=rect_aff.y;
                pre_frame.isRot=1;
                pre_frame.angle=trackBox.angle;
                pre_frame.pre_rect_affine=best_box;
                pre_frame.pre_img_affine=image_affine;

                best_box.x=trackBox.center.x+rect.x-best_box.width/2;
                best_box.y=trackBox.center.y+rect.y-best_box.height/2;
                pre_frame.pre_rect=best_box;
            }///------camshift finished
            else
            {
                best_box=trackWin;

                best_box.x=trackBox.center.x+rect.x-trackWin.width/2;
                //best_box.y=trackBox.center.y+rect.y-trackWin.height/2;
                best_box.y=trackWin.y+rect.y;
                if(best_box.width<200||best_box.height<200)
                {
                    best_box.width=200;
                    best_box.height=200;
                }
                if(best_box.width>best_box.height)
                    best_box.height=best_box.width;
                else
                    best_box.width=best_box.height;
                areaLimit(best_box,image);
                pre_frame.isRot=0;
                cv::rectangle( image, best_box, Scalar( 255, 0, 255 ), 2, 1 );//zise

                pre_frame.pre_rect=best_box;
                pre_frame.detected=1;
                pre_frame.pre_hist= hist_make(image,best_box);
                //pre_frame.pre_rect_affine=best_box;
                return -1;
            }
        }
        else
        {
            best_box=detected_faces[0];
            best_box.x+=rect.x;
            best_box.y+=rect.y;
            pre_frame.isRot=0;
            cv::rectangle(image, best_box, { 255, 0, 0 });
            pre_frame.pre_rect=best_box;
        }
        pre_frame.detected=1;
    }

    ///start landmarks detecting----
    //areaLimit(best_box,image);

    pre_frame.pre_hist= hist_make(image,best_box);
    end = system_clock::now();
    auto t_fd = duration_cast<milliseconds>(end - start).count();
    cout << "FaceDetect:" << t_fd << endl;
    if(pre_frame.isRot==1)
        return 2;
    return 1;
};

}
#endif /* LANDMARKS_IO_HPP_ */
