#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QDebug>
#include <QMainWindow>

#include <QDir>
#include <QUrl>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmapCache>
#include <QProgressDialog>
#include <QStandardPaths>

#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "runparameter.h"


namespace Ui {
class VideoControl;
}

class VideoControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoControl(QWidget *parent = 0);
    ~VideoControl();

public:
    void setUrl(const QUrl &url);
    void fetchVideoFrames(const QString & video_path);
    void startDetection();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showImage();

    void on_action_open_triggered();
    void on_action_Running_Info_triggered();
    void on_pushButton_startDetect_clicked();
    void on_horizontalSlider_play_control_valueChanged(int value);

private:
    std::vector<QImage> m_frameImgs;
    int m_lastFrameIdx; // 最近一帧播放的位置
    int m_curFrameIdx; // 当前准备播放的frame id
    cv::VideoCapture *m_pVideoCap;
    int m_video_fps; // 视频每秒播放多少帧
    QTimer* m_pPlay_Timer; //用于视频播放的定时器
    RunParameter* m_pParam_Widget; // 运行参数设置

private:
    Ui::VideoControl *ui;
};

#endif // VIDEOCONTROL_H
