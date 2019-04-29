#include "frameimage.h"
#include "ui_frameimage.h"
#include "VmbTransform.h"
frameimage::frameimage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frameimage),m_bIsStreaming(false)
{
    ui->setupUi(this);
    VmbErrorType err=m_api.ApiStartUp();
    Log("Vimbaopen",err);
    QObject::connect(ui->m_ButtonStartStop,SIGNAL(clicked()), this,SLOT( OnBnClickedButtonStartstop()));
    QObject::connect(m_api.GetCameraObserver(),SIGNAL(CameraListChangedSignal(int)),this,SLOT( OnCameraListChanged(int)));
    UpdateCameraListBox();
}

frameimage::~frameimage()
{
    delete ui;
    m_api.ApiShutDown();
}
void frameimage::OnBnClickedButtonStartstop()
{
    VmbErrorType err;
    int nRow=ui->m_ListBoxCameras->currentRow();//选择相机，在listwidget里面，利用数组
    if(nRow>-1)
    {
        if(false==m_bIsStreaming)
        {
            err=m_api.StartContinuousImageAcquisition(m_cameras[nRow]);
            if(VmbErrorSuccess==err)
            {
                m_Image = QImage( m_api.GetWidth(),m_api.GetHeight(),QImage::Format_RGB888 );
                QObject::connect( m_api.GetFrameObserver(), SIGNAL( FrameReceivedSignal(int) ), this, SLOT( OnFrameReady(int) ));
            }
            m_bIsStreaming = VmbErrorSuccess == err;
            Log("Start Acquisition");
        }
        else
        {
            m_bIsStreaming = false;
            err = m_api.StopContinuousImageAcquisition();
            m_api.ClearFrameQueue();
            m_Image = QImage();
            Log("Stop Acquisition");
        }
    }

    if( false == m_bIsStreaming )
    {
        ui->m_ButtonStartStop->setText( QString("Start Image Acquisition" ));
    }
    else
    {
        ui->m_ButtonStartStop->setText( QString("Stop Image Acquisition" ));
    }
}

void frameimage::OnFrameReady( int status )
{
    if(true==m_bIsStreaming)
    {
        FramePtr s_frame;
        s_frame=m_api.GetFrame();//获取帧
        if(SP_ISNULL(s_frame))//没有获取到帧
        {
            Log("frame pointer is NULL, late frame ready message");
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
                VmbPixelFormatType ePixelFormat = m_api.GetPixelFormat();//获取相机目前图像格式


                if( ! m_Image.isNull() )//我们需要这个，因为Qt可能在我们已经释放帧之后重新绘制视图
                {
                    CopyToImage(pBuffer,ePixelFormat, m_Image);
                }
                // Display it
                const QSize s = ui->m_LeftLabelStream->size() ;
                ui->m_LeftLabelStream->setPixmap(QPixmap::fromImage(m_Image).scaled(s,Qt::KeepAspectRatio));//自适应QLabel的大小
            }
        }
        else {
            Log( "Failure in receiving image", VmbErrorOther );
        }
        m_api.QueueFrame(s_frame);
    }
}

VmbErrorType frameimage::CopyToImage(VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, QImage &pOutImage, const float *Matrix)//pOutImage采用引用
{
    VmbError_t Result;
    const int nHeight=m_api.GetHeight();//高度
    const int nWidth=m_api.GetWidth();//宽度
    VmbImage SourceImage;//原图像
    VmbImage DestImage;//转换后图像
    SourceImage.Size=sizeof (SourceImage);
    DestImage.Size=sizeof (DestImage);
    Result=VmbSetImageInfoFromPixelFormat( ePixelFormat, nWidth, nHeight, & SourceImage );//从像素格式设置SourceImgae中的图像信息成员值
    if(VmbErrorSuccess!=Result)//是否设置成功？
    {
        Log("Could not set source image info", static_cast<VmbErrorType>(Result));//static_cast<>强制转换类型
        return static_cast<VmbErrorType>(Result);
    }
    QString OutputFormat;
    const int bytes_per_line=pOutImage.bytesPerLine();//这里pOutImage就相当于是m_Image，因为使用的引用
    switch(pOutImage.format())//查询输出格式
    {
    default:
        Log( "unknown output format",VmbErrorBadParameter);//无法识别格式
        return VmbErrorWrongType;
    case QImage::Format_RGB888:
        if(nWidth*3!=bytes_per_line)
        {
            Log( "image transform does not support stride",VmbErrorWrongType );
            return VmbErrorWrongType;
        }
        OutputFormat = "RGB24";
        break;
    }
    //在字符串中设置VmbImage中的图像信息成员值。
    Result = VmbSetImageInfoFromString(OutputFormat.toStdString().c_str(),OutputFormat.length(),nWidth,nHeight, &DestImage);
    if(VmbErrorSuccess != Result)//DeseImage格式没有设置对
    {
        Log("could not set output image info",static_cast<VmbErrorType>(Result));
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
            Log( "could not set matrix t o transform info ", static_cast<VmbErrorType>( Result ) );
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
        Log( "could not transform image", static_cast<VmbErrorType>(Result));
        return static_cast<VmbErrorType>( Result );
    }


    return static_cast<VmbErrorType>(Result);//返回错误信息
}


void frameimage::OnCameraListChanged(int reason)
{
    bool bUpdateList = false;
    // We only react on new cameras being found and known cameras being unplugged
    if( AVT::VmbAPI::UpdateTriggerPluggedIn == reason )//0，a new camera is find
    {
        bUpdateList = true;
    }
    else if( AVT::VmbAPI::UpdateTriggerPluggedOut == reason )//1,a camera is disappeared
    {
        bUpdateList = true;
    }

    if( true == bUpdateList )
    {
        UpdateCameraListBox();//当相机变化更新盒子
    }
}
void frameimage::UpdateCameraListBox()
{
    // Get all cameras currently connected to Vimba
    CameraPtrVector cameras = m_api.GetCameraList();

    // Simply forget about all cameras known so far
    ui->m_ListBoxCameras->clear();
    m_cameras.clear();

    // And query the camera details again
    for(    CameraPtrVector::const_iterator iter = cameras.begin();
            cameras.end() != iter;
            ++iter )
    {
        std::string strCameraName;
        std::string strCameraID;
        if( VmbErrorSuccess != (*iter)->GetName(strCameraName))
        {
            strCameraName = "[NoName]";
        }
        // If for any reason we cannot get the ID of a camera we skip it
        if(VmbErrorSuccess == (*iter)->GetID(strCameraID))
        {
            ui->m_ListBoxCameras->addItem(QString::fromStdString(strCameraName + " " +strCameraID));
            m_cameras.push_back(strCameraID);
        }
    }
}

void frameimage::Log(std::string strMsg, VmbErrorType eErr)
{
    strMsg += "..."+ m_api.ErrorCodeToMessages(eErr);
    ui->m_ListLog->insertItem(0, QString::fromStdString(strMsg));
}
void frameimage::Log( std::string strMsg)
{
    ui->m_ListLog->insertItem( 0, QString::fromStdString( strMsg ) );
}
