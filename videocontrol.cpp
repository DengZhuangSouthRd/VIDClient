#include "videocontrol.h"
#include "ui_videocontrol.h"

VideoControl::VideoControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoControl)
{
    ui->setupUi(this);
    ui->pushButton_startDetect->setDisabled(true);
    ui->pushButton_startDetect->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_frameImgs.clear();

    m_video_fps = 20;
    m_pPlay_Timer = nullptr;
    m_lastFrameIdx = 0;
    m_curFrameIdx = 0;

    m_pParam_Widget = new RunParameter();
}

VideoControl::~VideoControl() {
    delete ui;
    delete m_pParam_Widget;
}

void VideoControl::setUrl(const QUrl &url) {
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    fetchVideoFrames(url.toString());
    m_lastFrameIdx = 0;
    m_curFrameIdx = 0;
    showImage();
    ui->pushButton_startDetect->setEnabled(true);
}

void VideoControl::fetchVideoFrames(const QString &video_path) {
    m_pVideoCap = new cv::VideoCapture(video_path.toStdString());
    if(m_pVideoCap && !m_pVideoCap->isOpened()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("打开视频文件失败"), tr("请确认视频文件格式，视频文件是否损坏!"), 0, this);
        msgBox.show();
        delete m_pVideoCap;
        return ;
    }

    int total_frames = m_pVideoCap->get(CV_CAP_PROP_FRAME_COUNT);
    int video_fps = m_pVideoCap->get(CV_CAP_PROP_FPS);
    if(total_frames == 0)
        total_frames = video_fps * 60;
    //    int total_frames = m_pVideoCap->get(CV_CAP_PROP_POS_MSEC);
    QProgressDialog *load_video_progress = new QProgressDialog(this);
    load_video_progress->setModal(true);
    load_video_progress->setLabelText(tr("正在加载视频数据..."));
    load_video_progress->setCancelButton(nullptr);
    load_video_progress->setOrientation(Qt::Horizontal);
    load_video_progress->setRange(0, total_frames);
    load_video_progress->setValue(0);

    int idx = 0;

    while(true) {
        cv::Mat t_frame, rgb;
        bool cap_status = m_pVideoCap->read(t_frame);
        if(cap_status == false) break;
        idx++;

        cv::cvtColor(t_frame, rgb, cv::COLOR_BGR2RGB);
//        cv::cvtColor(t_frame, rgb, cv::COLOR_RGB2BGR);
        if(t_frame.channels() == 3) {
            m_frameImgs.push_back(
                        QImage(
                            (const unsigned char*)(rgb.data),
                            rgb.cols,
                            rgb.rows,
                            rgb.cols*rgb.channels(),
                            QImage::Format_RGB888).copy());
        } else {
            QMessageBox msgBox(QMessageBox::Warning, tr("解析视频出错"), tr("请确认视频文件格式，视频文件是否损坏，视频中图像的格式不对!"), 0, this);
            msgBox.show();
            if(m_pVideoCap) delete m_pVideoCap;
            if(load_video_progress) delete load_video_progress;
            exit(0);
        }
        load_video_progress->setValue(idx);
    }
    delete m_pVideoCap;
    delete load_video_progress;

    total_frames = m_frameImgs.size();
    qDebug() << "total_frames is " << total_frames << " video_fps is " << video_fps;

    //关联界面中播放控制进度条的数据
    ui->horizontalSlider_play_control->setRange(0, (int)m_frameImgs.size()-1);
    ui->horizontalSlider_play_control->setValue(1);
}

void VideoControl::showImage() {
    if(m_curFrameIdx >= (int)m_frameImgs.size()) {
        return ;
    }
    QPixmap t_pixmap = QPixmap::fromImage(m_frameImgs[m_curFrameIdx]);
    ui->label_image->setPixmap(t_pixmap);
    ui->label_image->resize(t_pixmap.size());
    ui->horizontalSlider_play_control->setValue(m_curFrameIdx);
    m_lastFrameIdx = m_curFrameIdx;
    m_curFrameIdx++;
    ui->label_framIdx->setText(tr("%04 帧").arg(m_curFrameIdx));
}

