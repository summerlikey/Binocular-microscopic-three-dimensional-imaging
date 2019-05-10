#ifndef API_H
#define API_H

#include "VimbaCPP/Include/VimbaCPP.h"
#include <iostream>
#include "cameraobserver.h"
#include "frameobserver.h"
#include "errorcodetomessage.h"
using namespace AVT::VmbAPI;

class Api
{
public:
    Api();//构造函数
    ~Api();//析构函数
    VmbErrorType ApiStartUp();//打开CDK,返回错误状态err
    void ApiShutDown();//关闭CDK


    std::string ErrorCodeToMessages(VmbErrorType eErr);
    CameraPtrVector GetCameraList();//获得所有相机，返回所有相机
    QObject* GetCameraObserver();//QOject对象

    VimbaSystem& getSystem();
private:
    // A reference to our Vimba singleton
    VimbaSystem& m_system;//系统初始化，利用构造函数
    CameraPtr m_pCamera;//由sartcontinuousimageacquisition传递上来，采用引用&的方式
    // Our camera observer
    //CameraObserver *m_pCameraObserver;//和帧观察器采用不同的方式实现
    ICameraListObserverPtr m_pCameraObserver;
};
#endif // API_H
