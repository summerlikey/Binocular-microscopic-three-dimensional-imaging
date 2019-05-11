#include "camerathread.h"
#include<stdio.h>
#include<QByteArray>
#include<QDebug>
#include "VmbTransform.h"
using namespace AVT::VmbAPI;
enum    { NUM_FRAMES=3, };//帧队列的个数
CameraThread::CameraThread():NowSystem  (VimbaSystem::GetInstance())//线程系统初始化
{
    creatnumber++;
    //SystemStartup();
    qDebug()<<"cameraThread "<<creatnumber<<QThread::currentThreadId();
    //QObject :: connect(&timer,SIGNAL(timeout()),this,SLOT(slot1()));
    //QObject :: connect(GetFrameObserver(), SIGNAL(FrameReceivedSignal(int)),this,SLOT(LeftOnFrameReady(int)));

}
void CameraThread::SystemStartup()
{
    VmbErrorType res;

    res = NowSystem.Startup();
    if(VmbErrorSuccess==res)
    {
        qDebug()<<"Thread System is Start "<<endl;
    }
}

void CameraThread::SystemShutDown()
{
    VmbErrorType res;

    res = NowSystem.Shutdown();
    if(VmbErrorSuccess==res)
    {
        qDebug()<<"Thread System is shutdown "<<endl;
    }
}

VimbaSystem& CameraThread ::GetNowSystem(){
    return NowSystem;
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
QObject* CameraThread::GetFrameObserver()
{
    return SP_DYN_CAST(m_pFrameObserver, FrameObserver).get();
}
VmbErrorType CameraThread::QueueFrame( FramePtr pFrame )
{
    VmbErrorType err;
    err = ThreadCamera->QueueFrame(pFrame);
    return err;
}
void CameraThread :: ClearFrameQueue()
{
    SP_DYN_CAST( m_pFrameObserver,FrameObserver )->ClearFrameQueue();
}
FramePtr CameraThread::GetFrame()
{
    return SP_DYN_CAST( m_pFrameObserver, FrameObserver )->GetFrame();
}
int CameraThread :: GetHeight()const
{
    return static_cast<int>(m_nHeight);
}
int CameraThread :: GetWidth()const
{
    return static_cast<int>(m_nWidth);
}
VmbPixelFormatType CameraThread :: GetPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(m_nPixelFormat);
}

void CameraThread::SetNowCameraId(QString CameraId)
{
//    char *pId;
//    QByteArray ba=CameraId.toLatin1();
//    pId=ba.data();
    NowCameraId = CameraId;
}
QString CameraThread::GetNowCameraId()
{
    return NowCameraId;
}


/*
void CameraThread::GetNowCamera(VimbaSystem &system,QString CameraId)
{
    VmbErrorType err;
    char *pId;
    //FramePtrVector frames( 3 );

    QByteArray ba=CameraId.toLatin1();
    pId=ba.data();


    VmbInt64_t nPLS; // Payload size value
    FeaturePtr pFeature; // Generic feature pointer
    if(VmbErrorSuccess == system.OpenCameraByID(pId,VmbAccessModeFull,ThreadCamera))//LeftCamera是引用值
    {
        qDebug()<<"left camera open success";
        //设置数据包
        FeaturePtr pCommandFeature;//命令
        if(VmbErrorSuccess==ThreadCamera->GetFeatureByName("GVSPAdjustPacketSize",pCommandFeature))//设置最大数据包，当连接两台相机后，选择是否改变
            {
            pCommandFeature->RunCommand();
            //未检验是否成功
        }

         err = SetValueIntMod2( ThreadCamera,"Width", m_nWidth );//width
         err = SetValueIntMod2( ThreadCamera,"Height", m_nHeight );//height
         if(err==VmbErrorSuccess)
         {
             FeaturePtr pFormatFeature;
             ThreadCamera->GetFeatureByName("PixelFormat",pFormatFeature);
             pFormatFeature->GetValue(m_nPixelFormat);
             //注册帧观察器
             ThreadCamera ->GetFeatureByName("PayloadSize", pFeature);
             pFeature ->GetValue(nPLS);//大小
             setFrameSize(nPLS);
//             SP_SET (m_pFrameObserver,new FrameObserver(LeftCamera));//新帧和新观察器绑定在一起，作用等同于上面两排

//             LeftCamera->StartContinuousImageAcquisition(NUM_FRAMES,m_pFrameObserver);//连续采集模式，每一帧匹配一个帧观察器，异步

//             qDebug()<<"帧观察器";
             //AcquireSingleImage()，同步单个图像
             //AcquireMultipleImages()，同步多个图像
             //SP_SET (m_pFrameObserver,new FrameObserver(LeftCamera));//新帧和新观察器绑定在一起，作用等同于上面两排

             //err = LeftCamera->StartContinuousImageAcquisition(NUM_FRAMES,m_pFrameObserver);//连续采集模式，每一帧匹配一个帧观察器，异步
         }
    }
}
*/



