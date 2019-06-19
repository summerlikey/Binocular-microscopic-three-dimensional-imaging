#include "cameramainwindow.h"
#include "ui_cameramainwindow.h"
#include <sstream>
#include <QString>
#include<QDebug>
#include "VmbTransform.h"
#include "VmbTransformTypes.h"
#include <QLineEdit>
#include <QAbstractSlider>
CameraMainWindow::CameraMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraMainWindow),LeftIsStream(false),RightIsStream(false)

{

    ui->setupUi(this);
    Image_LC.load(":/image/background");
    Image_RC.load(":/image/background");
    Image_SI.load(":/image/background");
    ui->Lable_LeftStream->setScaledContents(true);
    ui->Lable_RightStream->setScaledContents(true);
    ui->Label_ShowStream->setScaledContents(true);
    const QSize l = ui->Lable_LeftStream->size();
    ui->Lable_LeftStream->setPixmap(QPixmap::fromImage(Image_LC).scaled(l,Qt::KeepAspectRatio));//自适应QLabel的大小
    const QSize r = ui->Lable_RightStream->size();
    ui->Lable_RightStream->setPixmap(QPixmap::fromImage(Image_RC).scaled(r,Qt::KeepAspectRatio));//自适应QLabel的大小
    const QSize s = ui->Label_ShowStream->size();
    ui->Label_ShowStream->setPixmap(QPixmap::fromImage(Image_SI).scaled(s,Qt::KeepAspectRatio));//自适应QLabel的大小

    this->ui-> Label_LeftCameraLightStatus ->setPixmap(QPixmap(":/image/red").scaled(15,15));//左边相机状态灯
    this->ui-> Label_RightCameraLightStatus ->setPixmap(QPixmap(":/image/red").scaled(15,15));//右边相机状态灯
    this->ui-> Label_ProjectorLightStatus ->setPixmap(QPixmap(":/image/red").scaled(15,15));//右边相机状态灯

    SetChangeExposure();//the sxposure change

    VmbErrorType err =  NowApiController.StartUp();

    Log("Vimba start",err);
    if( VmbErrorSuccess == err )
    {
        // Connect new camera found event with event handler
        QObject::connect( NowApiController.GetCameraObeserver(), SIGNAL( CameraListChangedSignal( int ) ), this, SLOT( OnCameraListChanged( int ) ) );

        // Initially get all connected cameras
        UpdateCameraListBox();
        std::stringstream strMsg;
        strMsg << "Cameras found..." << AllCamerasId.size();
        Log(strMsg.str() );
    }

    QObject::connect( ui -> Btn_TwoCameraStartStop , SIGNAL( clicked() ), this, SLOT( OnClickedBtn_TwoStartStop() ) );

    QObject::connect( ui -> Btn_LeftCameraStartStop , SIGNAL( clicked() ), this, SLOT( OnCLickedBtn_LeftCameraStartStop() ) );

    QObject::connect( ui -> Btn_RightCameraStartStop , SIGNAL( clicked() ), this, SLOT( OnCLickedBtn_RightCameraStartStop() ) );


}
void CameraMainWindow::OnCameraListChanged(int reason)
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
    ui->Btn_TwoCameraStartStop->setEnabled( 0 < AllCamerasId.size() || ( LeftIsStream || RightIsStream ) );
}

void CameraMainWindow::SetChangeExposure()
{
    //slider**********
    int ndefaults=150000;
    int nmin=14;
    int nmax=891072;
    int nSingleStep = 100;
    this ->ui ->HS_LeftExposure ->setRange(14,891072);
    this ->ui ->HS_LeftExposure ->setSingleStep(nSingleStep);
    this ->ui ->SB_LeftExposure ->setMinimum(nmin);
    this ->ui ->SB_LeftExposure ->setMaximum(nmax);
    this ->ui ->SB_LeftExposure ->setSingleStep(nSingleStep);
    this ->ui ->HS_LeftExposure ->setValue(ndefaults);
    this ->ui ->SB_LeftExposure ->setValue(ndefaults);

    connect(this->ui->SB_LeftExposure,SIGNAL(valueChanged(int)),this->ui->HS_LeftExposure,SLOT(setValue(int)));
    connect(this->ui->HS_LeftExposure,SIGNAL(valueChanged(int)),this->ui->SB_LeftExposure,SLOT(setValue(int)));

    connect(this ->ui ->HS_LeftExposure,SIGNAL(valueChanged(int)),this ,SLOT(setLeftExposure(int)));


    this ->ui ->HS_RightExposure ->setRange(14,891072);
    this ->ui ->HS_RightExposure ->setSingleStep(nSingleStep);
    this ->ui ->SB_RightExposure ->setMinimum(nmin);
    this ->ui ->SB_RightExposure ->setMaximum(nmax);
    this ->ui ->SB_RightExposure ->setSingleStep(nSingleStep);
    this ->ui ->HS_RightExposure ->setValue(ndefaults);
    this ->ui ->SB_RightExposure ->setValue(ndefaults);

    connect(this->ui->SB_RightExposure,SIGNAL(valueChanged(int)),this->ui->HS_RightExposure,SLOT(setValue(int)));
    connect(this->ui->HS_RightExposure,SIGNAL(valueChanged(int)),this->ui->SB_RightExposure,SLOT(setValue(int)));

    connect(this ->ui ->HS_RightExposure,SIGNAL(valueChanged(int)),this ,SLOT(setRightExposure(int)));
    //***********
}

