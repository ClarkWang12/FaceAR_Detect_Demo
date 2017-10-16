#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T22:06:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = FaceAR_Detect
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11
CONFIG +=staticlib
DEFINES += FACEAR_DETECT_LIBRARY

DESTDIR = ../lib


INCLUDEPATH += \
    /usr/local/include \
    /usr/local/include/opencv \
    /usr/local/include/opencv2 \
    /usr/include/eigen3 \
    ../cereal-1.1.1/include \
    ../include



LIBS += \
    -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab \
    -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn \
    -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_plot \
    -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd \
    -lopencv_viz -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text \
    -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc \
    -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect \
    -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs \
    -lopencv_photo -lopencv_imgproc -lopencv_core

SOURCES += ../facear_detect.cpp \
    ../include/rcr/face_detect.cpp \
    ../include/rcr/hog.c

HEADERS += facear_detect.hpp \
    ../include/rcr/pre_frame_params.hpp \
    ../include/rcr/camshift_tfy.hpp \
    ../include/rcr/landmarks_io.hpp \
    ../include/rcr/landmarks_detect.hpp \
    ../include/rcr/hog.h \
    ../include/rcr/landmarks_stabilize.hpp \
    ../include/rcr/landmark.hpp \
    ../include/rcr/helpers.hpp \
    ../include/rcr/adaptive_vlhog.hpp \
    ../include/rcr/model.hpp \
    ../include/rcr/face_detect.hpp \
    ../include/superviseddescent/verbose_solver.hpp \
    ../include/superviseddescent/regressors.hpp \
    ../include/superviseddescent/superviseddescent.hpp \
    ../include/superviseddescent/utils/mat_cerealisation.hpp \
    ../include/superviseddescent/utils/mat_serialization.hpp \
    ../include/superviseddescent/utils/ThreadPool.h \
    ../rcr-track.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