void CameraThread::StopNowCamera()
{

    FeaturePtr pFeature; // Generic feature pointer
    VmbErrorType err;
    qDebug()<<"1";
    err = ThreadCamera -> GetFeatureByName( "AcquisitionStop", pFeature );
    err = pFeature ->RunCommand();
    qDebug()<<"ss";
    if(err== VmbErrorSuccess)
    {
        qDebug()<<"stop camera";
        ThreadCamera -> EndCapture();
        ThreadCamera -> FlushQueue();
        ThreadCamera -> RevokeAllFrames();
        ThreadCamera->Close();
    }
    else
    {
        qDebug()<<"not stop success";
    }


}

int CameraThread::creatnumber=0;//两种方法初始化静态数据成员int类型

void CameraThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    camerathread_canrun =false;
}

void CameraThread::run()
{
    qDebug()<<"run cameraThread "<<QThread::currentThreadId();
//    while(1)
//    {
//        if(GetCameraStatusIsRun()==false)
//            return;
//        else {
//            qDebug()<<GetNowCameraId()<<QThread::currentThreadId();
//        }
//    }

        if( GetCameraStatusIsRun() == false )
            return;
        else{
            VmbErrorType err;
            qDebug()<<"cameraThread  run() 111222"<<creatnumber<<"run"<<currentThreadId();

            //调用帧回调方式
            /*
            if(GetCameraStatusIsRun()==true)
            {

                VmbInt64_t nPLS; // Payload size value
                FeaturePtr pFeature; // Generic feature pointer

                    char *pId;
                    QByteArray ba=NowCameraId.toLatin1();
                    pId=ba.data();

                qDebug()<<*pId;
                //估计原因不能打开相机线程里面明天继续看这原因，已解决，相机打开成功原因id格式不对
                err=GetNowSystem() . OpenCameraByID(pId,VmbAccessModeFull,ThreadCamera);

                if(VmbErrorSuccess == err)//LeftCamera是引用值
                {

                    qDebug()<<"camera open success";
                    //设置数据包
                    FeaturePtr pCommandFeature;//命令
                    if(VmbErrorSuccess==ThreadCamera->GetFeatureByName("GVSPAdjustPacketSize",pCommandFeature))//设置最大数据包，当连接两台相机后，选择是否改变
                        {
                        pCommandFeature->RunCommand();
                        //未检验是否成功
                    }

                     VmbErrorType err;
                     err = SetValueIntMod2( ThreadCamera,"Width", m_nWidth );//width
                     err = SetValueIntMod2( ThreadCamera,"Height", m_nHeight );//height
                     if(err==VmbErrorSuccess)
                     {
                         FeaturePtr pFormatFeature;
                         ThreadCamera->GetFeatureByName("PixelFormat",pFormatFeature);
                         pFormatFeature->GetValue(m_nPixelFormat);
                         //注册帧观察器
                         ThreadCamera ->GetFeatureByName("PayloadSize", pFeature);
                         pFeature ->GetValue(nPLS);//大小
                         setFrameSize(nPLS);
                     }

                    FramePtrVector frames( 3 );

                    //FeaturePtr pFeature; // Any camera feature
                    for ( FramePtrVector::iterator iter = frames.begin() ; frames.end() != iter ; ++iter )
                    {
                        ( *iter ).reset( new Frame( FrameSize ) );
                        SP_SET (m_pFrameObserver,new FrameObserver(ThreadCamera));//新帧和新观察器绑定在一起，作用等同于上面两排
                        err = ( *iter )->RegisterObserver( m_pFrameObserver ) ;
                        ThreadCamera->AnnounceFrame(*iter);
                    }

                    err = ThreadCamera->StartCapture();

                    for ( FramePtrVector::iterator iter = frames.begin();frames.end() != iter;++iter )
                    {
                        err=ThreadCamera -> QueueFrame(*iter);
                    }

                    err = ThreadCamera -> GetFeatureByName( "AcquisitionStart", pFeature );
                    err = pFeature ->RunCommand();

                    if(err==VmbErrorSuccess)
                    {
                        qDebug()<<"observer success";
                    }


                    QObject::connect(GetFrameObserver(), SIGNAL(FrameReceivedSignal(int)),this,SLOT(NowOnFrameReady(int)),Qt:: DirectConnection);
                }
                else {
                    return;
                }
            }
            */

            //直接采用队列方式
            if(GetCameraStatusIsRun()==true)
            {
                VmbInt64_t nPLS; // Payload size value
                FeaturePtr pFeature; // Generic feature pointer

                    char *pId;
                    QByteArray ba=NowCameraId.toLatin1();
                    pId=ba.data();

                qDebug()<<*pId;
                //估计原因不能打开相机线程里面明天继续看这原因，已解决，相机打开成功原因id格式不对
                err=GetNowSystem() . OpenCameraByID(pId,VmbAccessModeFull,ThreadCamera);
                if(VmbErrorSuccess == err)//ThreadCamera是引用值
                {
                    qDebug()<<"camera open success";
                    //设置数据包
                    FeaturePtr pCommandFeature;//命令
                    if(VmbErrorSuccess==ThreadCamera->GetFeatureByName("GVSPAdjustPacketSize",pCommandFeature))//设置最大数据包，当连接两台相机后，选择是否改变
                    {
                        pCommandFeature->RunCommand();
                        //未检验是否成功
                    }
                    err = SetValueIntMod2( ThreadCamera,"Width", m_nWidth );//width
                    err = SetValueIntMod2( ThreadCamera,"Height", m_nHeight );//height
                    if(err==VmbErrorSuccess)
                    {
                        FeaturePtr pFormatFeature;
                        ThreadCamera->GetFeatureByName("PixelFormat",pFormatFeature);
                        pFormatFeature->GetValue(m_nPixelFormat);
                        //设置格式、帧大小
                        ThreadCamera ->GetFeatureByName("PayloadSize", pFeature);
                        pFeature ->GetValue(nPLS);//大小
                        setFrameSize(nPLS);

                        //单个帧
                        FramePtr frame;
                        frame.reset( new Frame( FrameSize ));
                        err= ThreadCamera -> StartCapture();
                        err = ThreadCamera -> QueueFrame(frame);

                        //初始多个帧队列
//                        FramePtrVector frames( 15 );
//                        for ( FramePtrVector::iterator iter = frames.begin() ; frames.end() != iter ; ++iter )
//                        {
//                            ( *iter ).reset( new Frame( FrameSize ) );
//                            ThreadCamera->AnnounceFrame(*iter);
//                        }
//                        err = ThreadCamera->StartCapture();
//                        for ( FramePtrVector::iterator iter = frames.begin();frames.end() != iter;++iter )
//                        {
//                            err=ThreadCamera -> QueueFrame(*iter);
//                        }
                        err = ThreadCamera -> GetFeatureByName( "AcquisitionStart", pFeature );
                        err = pFeature ->RunCommand();
                        while(1)
                        {
                            //注意目前图像处理函数在子线程循环里面，速度慢，目前只有一帧，如果需要提高将其传到主程序中排队处理，
                            if(CameraStatusIsRun == false)
                            {

                                return ;
                            }
                            bool bQueueDirectly = false;
                            VmbFrameStatusType eReceiveStatus;
                            //qDebug()<<"signal";

//                            while(frame -> GetReceiveStatus( eReceiveStatus ) !=0)
//                            {
//                                qDebug()<<eReceiveStatus;
//                                //thread_FramesMutex.lock();
//                                sleep(10);
//                                thread_Frames.push( frame );
//                                qDebug()<<"push frame " << QThread::currentThreadId();
//                                if( eReceiveStatus != 0)
//                                    qDebug()<<"no receive frame: "<<eReceiveStatus;
//                                const VmbUchar_t* pBuffer;
//                                err = frame->GetImage(pBuffer);
//                                if(err == VmbErrorSuccess)
//                                    qDebug() << "data " << *pBuffer <<QThread::currentThreadId();

//                                //thread_FramesMutex.unlock();
//                                emit FrameReceivedSignal( eReceiveStatus );//emit singal
//                                bQueueDirectly = true;
//                            }

                            if(VmbErrorSuccess == frame ->GetReceiveStatus( eReceiveStatus ))
                            {
                                qDebug()<<eReceiveStatus;
                                thread_FramesMutex.lock();
                                sleep(2);
                                thread_Frames.push( frame );
                                qDebug()<<"push frame " << QThread::currentThreadId();
                                if( eReceiveStatus != 0)
                                    qDebug()<<"no receive frame: "<<eReceiveStatus;
                                const VmbUchar_t* pBuffer;
                                err = frame->GetImage(pBuffer);
                                if(err == VmbErrorSuccess)
                                    qDebug() << "data " << *pBuffer <<QThread::currentThreadId();

                                thread_FramesMutex.unlock();
                                emit FrameReceivedSignal( eReceiveStatus );//emit singal
                                bQueueDirectly = true;
                            }

                            if( bQueueDirectly == false )
                            {
                                ThreadCamera -> QueueFrame( frame );
                            }
                            else
                            {
                                DoNowFrame(frame,eReceiveStatus);
                            }

                        }
                        StopNowCamera();
                    }
                }
            }
        qDebug()<<"can quit";
        //exec();//exec会让线程卡在这句话上，不会往下执行（除非调用exit或quit
        }

}
VmbInt64_t CameraThread::setFrameSize(VmbInt64_t nPLS)
{
    FrameSize=nPLS;
    return  FrameSize;
}
bool CameraThread::GetCameraStatusIsRun()
{
    return CameraStatusIsRun;
}
void CameraThread::SetCameraStatusIsRun(bool sta)
{
    CameraStatusIsRun=sta;
}
VmbErrorType CameraThread :: CopyToImage(VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, QImage &pOutImage, const float *Matrix)//pOutImage采用引用，用于图像转换
{
    VmbError_t Result;
    const int nHeight=GetHeight();//高度
    const int nWidth=GetWidth();//宽度
    VmbImage SourceImage;//原图像
    VmbImage DestImage;//转换后图像
    SourceImage.Size=sizeof (SourceImage);
    DestImage.Size=sizeof (DestImage);
    Result=VmbSetImageInfoFromPixelFormat( ePixelFormat, nWidth, nHeight, & SourceImage );//从像素格式设置SourceImgae中的图像信息成员值
    if(VmbErrorSuccess!=Result)//是否设置成功？
    {
        qDebug() << "Could not set source image info", static_cast<VmbErrorType>(Result);//static_cast<>强制转换类型
        return static_cast<VmbErrorType>(Result);
    }
    QString OutputFormat;
    const int bytes_per_line=pOutImage.bytesPerLine();//这里pOutImage就相当于是m_Image，因为使用的引用
    switch(pOutImage.format())//查询输出格式
    {
    default:
        qDebug() <<  "unknown output format",VmbErrorBadParameter;//无法识别格式
        return VmbErrorWrongType;
    case QImage::Format_RGB888:
        if(nWidth*3!=bytes_per_line)
        {
            qDebug() << "image transform does not support stride";
            return VmbErrorWrongType;
        }
        OutputFormat = "RGB24";
        break;
    }
    //在字符串中设置VmbImage中的图像信息成员值。
    Result = VmbSetImageInfoFromString(OutputFormat.toStdString().c_str(),OutputFormat.length() , nWidth , nHeight , &DestImage);
    if(VmbErrorSuccess != Result)//DeseImage格式没有设置对
    {
        qDebug() << "could not set output image info",static_cast<VmbErrorType>(Result);
        return static_cast<VmbErrorType>(Result);
    }
    SourceImage.Data=pInBuffer;//原图像从缓冲区获取图像数据
    DestImage.Data=pOutImage.bits();//输出图像和DestImage绑定
    if( NULL != Matrix )//采用颜色转换矩阵，实验为黑白未使用此功能
    {
        VmbTransformInfo TransformParameter;
        Result = VmbSetColorCorrectionMatrix3x3( Matrix, &TransformParameter );//
        if( VmbErrorSuccess == Result )
        {
            Result = VmbImageTransform( &SourceImage, &DestImage, &TransformParameter,1 );
        }
        else
        {
            qDebug() <<  "could not set matrix t o transform info " ;
            return static_cast<VmbErrorType>( Result );
        }
    }
    else
    {
        Result = VmbImageTransform( &SourceImage, &DestImage,NULL,0 );//转换
    }
    if( VmbErrorSuccess != Result )
    {
        //转换未成功
        qDebug() << "could not transform image";
        return static_cast<VmbErrorType>( Result );
    }
    return static_cast<VmbErrorType>(Result);//返回错误信息
}
//直接处理
void CameraThread::DoNowFrame(FramePtr d_frame,int status)
{

    NowImage = QImage( GetWidth(),GetHeight(),QImage::Format_RGB888);
    if(SP_ISNULL(d_frame))//没有获取到帧
    {
        qDebug()<<"Left frame pointer is NULL, late frame ready message";
        return;
    }
    if( VmbFrameStatusComplete == status )//完全帧
    {
        qDebug()<<"NowOnFrameReady"<<QThread::currentThreadId();
        VmbUchar_t *pBuffer;
        VmbErrorType err = d_frame->GetImage(pBuffer);//获取帧图像数据，没有标识位，全图像数据
        if(VmbErrorSuccess==err)//缓冲区数据获取成功
        {
            VmbUint32_t nSize;
            d_frame->GetImageSize( nSize );//图像大小
            VmbPixelFormatType ePixelFormat = GetPixelFormat();//获取相机目前图像格式


            if( ! NowImage.isNull() )//我们需要这个，因为Qt可能在我们已经释放帧之后重新绘制视图
            {
                VmbErrorType err = CopyToImage(pBuffer,ePixelFormat, NowImage);
                emit QimageIsReady(err);
            }
        }
        else {
            qDebug()<< "Left Failure in receiving image";
        }
        //qDebug()<<"success";

        QueueFrame(d_frame);
    }
}

