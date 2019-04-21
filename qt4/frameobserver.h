#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H
#include <VimbaCPP/Include/IFrameObserver.h>
#include<QObject>
#include<QImage>
using namespace AVT::VmbAPI;
class FrameObserver : virtual public IFrameObserver,public QObject
{
    Q_OBJECT
public:
    //FrameObserver():IFrameObserver(){}
    FrameObserver(CameraPtr pCamera ) : IFrameObserver( pCamera){;}
    //
    // This is our callback routine that will be executed on every received frame.
    // Triggered by the API.
    // Parameters:
    //  [in]    pFrame          The frame returned from the API
    void FrameObserver::FrameReceived( const FramePtr pFrame );

    //
    // After the view has been notified about a new frame it can pick it up.
    // It is then removed from the internal queue
    //
    // Returns:
    // A shared pointer to the latest frame
    //
    //FramePtr GetFrame();
    //
    //Clears the internal (double buffering) frame queue
    //
    //void ClearFrameQueue();
    const QImage& Image() const;
    // Current camera image
    QImage imageX;//图片类

signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    //QImage imageX;//图片类

};
#include"frameobserver.h"
#endif // FRAMEOBSERVER_H
