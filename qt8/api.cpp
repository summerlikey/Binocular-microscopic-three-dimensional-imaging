#include "api.h"
using namespace AVT::VmbAPI;
using namespace std;

enum    { NUM_FRAMES=3, };//帧队列的个数
Api::Api():m_system(VimbaSystem::GetInstance())
{

}
Api::~Api()
{

}

VmbErrorType Api::ApiStartUp()//打开系统
{
    VmbErrorType res;
    res = m_system.Startup();
    if(VmbErrorSuccess == res)
    {
        cout<<"m_system is startup"<<endl;
        // This will be wrapped in a shared_ptr so we don't delete it
        SP_SET(m_pCameraObserver,new CameraObserver);
        // Register an observer whose callback routine gets triggered whenever a camera is plugged in or out
        res=m_system.RegisterCameraListObserver(m_pCameraObserver);
    }
    return res;
}
VimbaSystem& Api::getSystem(){
    return m_system;
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

CameraPtrVector Api::GetCameraList()//获取相机
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
    return SP_DYN_CAST(m_pCameraObserver,CameraObserver).get();
}


std::string Api::ErrorCodeToMessages(VmbErrorType eErr)
{
    return ErrorCodeToMessage(eErr);
}