void CameraMainWindow::setLeftExposure(int value)
{
    if(LeftIsStream == false)
        return;
    VmbErrorType err = NowApiController.SetApiLeftExposure(value);
    //Log("LeftExposure set",err);
}

void CameraMainWindow::setRightExposure(int value)
{
    if(RightIsStream ==false)
        return;

    VmbErrorType err = NowApiController.SetApiRightExposure(value);
    //Log("RightExposure set",err);
}

void CameraMainWindow::UpdateCameraListBox()
{
    // Get all cameras currently connected to Vimba
    CameraPtrVector  cameras = NowApiController.GetCameraList();

    // Simply forget about all cameras known so far
    ui->LW_Cameras->clear();
    AllCamerasId.clear();  //vector of string for cameras..
    this->ui-> Label_LeftCameraLightStatus -> setPixmap(QPixmap(":/image/red").scaled(15,15));//左边相机状weibo态灯
    this->ui-> Label_RightCameraLightStatus -> setPixmap(QPixmap(":/image/red").scaled(15,15));//左边相机状态灯

    // And query the camera details again
    for(    CameraPtrVector::const_iterator iter = cameras.begin();
            cameras.end() != iter;
            ++iter )
    {
        std::string strCameraName;
        std::string strCameraID;
        if( VmbErrorSuccess != (*iter)->GetName( strCameraName ) )
        {
            strCameraName = "[NoName]";
        }
        // If for any reason we cannot get the ID of a camera we skip it
        if( VmbErrorSuccess == (*iter)->GetID( strCameraID ) )
        {
            ui->LW_Cameras->addItem( QString::fromStdString( strCameraName + " " +strCameraID ) );
            AllCamerasId.push_back( strCameraID );
            if( LeftCameraId == QString::fromStdString(strCameraID))
            {
                //Log("Left camera is found");

                this->ui-> Label_LeftCameraLightStatus ->setPixmap(QPixmap(":/image/green").scaled(15,15));//左边相机状态灯
            }
            if( RightCameraId == QString::fromStdString(strCameraID))
            {
                //Log("Right camera is found");

                this -> ui ->Label_RightCameraLightStatus ->setPixmap(QPixmap(":image/green").scaled(15,15));
            }
        }
    }
    ui->Btn_TwoCameraStartStop->setEnabled( 0 < AllCamerasId.size() || ( LeftIsStream || RightIsStream ) );
}

