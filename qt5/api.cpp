#include "api.h"
using namespace AVT::VmbAPI;
using namespace std;

Api::Api():m_system(VimbaSystem::GetInstance())
{

}
Api::~Api()
{

}

void Api::ApiStartUp()//打开系统
{
    VmbErrorType res;
    res = m_system.Startup();
    if(VmbErrorSuccess == res)
    {
        cout<<"m_system is startup"<<endl;
    }

}
void Api::ApiShutDown()//关闭系统
{
    VmbErrorType res;
    res = m_system.Shutdown();
    if(VmbErrorSuccess==res)
    {
        cout<<"m_system is shutdown "<<endl;
    }
}

CameraPtrVector Api::GetCameraList()//获取相机定义
{
    CameraPtrVector cameras;
    // Get all known cameras
    if( VmbErrorSuccess == m_system.GetCameras( cameras ) )
    {
        // And return them
        return cameras;
    }
    return CameraPtrVector();
}

// Returns the camera observer as QObject pointer to connect their signals to the view's slots
QObject* Api::GetCameraObserver()
{
    //return SP_DYN_CAST(m_pCameraObserver, CameraObserver).get();

    m_pCameraObserver=new CameraObserver();
    if(VmbErrorSuccess==m_system.RegisterCameraListObserver( ICameraListObserverPtr (m_pCameraObserver)))
    {
        cout<<"m_system.RegisterCameraListObserver"<<endl;

        return m_pCameraObserver;
    }

}
