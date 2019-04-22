#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <VimbaCPP/Include/VimbaCPP.h>
#include <VimbaCPP/Include/IFrameObserver.h>
#include<QObject>
#include<QImage>
#include<QTime>
using namespace AVT::VmbAPI;

void sleep(int msec);//定义延时函数
class FrameObserver : public QObject,public IFrameObserver
{
    Q_OBJECT
public:
    FrameObserver(CameraPtr pCamera ) : IFrameObserver( pCamera){;}
    virtual void FrameReceived( const FramePtr pFrame );
    const QImage& Image() const;
signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    QImage imageX;//图片类
    CameraPtr m_pCam;
};

#endif // FRAMEOBSERVER_H