void CameraMainWindow::OnCLickedBtn_LeftCameraStartStop()
{
    VmbErrorType err;
    int nRow = ui->LW_Cameras -> count();
    if(nRow>0)
    {
        if(false ==LeftIsStream)
        {
            int pixel_format = 0;//mono8 mark
            err = NowApiController.StartContinuousAcquisitionOfLeftCameras(AllCamerasId[0],pixel_format);

            //set qt image
            if(VmbErrorSuccess == err )
            {
                LeftQImage = QImage( NowApiController.GetLeftWidth(),
                                  NowApiController.GetLeftHeight(),
                                  QImage::Format_Grayscale8 );
                //start multiple acquisition frame observers -> number of cameras...
                QObject::connect( NowApiController.GetLeftFrameObserver(), SIGNAL( FrameReceivedSignal(int, QString) ), this, SLOT( OnLeftFrameReady(int, QString) ) );
            }
            Log( "Left Starting Acquisition", err );
            LeftIsStream = VmbErrorSuccess == err;
        }
        else
        {
            LeftIsStream = false;
            //RightIsStream = false;
            // Stop acquisition
            err = NowApiController.StopLeftContinuousImageAcquisition();
            // Clear all frames that we have not picked up so far
            NowApiController.ClearLeftFrameQueue();
            LeftQImage = QImage();
            Log( "Stopping Acquisition", err );
        }
        if( false == LeftIsStream)
        {
            ui->Btn_LeftCameraStartStop -> setText( QString( "Start Image Acquisition" ) );
        }
        else
        {
            ui->Btn_LeftCameraStartStop -> setText( QString( "Stop Image Acquisition" ) );
        }
    }
}
void CameraMainWindow::OnCLickedBtn_RightCameraStartStop()
{
    VmbErrorType err;
    int nRow = ui->LW_Cameras -> count();
    if(nRow>0)
    {
        if(false ==RightIsStream)
        {
            int pixel_format = 0;//mono8 mark
            err = NowApiController.StartContinuousAcquisitionOfRightCameras(AllCamerasId[1],pixel_format);

            //set qt image
            if(VmbErrorSuccess == err )
            {
                RightQImage = QImage( NowApiController.GetRightWidth(),
                                  NowApiController.GetRightHeight(),
                                  QImage::Format_Grayscale8 );
                //start multiple acquisition frame observers -> number of cameras...

                QObject::connect( NowApiController.GetRightFrameObserver(), SIGNAL( FrameReceivedSignal(int, QString) ), this, SLOT( OnRightFrameReady(int, QString) ) );
            }
            Log( "Right Starting Acquisition", err );
            RightIsStream = VmbErrorSuccess == err;
        }
        else
        {
            RightIsStream = false;
            //RightIsStream = false;
            // Stop acquisition
            err = NowApiController.StopRightContinuousImageAcquisition();
            // Clear all frames that we have not picked up so far
            NowApiController.ClearRightFrameQueue();
            RightQImage = QImage();
            Log( "Stopping Acquisition", err );
        }
        if( false == RightIsStream)
        {
            ui->Btn_RightCameraStartStop -> setText( QString( "Start Image Acquisition" ) );
        }
        else
        {
            ui->Btn_RightCameraStartStop -> setText( QString( "Stop Image Acquisition" ) );
        }
    }
}

void CameraMainWindow::OnClickedBtn_TwoStartStop()
{

    VmbErrorType err;
    int nRow = ui->LW_Cameras -> count();
    if(nRow==2)
    {
        if(( false == LeftIsStream ) && (false == RightIsStream ) )
        {
            // Start acquisition
            int pixel_format = 0; //mono8
            err = NowApiController.StartContinuousAcquisitionOfTwoCameras(AllCamerasId[0], pixel_format, AllCamerasId[1], pixel_format);


            // Set up Qt image
            if ( VmbErrorSuccess == err )
            {
                LeftQImage = QImage( NowApiController.GetLeftWidth(),
                                  NowApiController.GetLeftHeight(),
                                  QImage::Format_Grayscale8 );

                RightQImage = QImage( NowApiController.GetRightWidth(),
                                  NowApiController.GetRightHeight(),
                                  QImage::Format_Grayscale8 );

                //start multiple acquisition frame observers -> number of cameras...
                QObject::connect( NowApiController.GetLeftFrameObserver(), SIGNAL( FrameReceivedSignal(int, QString) ), this, SLOT( OnLeftFrameReady(int, QString) ) );
                QObject::connect( NowApiController.GetRightFrameObserver(), SIGNAL( FrameReceivedSignal(int, QString) ), this, SLOT( OnRightFrameReady(int, QString) ) );

            }
            Log( "Starting Acquisition", err );
            LeftIsStream = VmbErrorSuccess == err;
            RightIsStream = VmbErrorSuccess == err;

        }
        else
        {
            LeftIsStream = false;
            RightIsStream = false;
            // Stop acquisition
            err = NowApiController.StopContinuousImageAcquisition();
            // Clear all frames that we have not picked up so far
            NowApiController.ClearFrameQueue();
            LeftQImage = QImage();
            RightQImage = QImage();
            Log( "Stopping Acquisition", err );
        }

        if( (false == LeftIsStream) || (false == RightIsStream) )
        {
            ui->Btn_TwoCameraStartStop -> setText( QString( "Start Image Acquisition" ) );
        }
        else
        {
            ui->Btn_TwoCameraStartStop -> setText( QString( "Stop Image Acquisition" ) );
        }
    }
}

