#include "camerathread.h"
#include <QDebug>
camerathread::camerathread()
{

}

void camerathread::run()
{
    qDebug()<<"ddd"<<"my current thread"<<QThread::currentThread();
    //QObject::connect(ui->left_start_btn,SIGNAL(clicked()), this,SLOT(LeftCamera_Startstop()));//控件左边相机
}
