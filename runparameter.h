#ifndef RUNPARAMETER_H
#define RUNPARAMETER_H

#include <QWidget>
#include <QMessageBox>
#include <QTcpSocket>

#include <QDebug>

#include <vector>
#include <string>

#include <iostream>

using namespace std;

namespace Ui {
class RunParameter;
}

class RunParameter : public QWidget
{
    Q_OBJECT

public:
    explicit RunParameter(QWidget *parent = 0);
    ~RunParameter();

public slots:
    void enable_comBox_select_model();
    void parese_server_content();

private slots:
    void on_comboBox_select_model_currentIndexChanged(int index);
    void on_pushButton_modelselect_clicked();
    void on_commandLinkButton_clicked();

private:
    Ui::RunParameter *ui;
    QTcpSocket* m_socket;

    bool m_isConnected;
};

#endif // RUNPARAMETER_H
