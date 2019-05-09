#ifndef LEFTCAMERAOBSERVER_H
#define LEFTCAMERAOBSERVER_H

#include "frameobserver.h"

class LeftCameraObserver
{
public:
    LeftCameraObserver();
    int GetHeight()const;//获取私有数据成员m_nHeight
    int GetWidth()const;//获取室友数据成员m_nWidth
    FramePtr GetFrame();//获取私有数据成员，帧观察器对应的帧
    QObject* GetFrameObserver();//QOject帧对象
    VmbErrorType QueueFrame(CameraPtr LeftCamera,FramePtr pFrame);//将帧放入队列，这个在sdk中有直接封装好的函数可以直接用，这里自己写的，返回错误类型VmbErrorType err
    VmbPixelFormatType GetPixelFormat() const;//获取私有数据成员m_nPixelFormat
    VmbErrorType StartContinuousImageAcquisition(CameraPtr Camera);//打开选定的相机
private:
    IFrameObserverPtr m_pFrameObserver;//帧观察器
    VmbInt64_t m_nWidth;//目前的宽度
    VmbInt64_t m_nHeight;//目前的高度
    VmbInt64_t m_nPixelFormat;//格式
};

#endif // LEFTCAMERAOBSERVER_H
