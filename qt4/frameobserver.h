#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <VimbaCPP/Include/VimbaCPP.h>
#include <VimbaCPP/Include/IFrameObserver.h>
#include<QObject>
#include<QImage>
using namespace AVT::VmbAPI;
class FrameObserver : public QObject,public IFrameObserver
{
    Q_OBJECT
public:
    //FrameObserver():IFrameObserver(){}
    FrameObserver(CameraPtr pCamera ) : IFrameObserver( pCamera){;}

    virtual void FrameReceived( const FramePtr pFrame );
    //

    FramePtr GetFrame();
    //
    void ClearFrameQueue();

    const QImage& Image() const;
    // Current camera image
    //QImage imageX;//图片类

signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    QImage imageX;//图片类
    CameraPtr m_pCam;

};

#endif // FRAMEOBSERVER_H