void CameraMainWindow::OnLeftFrameReady(int status, QString camera )
{
    std::string cameraname = camera.toStdString();

    if( true == LeftIsStream)
    {
        // Pick up frame
        FramePtr pFrame = NowApiController.GetLeftFrame();
        if( SP_ISNULL( pFrame ) )
        {
            Log("frame pointer is NULL, late frame ready message");
            return;
        }
        // See if it is not corrupt
        if( VmbFrameStatusComplete == status )
        {
            VmbUchar_t *pBuffer;
            VmbErrorType err = SP_ACCESS( pFrame )->GetImage( pBuffer );
            if( VmbErrorSuccess == err )
            {
                VmbUint32_t nSize;
                err = SP_ACCESS( pFrame )->GetImageSize( nSize );
                if( VmbErrorSuccess == err )
                {
                    VmbPixelFormatType ePixelFormat = NowApiController.GetLeftPixelFormat();
                    SP_ACCESS( pFrame )->GetWidth(LeftFrameWidth);
                    SP_ACCESS( pFrame )->GetHeight(LeftFrameHeight);
                    //We don't need VmbImageTransport as we are using OpenCV

                    if( ! LeftQImage.isNull() )
                    {
                        /*
                        // Copy it
                        // We need that because Qt might repaint the view after we have released the frame already
                        if( ui.m_ColorProcessingCheckBox->checkState()==  Qt::Checked )
                        {
                            static const VmbFloat_t Matrix[] = {    8.0f, 0.1f, 0.1f, // this matrix just makes a quick color to mono conversion
                                                                    0.1f, 0.8f, 0.1f,
                                                                    0.0f, 0.0f, 1.0f };

                            if( VmbErrorSuccess != CopyToImage( pBuffer,ePixelFormat, m_ApiController.GetHeight1(), m_ApiController.GetWidth1(), m_Image1, Matrix ) )

                            {
                                ui.m_ColorProcessingCheckBox->setChecked( false );
                            }
                        }
                        else
                        {
                            CopyToImage( pBuffer,ePixelFormat, m_ApiController.GetHeight1(), m_ApiController.GetWidth1(), m_Image1, NULL );
                        }
                        */
                        CopyToImage( pBuffer,ePixelFormat, NowApiController.GetLeftHeightVmb32(), NowApiController.GetLeftWidthVmb32(), LeftQImage, NULL );

                        // Display it
                        const QSize s = ui->Lable_LeftStream ->size() ;
                        ui->Lable_LeftStream->setPixmap( QPixmap::fromImage( LeftQImage ).scaled(s,Qt::KeepAspectRatio ) );
                    }
                }
            }
        }
        else
        {
            // If we receive an incomplete image we do nothing but logging
            Log( "Failure in receiving image ss", VmbErrorOther );
        }

        // And queue it to continue streaming
        NowApiController.QueueLeftFrame( pFrame );
    }
}

void CameraMainWindow::OnRightFrameReady(int status, QString camera )
{
    std::string cameraname = camera.toStdString();
    if( true == RightIsStream)
    {
        // Pick up frame
        FramePtr pFrame = NowApiController.GetRightFrame();
        if( SP_ISNULL( pFrame ) )
        {
            Log("frame pointer is NULL, late frame ready message");
            return;
        }
        // See if it is not corrupt
        if( VmbFrameStatusComplete == status )
        {
            VmbUchar_t *pBuffer;
            VmbErrorType err = SP_ACCESS( pFrame )->GetImage( pBuffer );
            if( VmbErrorSuccess == err )
            {
                VmbUint32_t nSize;
                err = SP_ACCESS( pFrame )->GetImageSize( nSize );
                if( VmbErrorSuccess == err )
                {
                    VmbPixelFormatType ePixelFormat = NowApiController.GetRightPixelFormat();
                    SP_ACCESS( pFrame )->GetWidth(RightFrameWidth);
                    SP_ACCESS( pFrame )->GetHeight(RightFrameHeight);
                    //We don't need VmbImageTransport as we are using OpenCV

                    if( ! RightQImage.isNull() )
                    {
                        /*
                        // Copy it
                        // We need that because Qt might repaint the view after we have released the frame already
                        if( ui.m_ColorProcessingCheckBox->checkState()==  Qt::Checked )
                        {
                            static const VmbFloat_t Matrix[] = {    8.0f, 0.1f, 0.1f, // this matrix just makes a quick color to mono conversion
                                                                    0.1f, 0.8f, 0.1f,
                                                                    0.0f, 0.0f, 1.0f };

                            if( VmbErrorSuccess != CopyToImage( pBuffer,ePixelFormat, m_ApiController.GetHeight1(), m_ApiController.GetWidth1(), m_Image1, Matrix ) )

                            {
                                ui.m_ColorProcessingCheckBox->setChecked( false );
                            }
                        }
                        else
                        {
                            CopyToImage( pBuffer,ePixelFormat, m_ApiController.GetHeight1(), m_ApiController.GetWidth1(), m_Image1, NULL );
                        }
                        */
                        CopyToImage( pBuffer,ePixelFormat, NowApiController.GetRightHeightVmb32(), NowApiController.GetRightWidthVmb32(), RightQImage, 0 );

                        // Display it
                        const QSize s = ui-> Lable_RightStream ->size() ;
                        ui-> Lable_RightStream ->setPixmap( QPixmap::fromImage( RightQImage ).scaled(s,Qt::KeepAspectRatio ) );
                    }
                }
            }
        }
        else
        {
            // If we receive an incomplete image we do nothing but logging
            Log( "Failure in receiving image ss", VmbErrorOther );
        }

        // And queue it to continue streaming
        NowApiController.QueueRightFrame( pFrame );
    }
}

