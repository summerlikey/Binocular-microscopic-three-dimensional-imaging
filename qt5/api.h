#ifndef API_H
#define API_H

#include "VimbaCPP/Include/VimbaCPP.h"
#include <iostream>
#include "CameraObserver.h"

using namespace AVT::VmbAPI;

class Api
{
public:
    Api();//构造函数
    ~Api();//析构函数
    void ApiStartUp();//打开CDK
    void ApiShutDown();//关闭CDK
    CameraPtrVector GetCameraList();//获得所有相机，返回所有相机
    QObject* GetCameraObserver();//QOject对象
private:
    // A reference to our Vimba singleton
    VimbaSystem& m_system;//系统初始化，利用构造函数
    CameraPtr m_pCamera;
    // Our camera observer
    CameraObserver *m_pCameraObserver;
    //ICameraListObserverPtr *m_pCameraObserver;
};

#endif // API_H
