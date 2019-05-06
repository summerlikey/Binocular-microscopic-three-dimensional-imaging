#include "frameimage.h"
#include <QApplication>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frameimage w;
    w.show();
    qDebug()<<"main"<<QThread::currentThread();
    return a.exec();
}