VmbErrorType CameraMainWindow::CopyToImage( VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, VmbUint32_t nHeight, VmbUint32_t nWidth, QImage &pOutImage, const float *Matrix /*= NULL */ )
{
    int iWidth = static_cast<int>(nWidth);
    int iHeight = static_cast<int>(nHeight);

    pOutImage = QImage(pInBuffer,iWidth,iHeight,QImage::Format_Grayscale8, 0 , 0 );

    VmbError_t          Result;
    Result = 0;

    /*
    qDebug()<<*pInBuffer;
    VmbImage SourceImage;
    VmbImage DestImage;

    SourceImage.Size    = sizeof( SourceImage );
    DestImage.Size      = sizeof( DestImage );
    SourceImage.Data    = pInBuffer;
    Result = VmbSetImageInfoFromPixelFormat( ePixelFormat, nWidth, nHeight, & SourceImage );//从像素格式设置SourceImgae中的图像信息成员值
    if( VmbErrorSuccess != Result )
    {
        qDebug()<<"55";
        Log( "Could not set source image info", static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    QString             OutputFormat;
    const int           bytes_per_line = pOutImage.bytesPerLine();
    switch( pOutImage.format() )
    {
    default:
        Log( "unknown output format",VmbErrorBadParameter );
        return VmbErrorBadParameter;
    case QImage::Format_RGB888:
        if( iWidth*3 != bytes_per_line )
        {
            Log( "image transform does not support stride",VmbErrorWrongType );
            return VmbErrorWrongType;
        }
        OutputFormat = "RGB24";
        break;
    case QImage::Format_Grayscale8:
        OutputFormat = "MONO8";
        break;

    }
    VmbUint32_t           stringLength;
    stringLength = static_cast<VmbUint32_t>(OutputFormat.length()) ;
    Result = VmbSetImageInfoFromString( OutputFormat.toStdString().c_str(), stringLength,nWidth,nHeight, &DestImage );
    if( VmbErrorSuccess != Result )
    {
        Log( "could not set output image info",static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    SourceImage.Data    = pInBuffer;
    DestImage.Data      = pOutImage.bits();
    // do color processing?
    if( NULL != Matrix )
    {
        VmbTransformInfo TransformParameter;
        Result = VmbSetColorCorrectionMatrix3x3( Matrix, &TransformParameter );
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
        Result = VmbImageTransform( &SourceImage, &DestImage,NULL,0 );
    }
    */
    if( VmbErrorSuccess != Result )
    {
        Log( "could not transform image", static_cast<VmbErrorType>( Result ) );
        return static_cast<VmbErrorType>( Result );
    }
    return static_cast<VmbErrorType>( Result );
}

// Prints out a given logging string, error code and the descriptive representation of that error code
//
// Parameters:
//  [in]    strMsg          A given message to be printed out
//  [in]    eErr            The API status code
//
void CameraMainWindow::Log( std::string strMsg, VmbErrorType eErr )
{
    strMsg += "..."+ NowApiController.ErrorCodeToMessages(eErr);
    ui->LW_Logs->insertItem(0, QString::fromStdString(strMsg));

}

//
// Prints out a given logging string
//
// Parameters:
//  [in]    strMsg          A given message to be printed out
//
void CameraMainWindow::Log( std::string strMsg)
{
    ui->LW_Logs->insertItem( 0, QString::fromStdString( strMsg ) );
}
CameraMainWindow::~CameraMainWindow()
{
    NowApiController.ShutDown();
    delete ui;
}
