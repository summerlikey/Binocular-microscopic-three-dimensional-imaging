#ifndef VMBIMAGE_H
#define VMBIMAGE_H

#include <QObject>
#include<QImage>
#include<VimbaCPP/Include/VimbaCPP.h>
using namespace AVT::VmbAPI;

//只做图片显示
class vmbimage:public QObject
{
    Q_OBJECT
public:

    const QImage& Image() const;
signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    // Current camera image
    QImage image;
    // Function called by Vimba to receive the frame
    void FrameDoneCallback(CameraPtr camera,FramePtr frame);//回调函数
};

#endif // VMBIMAGE_H
