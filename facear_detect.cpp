#include "facear_detect.hpp"
#include"rcr-track.hpp"

FaceAR_Detect::FaceAR_Detect()
{
}

int FaceAR_Detect::init(cv::VideoCapture &cap){
    //string facedetector="./data/haarcascade_frontalface_alt2.xml",modelfile="./data/model_9.bin";

    //    rcr::detection_model rcr_model;
    // Load the learned model:

    try {

        rcr_model = rcr::load_detection_model(modelfile);
    }
    catch (const cereal::Exception& e) {
        cout << "Error reading the RCR model " << modelfile << ": " << e.what() << endl;
        return EXIT_FAILURE;
    }

    // Load the face detector from OpenCV:

    if (!face_cascade.load(facedetector))
    {
        cout << "Error loading the face detector " << facedetector << "." << endl;
        return EXIT_FAILURE;
    }

    cap.open(0); // no file given, open the default camera
    if (!cap.isOpened()) {  // check if we succeeded
        cout << "Couldn't open the given file or camera 0." << endl;
        return EXIT_FAILURE;
    }
    ///load finished--------------------------------------
}

int FaceAR_Detect::dectectFace(Mat &img,cv::Rect &rect){
    int x=face_detect1(img,pre_frame,face_cascade,rcr_model);
    rect=pre_frame.pre_rect;
    return x;
}

vector<cv::Point> FaceAR_Detect::dectectLandmark(Mat &image){
    rcr::LandmarkCollection<cv::Vec2f> current_landmarks;
    landmarks_detect(image,pre_frame,rcr_model,current_landmarks,face_cascade);

    vector<cv::Point> landmarks(pre_frame.pre_landmarks.size());
    for(int i=0;i<pre_frame.pre_landmarks.size();i++)
    {
        landmarks[i].x=pre_frame.pre_landmarks[i].coordinates[0];
        landmarks[i].y=pre_frame.pre_landmarks[i].coordinates[1];
    }
    return landmarks;
}
vector<cv::Point> FaceAR_Detect::dectectLandmark(Mat &image,cv::Rect rect){
    rcr::LandmarkCollection<cv::Vec2f> current_landmarks = rcr_model.detect(image,rect);
    vector<cv::Point> landmarks(pre_frame.pre_landmarks.size());
    for(int i=0;i<current_landmarks.size();i++)
    {
        landmarks[i].x=current_landmarks[i].coordinates[0];
        landmarks[i].y=current_landmarks[i].coordinates[1];
    }
    return landmarks;
}


void FaceAR_Detect::close(cv::VideoCapture &cap){
    if(!cap.isOpened())
        cap.release();
}
void FaceAR_Detect::draw_Face(Mat &img,Rect rect){
    cv::rectangle(img,rect,Scalar(225,225,0));
}
void FaceAR_Detect::draw_landmark(Mat &img,vector<cv::Point> landmarks){
    rcr::LandmarkCollection<cv::Vec2f> current_landmarks(landmarks.size());
    for(int i=0;i<pre_frame.pre_landmarks.size();i++)
    {
        current_landmarks[i].coordinates[0]=landmarks[i].x;
        current_landmarks[i].coordinates[1]=landmarks[i].y;
    }
    rcr::draw_landmarks(img, current_landmarks);
}
