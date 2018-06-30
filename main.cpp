#include "videocontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoControl w;
    w.show();

    return a.exec();
}
