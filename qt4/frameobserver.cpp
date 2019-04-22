#include "frameobserver.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/Camera.h"
using namespace AVT::VmbAPI;
const QImage& FrameObserver::Image() const{
    return imageX;
}

//观察器，回调函数

void FrameObserver::FrameReceived( const FramePtr pFrame )
{
    VmbFrameStatusType eReceiveStatus;
    if( VmbErrorSuccess == pFrame ->GetReceiveStatus( eReceiveStatus ) )
    {

        if ( VmbFrameStatusComplete == eReceiveStatus )
        {
            //检查帧设置
            VmbFrameStatusType status;//帧状态，是否完全
            pFrame->GetReceiveStatus(status);//帧状态
            VmbUchar_t* pBuffer;
            pFrame->GetImage(pBuffer);//图像数据
            VmbUint32_t imageSize;
            pFrame->GetImageSize(imageSize);//图像大小
            //获取帧的图像对象
            //vmbimage* imageX;
            memcpy(imageX.bits(),pBuffer,imageSize);//将相机缓冲区复制到Qt图像
            //cout<<"图像就绪"<<endl;
            //emit ImageReady();
        }
        else
        {
             // Put your code here to react on an unsuccessfully received frame

        }
    }
    // When you are finished copying the frame , re-queue it
    m_pCam->QueueFrame(pFrame);

}


