#ifndef RUNPARAMETER_H
#define RUNPARAMETER_H

#include <QWidget>

namespace Ui {
class RunParameter;
}

class RunParameter : public QWidget
{
    Q_OBJECT

public:
    explicit RunParameter(QWidget *parent = 0);
    ~RunParameter();

private:
    Ui::RunParameter *ui;
};

#endif // RUNPARAMETER_H
