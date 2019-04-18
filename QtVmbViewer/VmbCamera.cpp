#include "VmbCamera.h"

const int VmbCamera::frame_buffer_size=5;

void VmbCamera::Startup(){
    // Initialize Vimba
    VmbStartup();
    //送发现包给相机
    VmbFeatureCommandRun(gVimbaHandle,"GeVDiscoveryAllOnec");
    //获取第一个相机
    VmbCameraInfo_t camera;
    VmbUint32_t count;
    VmbCamerasList(&camera,1,&count,sizeof(VmbCameraInfo_t));
    //链接相机
    VmbCameraOpen(camera.cameraIdString,VmbAccessModeFull,&handle);
    //调整包大小
    VmbFeatureCommandRun(handle,"GVSPAdjustPacketSize");
    //获取图像参数
    VmbInt64_t width;
    VmbInt64_t height;
    VmbInt64_t payload_size;
    VmbFeatureIntGet(handle,"Width",&width);
    VmbFeatureIntGet(handle,"Highet",&height);
    VmbFeatureIntGet(handle,"PayloadSize",&payload_size);
    //创造qt 灰度图像
    image=QImage(width,height,QImage::Format_Indexed8);
    //为qt 创建索引颜色表
    image.setColorCount(256);
    for(int i=0;i<255;i++)
        image.setColor(i,qRgb(i,i,i));
    //初始化帧缓冲区
    frame_buffer =(VmbFrame_t*)malloc(frame_buffer_size * sizeof(VmbFrame_t));
    //分配帧
    for(int i=0;i<frame_buffer_size;i++){
        frame_buffer[i].buffer=(VmbUchar_t*)malloc((VmbUint32_t)payload_size);
        frame_buffer[i].bufferSize=(VmbUint32_t)payload_size;
        frame_buffer[i].context[0]=this;
    }
}
// Shutdown the camera and Vimb
void VmbCamera::Shutdown(){
    //关闭相机
    VmbCameraClose(handle);
    //关闭Vimba
    VmbShutdown();
    //释放帧
    for(int i=0;i<frame_buffer_size;i++)
    {
        free(frame_buffer[i].buffer);
    }
    free(frame_buffer);
}
void VmbCamera::StartCapture() const{
    //宣布帧
    for(int i=0;i<frame_buffer_size;i++)
        VmbFrameAnnounce(handle,&frame_buffer[i],sizeof (VmbFrame_t));
    //开始捕获引擎
    VmbCaptureStart(handle);
    //帧队列，定义回调函数
    for(int i=0;i<frame_buffer_size;i++)
        VmbCaptureFrameQueue(handle,&frame_buffer[i],&FrameDoneCallback);
    //开始抓取
    VmbFeatureCommandRun(handle,"AcquisitionStart");

}
//停止采集
void VmbCamera::StopCapture() const{
    //停止抓取
    VmbFeatureCommandRun(handle,"AcquisitionStop");
    //释放帧
    VmbCaptureQueueFlush(handle);
    //停止捕获引擎
    VmbCaptureEnd(handle);
    //撤销帧
    VmbFrameRevokeAll(handle);

}
//获取曝光值
double VmbCamera::Exposure() const{
    double exposure;
    VmbFeatureFloatGet(handle,"ExposureTimeAbs",&exposure);
    return exposure;

}
//设置曝光值
void VmbCamera::SetExposure(double exposure) const{
    VmbFeatureFloatSet(handle,"ExposureTimeAbs",exposure);

}
//获取图像
const QImage& VmbCamera::Image() const{
    return image;
}
//当每个帧填充满，调用回调函数
void VMB_CALL VmbCamera::FrameDoneCallback(const VmbHandle_t camera_handle,VmbFrame_t* frame_pointer){
    //检查帧设置
    if(frame_pointer->receiveStatus==VmbFrameStatusComplete){
        //获取帧的相机对象
        VmbCamera* camera=(VmbCamera*)frame_pointer->context[0];
        //将相机缓冲区复制到Qt图像
        memcpy(camera->image.bits(),frame_pointer->buffer,frame_pointer->bufferSize);
        //发出图像就绪信号
        emit camera->ImageReady();
    }
    //重新帧排队
    VmbCaptureFrameQueue(camera_handle,frame_pointer,&FrameDoneCallback);
}
