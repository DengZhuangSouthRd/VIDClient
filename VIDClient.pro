#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T16:35:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoTargetDetection
TEMPLATE = app

QMAKE_CXXFLAGS += -stdlib=libc++

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        videocontrol.cpp \
    runparameter.cpp

HEADERS += \
        videocontrol.h \
    runparameter.h

FORMS += \
        videocontrol.ui \
    runparameter.ui


INCLUDEPATH += /usr/local/Cellar/opencv/3.4.1_2/include/

LIBS += -L/usr/local/Cellar/opencv/3.4.1_2/lib

LIBS += -lopencv_calib3d \
-lopencv_core \
-lopencv_features2d \
-lopencv_flann \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_ml \
-lopencv_objdetect \
-lopencv_video \
-lopencv_videostab \
-lopencv_videoio