void VideoControl::startDetection() {
    if(m_pPlay_Timer == nullptr) {
        m_pPlay_Timer = new QTimer(this);
        connect(m_pPlay_Timer, SIGNAL(timeout()), this, SLOT(showImage()));
    }
    m_pPlay_Timer->setInterval(1000/m_video_fps);
    m_pPlay_Timer->start();
    if(m_curFrameIdx >= (int)m_frameImgs.size()) {
        m_pPlay_Timer->stop();
    }
}

void VideoControl::paintEvent(QPaintEvent *event) {
    event;
    //    qDebug() << "Brush Event !";
    //    int lx = ui->label_image->frameGeometry().x();
    //    int ly = ui->label_image->frameGeometry().y();
    if(m_curFrameIdx < (int)m_frameImgs.size()) {
        QPainter painter(this);
        //        painter.drawImage(lx, ly, *m_frameImgs[m_curFrameIdx]);
    }
}

// 传入的参数是视频名字或者是视频的存储路径
bool VideoControl::is_have_videoname(QString &video_name, QString &select_video) {
    QSet<QString> video_set;
    video_set.insert(tr("JL101B_MSS_20160904180811_000013363_101_001_L1B_MSS"));
    video_set.insert(tr("JL101B_MSS_20161017092509_000014631_101_001_L1B_MSS"));
    video_set.insert(tr("20170525_7.5114_突尼斯"));
    video_set.insert(tr("20170602_5.0133_明尼阿波利斯"));

    QStringList name_list = video_name.split("/");
    int n = name_list.size();
    QString name = name_list[n-1];
    name_list = name.split(".");
    name_list.removeAt(name_list.size()-1);
    name = name_list.join(".");
//    qDebug() << name;
    select_video = name;
    return video_set.contains(name);
}

void VideoControl::on_action_open_triggered() {
    if (!(m_pParam_Widget && m_pParam_Widget->isConnected())) {
        QMessageBox::warning(this, tr("Warning"), tr("请先连接服务器!"));
    } else {
        QFileDialog fileDialog(this);
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setWindowTitle(tr("加载检测视频"));
        QString str = "*.avi,*.mp4,*.mpg";
        QStringList supportedMimeTypes = str.split(",");
        if (!supportedMimeTypes.isEmpty())
            fileDialog.setMimeTypeFilters(supportedMimeTypes);
        fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
        if (fileDialog.exec() == QDialog::Accepted) {
            QUrl url = fileDialog.selectedUrls().constFirst();
            QString video_path = url.toString();
            QString select_name = "";
            bool is_have = is_have_videoname(video_path, select_name);
            if(is_have) {
                //先将视频相关信息送到服务器端进行处理
                m_pParam_Widget->send_to_server(select_name);
                //客户端进行视频数据的预处理
                setUrl(url);
            }
        }
    }
}

//弹出参数运行的设置框
void VideoControl::on_action_Running_Info_triggered() {
    m_pParam_Widget->show();
}

//对于按钮来说，可以进行播放和暂停
void VideoControl::on_pushButton_startDetect_clicked() {
    QString cur_button_text = ui->pushButton_startDetect->text();
    if(cur_button_text == tr("播放")) {
        ui->pushButton_startDetect->setText(tr("暂停"));
        ui->pushButton_startDetect->setStyleSheet("");
        ui->pushButton_startDetect->setStyleSheet("background-color: rgb(150, 0, 0);");
        startDetection();
    } else if(cur_button_text == tr("暂停")) {
        if(m_pPlay_Timer->isActive())
            m_pPlay_Timer->stop();
        ui->pushButton_startDetect->setText(tr("播放"));
        ui->pushButton_startDetect->setStyleSheet("");
        ui->pushButton_startDetect->setStyleSheet("background-color: rgb(0, 150, 0);");
    }
    ui->pushButton_startDetect->update();
}

//当播放条的位置由用户改变的时候，需要将图片切换到指定的位置
void VideoControl::on_horizontalSlider_play_control_valueChanged(int value) {
    m_curFrameIdx = value;
    showImage();
}

