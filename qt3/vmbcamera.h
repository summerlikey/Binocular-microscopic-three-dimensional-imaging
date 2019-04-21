#ifndef VMBCAMERA_H
#define VMBCAMERA_H

#include <QObject>
#include <QImage>
#include <VimbaCPP/Include/VimbaCPP.h>
using namespace AVT::VmbAPI;
class vmbcamera:public QObject
{
    Q_OBJECT
public:
    vmbcamera();
    void StartImage();
    // Get the camera image
    const QImage& Image() const;
    // Current camera image
    QImage image;

signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    // Current camera image
    //QImage image;
    // Function called by Vimba to receive the frame


};
#endif // VMBCAMERA_H

void FrameDoneCallback(CameraPtr camera,FramePtr frame);//回调函数
