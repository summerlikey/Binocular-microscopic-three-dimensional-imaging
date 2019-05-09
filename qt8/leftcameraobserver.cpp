#include "leftcameraobserver.h"
enum    { NUM_FRAMES=3, };//帧队列的个数
LeftCameraObserver::LeftCameraObserver()
{

}
QObject* LeftCameraObserver::GetFrameObserver()
{
    return SP_DYN_CAST(m_pFrameObserver, FrameObserver).get();//指针的动态转换
}
FramePtr LeftCameraObserver::GetFrame()
{
    return SP_DYN_CAST( m_pFrameObserver, FrameObserver )->GetFrame();
}
int LeftCameraObserver::GetHeight()const
{
    return static_cast<int>(m_nHeight);
}
int LeftCameraObserver::GetWidth()const
{
    return static_cast<int>(m_nWidth);
}
VmbErrorType LeftCameraObserver::QueueFrame( CameraPtr LeftCamera,FramePtr pFrame )
{
    VmbErrorType err;
    err = LeftCamera->QueueFrame(pFrame);
    return err;
}
VmbPixelFormatType LeftCameraObserver::GetPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(m_nPixelFormat);
}
VmbErrorType LeftCameraObserver::StartContinuousImageAcquisition(CameraPtr Camera)//按钮
{
    VmbErrorType res;
        FeaturePtr pCommandFeature;//命令
        if(VmbErrorSuccess==Camera->GetFeatureByName("GVSPAdjustPacketSize", pCommandFeature))//设置最大数据包，当连接两台相机后，选择是否改变
        {
            pCommandFeature->RunCommand();
            bool bIsCommandDone = false;
            do
            {
                if(VmbErrorSuccess!=pCommandFeature->IsCommandDone(bIsCommandDone))//检测命令是否执行
                    break;
            }while(bIsCommandDone==false);
        }

                   //AcquireSingleImage()，同步单个图像
                   //AcquireMultipleImages()，同步多个图像
    if( VmbErrorSuccess != res )
    {
        // If anything fails after opening the camera we close it
        Camera->Close();//不成功关闭相机
    }
    return res;
}
