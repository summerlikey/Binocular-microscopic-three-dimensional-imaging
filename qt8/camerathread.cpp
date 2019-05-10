#include "camerathread.h"
#include<stdio.h>
#include<QByteArray>
#include<QDebug>
#include "VmbTransform.h"
using namespace AVT::VmbAPI;
enum    { NUM_FRAMES=3, };//帧队列的个数
CameraThread::CameraThread()
{

    creatnumber++;
    qDebug()<<"cameraThread "<<creatnumber<<QThread::currentThreadId();
    //QObject :: connect(&timer,SIGNAL(timeout()),this,SLOT(slot1()));
    //QObject :: connect(GetFrameObserver(), SIGNAL(FrameReceivedSignal(int)),this,SLOT(LeftOnFrameReady(int)));

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

void CameraThread::setNowCameraId(VimbaSystem &system,QString CameraId)
{
    char *pId;
    QByteArray ba=CameraId.toLatin1();
    pId=ba.data();
    NowCameraId = pId;
}



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

void CameraThread::StopNowCamera()
{
    FeaturePtr pFeature; // Generic feature pointer
    VmbErrorType err;
    err = ThreadCamera->GetFeatureByName( "AcquisitionStop", pFeature );
    pFeature ->RunCommand();
    ThreadCamera -> EndCapture();
    ClearFrameQueue();
    ThreadCamera -> RevokeAllFrames();

    ThreadCamera->Close();

}

int CameraThread::creatnumber=0;//两种方法初始化静态数据成员int类型

void CameraThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    camerathread_canrun =false;
}

void CameraThread::run()
{



    qDebug()<<"cameraThread  run() 111222"<<creatnumber<<"run"<<currentThreadId();
    if(GetCameraStatus()==true)
    {
        FramePtrVector frames( 3 );
        VmbErrorType err;
        FeaturePtr pFeature; // Any camera feature
        for ( FramePtrVector::iterator iter = frames.begin();frames.end() != iter;++iter )
        {
            ( *iter ).reset( new Frame( frameSize ) );
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

        QObject::connect(GetFrameObserver(), SIGNAL(FrameReceivedSignal(int)),this,SLOT(NowOnFrameReady(int)),Qt::DirectConnection);

    qDebug()<<"can quit";
    exec();//exec会让线程卡在这句话上，不会往下执行（除非调用exit或quit
    }
}
VmbInt64_t CameraThread::setFrameSize(VmbInt64_t nPLS)
{
    frameSize=nPLS;
    return  frameSize;
}
bool CameraThread::GetCameraStatus()
{
    return CameraisRun;
}
void CameraThread::SetCameraStatus(bool sta)
{
    CameraisRun=sta;
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
            qDebug() << "image transform does not support stride",VmbErrorWrongType ;
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
            qDebug() <<  "could not set matrix t o transform info ", static_cast<VmbErrorType>( Result ) ;
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
        qDebug() << "could not transform image", static_cast<VmbErrorType>(Result);
        return static_cast<VmbErrorType>( Result );
    }


    return static_cast<VmbErrorType>(Result);//返回错误信息
}

void CameraThread::NowOnFrameReady( int status )//左边相机帧处理
{
    qDebug()<<"camera Frameon ready"<<creatnumber<<"run"<<currentThreadId();;
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
            // Display it

            //Log("Left Camera display");
            }
        }
    else {
        qDebug()<< "Left Failure in receiving image", VmbErrorOther ;
    }
    qDebug()<<"success";
    QueueFrame(s_frame);
    }
}