void CameraThread::NowOnFrameReady( int status )//左边相机帧处理
{
    qDebug()<<"NowOnFrameReady"<<QThread::currentThreadId();
    NowImage = QImage( GetWidth(),GetHeight(),QImage::Format_RGB888);
    FramePtr s_frame;
    s_frame=GetFrame();//获取帧
    if(SP_ISNULL(s_frame))//没有获取到帧
    {
        qDebug()<<"Left frame pointer is NULL, late frame ready message";
        return;
    }
    if( VmbFrameStatusComplete == status )//完全帧
    {

        VmbUchar_t *pBuffer;
        VmbErrorType err = s_frame->GetImage(pBuffer);//获取帧图像数据，没有标识位，全图像数据
        if(VmbErrorSuccess==err)//缓冲区数据获取成功
        {
            VmbUint32_t nSize;
            s_frame->GetImageSize( nSize );//图像大小
            VmbPixelFormatType ePixelFormat = GetPixelFormat();//获取相机目前图像格式


            if( ! NowImage.isNull() )//我们需要这个，因为Qt可能在我们已经释放帧之后重新绘制视图
            {
                VmbErrorType err = CopyToImage(pBuffer,ePixelFormat, NowImage);
                emit QimageIsReady(err);
            }
        }
        else {
            qDebug()<< "Left Failure in receiving image";
        }
        //qDebug()<<"success";

        QueueFrame(s_frame);
    }
}
