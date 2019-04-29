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
void Api::ApiShutDown()//关闭系统
{
    VmbErrorType res;

    res = m_system.Shutdown();
    if(VmbErrorSuccess==res)
    {
        cout<<"m_system is shutdown "<<endl;
    }
}

int Api::GetHeight()const
{
    return static_cast<int>(m_nHeight);
}
int Api::GetWidth()const
{
    return static_cast<int>(m_nWidth);
}

FramePtr Api::GetFrame()
{
    return SP_DYN_CAST( m_pFrameObserver, FrameObserver )->GetFrame();
}
VmbPixelFormatType Api::GetPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(m_nPixelFormat);
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
VmbErrorType Api::QueueFrame( FramePtr pFrame )
{
    VmbErrorType err;
    err = m_pCamera->QueueFrame(pFrame);
    return err;
}

void Api::ClearFrameQueue()
{
    SP_DYN_CAST( m_pFrameObserver,FrameObserver )->ClearFrameQueue();
}
// Returns the camera observer as QObject pointer to connect their signals to the view's slots
QObject* Api::GetCameraObserver()
{
    return SP_DYN_CAST(m_pCameraObserver,CameraObserver).get();
}

QObject* Api::GetFrameObserver()
{
    return SP_DYN_CAST(m_pFrameObserver, FrameObserver).get();
}
inline VmbErrorType SetValueIntMod2(const CameraPtr &camera,const std::string &featureName,VmbInt64_t &storage)//const 和引用
{
    VmbErrorType res;//错误类型
    FeaturePtr pfeature;
    res=camera->GetFeatureByName(featureName.c_str(),pfeature);
    if(VmbErrorSuccess==res)
    {
        VmbInt64_t minValue,maxValue;
        res=pfeature->GetRange(minValue,maxValue);//获取pfeature的最小值和最大值
        if(VmbErrorSuccess==res)//获取成功
        {
            maxValue=(maxValue>>1)<<1;//猜测去零
            res=pfeature->SetValue(maxValue);
            storage = maxValue;
        }
    }
    return res;
}

VmbErrorType Api::StartContinuousImageAcquisition(const std::string &rStrCameraID)//按钮
{
    VmbErrorType res = m_system.OpenCameraByID( rStrCameraID.c_str(), VmbAccessModeFull, m_pCamera );//打开相机通过ID，并且输出一个m_pCamera,以便后续操作,因为是引用所以数据成员也变为这个
    if( VmbErrorSuccess == res )//打卡成功
    {
        FeaturePtr pCommandFeature;//命令
        if(VmbErrorSuccess==m_pCamera->GetFeatureByName("GVSPAdjustPacketSize", pCommandFeature))//设置最大数据包，当连接两台相机后，选择是否改变
        {
            pCommandFeature->RunCommand();
            bool bIsCommandDone = false;
            do
            {
                if(VmbErrorSuccess!=pCommandFeature->IsCommandDone(bIsCommandDone))//检测命令是否执行
                    break;
            }while(bIsCommandDone==false);
        }
    }
    res = SetValueIntMod2( m_pCamera,"Width", m_nWidth );//width
    if(VmbErrorSuccess==res)
    {
        res=SetValueIntMod2(m_pCamera,"Height",m_nHeight);//height
        if(VmbErrorSuccess==res)
        {
            // Store currently selected image format
            FeaturePtr pFormatFeature;
            res=m_pCamera->GetFeatureByName("PixelFormat",pFormatFeature);
            if(VmbErrorSuccess==res)
            {
               res = pFormatFeature->GetValue(m_nPixelFormat);
               if(VmbErrorSuccess==res)
               {
                   //注册帧观察器
                   //IFrameObserverPtr m_pObserver(new FrameObserver(m_pCamera));
                   //m_pFrameObserver=m_pObserver;
                   SP_SET( m_pFrameObserver , new FrameObserver(m_pCamera));//新帧和新观察器绑定在一起，作用等同于上面两排
                   m_pCamera->StartContinuousImageAcquisition(NUM_FRAMES, m_pFrameObserver);//连续采集模式，每一帧匹配一个帧观察器，异步
                   //AcquireSingleImage()，同步单个图像
                   //AcquireMultipleImages()，同步多个图像
               }
            }
        }
    }
    if ( VmbErrorSuccess != res )
    {
        // If anything fails after opening the camera we close it
        m_pCamera->Close();//不成功关闭相机
    }

    return res;
}

VmbErrorType Api::StopContinuousImageAcquisition()
{
    m_pCamera->StopContinuousImageAcquisition();
    return m_pCamera->Close();
}

std::string Api::ErrorCodeToMessages(VmbErrorType eErr)
{
    return ErrorCodeToMessage(eErr);
}
