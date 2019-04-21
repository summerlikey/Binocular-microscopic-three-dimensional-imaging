#include "vmbimage.h"

//获取图像
const QImage& vmbimage::Image() const{
    return image;
}
void vmbimage::FrameDoneCallback(CameraPtr camera,FramePtr frame){
    //检查帧设置
    VmbFrameStatusType status;//帧状态，是否完全
    frame->GetReceiveStatus(status);//帧状态
    VmbUchar_t* pBuffer;
    frame->GetImage(pBuffer);//图像数据
    VmbUint32_t imageSize;
    frame->GetImageSize(imageSize);//图像大小
    if(status==VmbFrameStatusComplete){
        //获取帧的图像对象
        //vmbimage* imageX;
        memcpy(image.bits(),pBuffer,imageSize);//将相机缓冲区复制到Qt图像
        //cout<<"图像就绪"<<endl;
        emit ImageReady();
    }
    camera->QueueFrame(frame);//重新帧排队
}
