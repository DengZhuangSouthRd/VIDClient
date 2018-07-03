#include "runparameter.h"
#include "ui_runparameter.h"

RunParameter::RunParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunParameter)
{
    ui->setupUi(this);

    m_socket = NULL;
    m_isConnected = false;
}

RunParameter::~RunParameter() {
    delete ui;
}

bool RunParameter::isConnected() {
    return m_isConnected;
}

bool RunParameter::send_to_server(QString content) {
    if(m_socket && m_isConnected) {
        QByteArray bcontent(content.toStdString().data());
        qint64 len = m_socket->write(bcontent);
        qDebug() << len << " = " << bcontent.size();
        return len == bcontent.size();
    }
    return false;
}

void RunParameter::on_comboBox_select_model_currentIndexChanged(int index) {
    vector<string> models = {
        "None",
        "VGG+SSD",
        "VGG+DSSD",
        "SqueezeNet+SSD"
    };
    if (index < 0 || index > (int)models.size()) {
        std::cout  << "index out of range !" << std::endl;
    } else {
        std::cout << models[index] << std::endl;
        ui->comboBox_select_model->setDisabled(true);
    }
}

void RunParameter::enable_comBox_select_model() {
    ui->comboBox_select_model->setEnabled(true);
    ui->lineEdit_mAP->clear();
    ui->lineEdit_percision->clear();
    ui->lineEdit_recall->clear();
}

void RunParameter::on_pushButton_modelselect_clicked() {
    this->enable_comBox_select_model();
}

//连接服务器代码
void RunParameter::on_commandLinkButton_clicked() {
    QString ip = ui->lineEdit_server_ip->text();
    QString port = ui->lineEdit_server_port->text();
    qDebug() << ip << ", " << port;

    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if (!rx2.exactMatch(ip)) {
        QMessageBox::warning(this, "warning", "IP Address Invalid !");
    } else {
        qDebug() << "IP Address Valid !";
        if (m_isConnected == false) {
            m_socket = new QTcpSocket();
            m_socket->connectToHost(ip, port.toUInt(), QTcpSocket::ReadWrite);

            connect(m_socket, &QTcpSocket::connected,
                    [=]() {
                m_isConnected = true;
                ui->commandLinkButton->setText(tr("断开连接"));
            });
//            connect(m_socket, SIGNAL(connected()), this, SLOT(communicate_with_server()));
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(parese_server_content()));
        } else {
            m_isConnected = false;
            ui->commandLinkButton->setText(tr("服务连接"));

            m_socket->disconnectFromHost();
        }
    }
}

void RunParameter::parese_server_content() {
    QByteArray array = m_socket->readAll();
    qDebug(array);
}
