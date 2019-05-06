#ifndef API_H
#define API_H

#include "VimbaCPP/Include/VimbaCPP.h"
#include <iostream>
#include "CameraObserver.h"
#include"FrameObserver.h"
#include"ErrorCodeToMessage.h"
using namespace AVT::VmbAPI;

class Api
{
public:
    Api();//构造函数
    ~Api();//析构函数
    VmbErrorType ApiStartUp();//打开CDK,返回错误状态err
    void ApiShutDown();//关闭CDK
    int GetHeight()const;//获取私有数据成员m_nHeight
    int GetWidth()const;//获取室友数据成员m_nWidth
    FramePtr GetFrame();//获取私有数据成员，帧观察器对应的帧
    VmbPixelFormatType GetPixelFormat() const;//获取私有数据成员m_nPixelFormat
    VmbErrorType QueueFrame(FramePtr pFrame);//将帧放入队列，这个在sdk中有直接封装好的函数可以直接用，这里自己写的，返回错误类型VmbErrorType err

    void ClearFrameQueue();//清除帧队列
    std::string ErrorCodeToMessages(VmbErrorType eErr);
    CameraPtrVector GetCameraList();//获得所有相机，返回所有相机
    QObject* GetCameraObserver();//QOject对象
    QObject* GetFrameObserver();//QOject帧对象
    VmbErrorType StartContinuousImageAcquisition( const std::string &rStrCameraID );//打开选定的相机
    VmbErrorType StopContinuousImageAcquisition();//关闭

private:
    // A reference to our Vimba singleton
    VimbaSystem& m_system;//系统初始化，利用构造函数
    CameraPtr m_pCamera;//由sartcontinuousimageacquisition传递上来，采用引用&的方式
    // Our camera observer
    //CameraObserver *m_pCameraObserver;//和帧观察器采用不同的方式实现
    ICameraListObserverPtr m_pCameraObserver;
    IFrameObserverPtr m_pFrameObserver;
    //ICameraListObserverPtr *m_pCameraObserver;
    VmbInt64_t m_nWidth;//目前的宽度
    VmbInt64_t m_nHeight;//目前的高度
    VmbInt64_t m_nPixelFormat;//格式
};

#endif // API_H
