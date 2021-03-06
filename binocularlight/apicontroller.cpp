#include "apicontroller.h"
#include <QDebug>
enum    { NUM_FRAMES=25, };
ApiController::ApiController(): NowSystem( VimbaSystem::GetInstance() )
{

}
ApiController::~ApiController()
{

}

int ApiController::leftexposure=0;//

std::string ApiController::ErrorCodeToMessages(VmbErrorType eErr)
{
    return ErrorCodeToMessage(eErr);
}

VmbErrorType ApiController::StartUp()
{
    VmbErrorType res;
    res = NowSystem.Startup();
    if( VmbErrorSuccess == res )
    {
        // This will be wrapped in a shared_ptr so we don't delete it
        SP_SET( m_pCameraObserver , new CameraObserver() );
        // Register an observer whose callback routine gets triggered whenever a camera is plugged in or out
        res = NowSystem.RegisterCameraListObserver( m_pCameraObserver );
    }
    return res;
}

void ApiController::ShutDown()
{
    NowSystem.UnregisterCameraListObserver(m_pCameraObserver);
    NowSystem.Shutdown();
}

QObject* ApiController::GetCameraObeserver()
{
    return SP_DYN_CAST( m_pCameraObserver, CameraObserver ).get();
}

CameraPtrVector ApiController::GetCameraList()
{
    CameraPtrVector cameras;
    // Get all known cameras
    if( VmbErrorSuccess == NowSystem.GetCameras( cameras ) )
    {
        // And return them
        return cameras;
    }
    return CameraPtrVector();
}
int ApiController::GetLeftWidth() const
{
    return static_cast<int>(LeftWidth);
}
int ApiController::GetRightWidth() const
{
    return static_cast<int>(RightWidth);
}

int ApiController::GetLeftHeight() const
{
    return static_cast<int>(LeftHeight);
}
int ApiController::GetRightHeight() const
{
    return static_cast<int>(RightHeight);
}

VmbUint32_t ApiController::GetLeftWidthVmb32() const
{
    return static_cast<VmbUint32_t>(LeftWidth);
}
VmbUint32_t ApiController::GetRightWidthVmb32() const
{
    return static_cast<VmbUint32_t>(RightWidth);
}

VmbUint32_t ApiController::GetLeftHeightVmb32() const
{
    return static_cast<VmbUint32_t>(LeftHeight);
}
VmbUint32_t ApiController::GetRightHeightVmb32() const
{
    return static_cast<VmbUint32_t>(RightHeight);
}


VmbPixelFormatType ApiController::GetLeftPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(LeftPixelFormat);
}
VmbPixelFormatType ApiController::GetRightPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(RightPixelFormat);
}

FramePtr ApiController::GetLeftFrame()
{
    return SP_DYN_CAST( LeftFrameObserver, FrameObserver )->GetFrame();
}

FramePtr ApiController::GetRightFrame()
{
    return SP_DYN_CAST( RightFrameObserver, FrameObserver )->GetFrame();
}

VmbErrorType ApiController::QueueLeftFrame( FramePtr pFrame )
{
    return SP_ACCESS( LeftCamera )->QueueFrame( pFrame );
}
VmbErrorType ApiController::QueueRightFrame( FramePtr pFrame )
{
    return SP_ACCESS( RightCamera )->QueueFrame( pFrame );
}

VmbErrorType ApiController::SetApiLeftExposure(int exposure_value )
{
    double dex;
    dex = static_cast <double> (exposure_value);
    VmbErrorType res;
    //set manual exposure..
    FeaturePtr pfeature;
    LeftCamera -> GetFeatureByName("ExposureTimeAbs",pfeature);
    res = pfeature -> SetValue(dex);
//    FeaturePtr pExposuremode;
//    res = SP_ACCESS( LeftCamera )->GetFeatureByName( "ExposureMode", pExposuremode );
//    if( VmbErrorSuccess == res )
//        res = SP_ACCESS( pExposuremode )->SetValue ("Off");
//    qDebug()<<res;
//    //set manual exposure supplied as a argument.
//    FeaturePtr pSetExposure;
//    res = SP_ACCESS( LeftCamera )->GetFeatureByName("ExposureTimeAbs", pSetExposure);
//    if( VmbErrorSuccess == res )
//        res = SP_ACCESS( pSetExposure )->SetValue(exposure_value);
    return res;
}


VmbErrorType ApiController::SetApiRightExposure(int exposure_value )
{
    double dex;
    dex = static_cast <double> (exposure_value);
    VmbErrorType res;
    //set manual exposure..
    FeaturePtr pfeature;
    RightCamera -> GetFeatureByName("ExposureTimeAbs",pfeature);
    res = pfeature -> SetValue(dex);
//    FeaturePtr pExposuremode;
//    res = SP_ACCESS( LeftCamera )->GetFeatureByName( "ExposureMode", pExposuremode );
//    if( VmbErrorSuccess == res )
//        res = SP_ACCESS( pExposuremode )->SetValue ("Off");
//    qDebug()<<res;
//    //set manual exposure supplied as a argument.
//    FeaturePtr pSetExposure;
//    res = SP_ACCESS( LeftCamera )->GetFeatureByName("ExposureTimeAbs", pSetExposure);
//    if( VmbErrorSuccess == res )
//        res = SP_ACCESS( pSetExposure )->SetValue(exposure_value);
    return res;
}




VmbErrorType ApiController::StartContinuousAcquisitionOfLeftCameras( const std::string &lStrCameraID ,int pixel_mode)
{
    QString res1;
    res1 = QString::fromStdString(lStrCameraID);
    VmbErrorType res = NowSystem.OpenCameraByID( lStrCameraID.c_str(), VmbAccessModeFull, LeftCamera );
    if( VmbErrorSuccess == res )
    {

        
        FeaturePtr pFeature; // Generic feature pointer
        LeftCamera -> GetFeatureByName("StreamBytesPerSecond",pFeature);

        if(VmbErrorSuccess !=  pFeature -> SetValue(6200000))
        {
            qDebug()<<"left stream bytespersecond not 6200000";
        }
        // Set the GeV packet size to the highest possible value
        // (In this example we do not test whether this cam actually is a GigE cam)
        FeaturePtr pCommandFeature;
        if( VmbErrorSuccess == SP_ACCESS( LeftCamera )->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ) )
        {

            if( VmbErrorSuccess == SP_ACCESS( pCommandFeature )->RunCommand() )
            {
                bool bIsCommandDone = false;
                do
                {
                    if( VmbErrorSuccess != SP_ACCESS( pCommandFeature )->IsCommandDone( bIsCommandDone ) )
                    {
                        break;
                    }
                } while( false == bIsCommandDone );
            }
        }
        FeaturePtr pFormatFeatureCam1;
        res = SP_ACCESS( LeftCamera )->GetFeatureByName( "PixelFormat", pFormatFeatureCam1 );

        if( VmbErrorSuccess == res )
        {
            if (pixel_mode == 0)
            res = SP_ACCESS( pFormatFeatureCam1 )->SetValue( 17301505  );  //mono frame

            else if (pixel_mode == 1)
            res = SP_ACCESS( pFormatFeatureCam1 )->SetValue( 35127317 );  //color frame
        }
        SP_ACCESS( LeftCamera )->GetFeatureByName("Width", pFormatFeatureCam1);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam1 )->GetValue(LeftWidth);
        }

        SP_ACCESS( LeftCamera )->GetFeatureByName("Height", pFormatFeatureCam1);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam1 )->GetValue(LeftHeight);
        }


         // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)
         SP_SET( LeftFrameObserver , new FrameObserver( LeftCamera ) );

    }
    //once the configuration of both cameras is done, then we can start the continuous acquisition of both cameras.
        // Start streaming
        res = SP_ACCESS( LeftCamera )->StartContinuousImageAcquisition( NUM_FRAMES,  LeftFrameObserver );
        return res;
}
VmbErrorType ApiController::StartContinuousAcquisitionOfRightCameras( const std::string &rStrCameraID ,int pixel_mode)
{
    QString res2;
    res2 = QString::fromStdString(rStrCameraID);
    VmbErrorType res = NowSystem.OpenCameraByID( rStrCameraID.c_str(), VmbAccessModeFull, RightCamera );

    if( VmbErrorSuccess == res )
    {

        FeaturePtr pFeature; // Generic feature pointer
        RightCamera -> GetFeatureByName("StreamBytesPerSecond",pFeature);
        if(VmbErrorSuccess !=  pFeature -> SetValue(6200000))
        {
            qDebug()<<"right stream bytespersecond not 6200000";
        }
        // Set the GeV packet size to the highest possible value
        // (In this example we do not test whether this cam actually is a GigE cam)
        FeaturePtr pCommandFeature;
        if( VmbErrorSuccess == SP_ACCESS( RightCamera )->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ) )
        {
            if( VmbErrorSuccess == SP_ACCESS( pCommandFeature )->RunCommand() )
            {
                bool bIsCommandDone = false;
                do
                {
                    if( VmbErrorSuccess != SP_ACCESS( pCommandFeature )->IsCommandDone( bIsCommandDone ) )
                    {
                        break;
                    }
                } while( false == bIsCommandDone );
            }
        }
        FeaturePtr pFormatFeatureCam2;
        res = SP_ACCESS( RightCamera )->GetFeatureByName( "PixelFormat", pFormatFeatureCam2 );

        if( VmbErrorSuccess == res )
        {
            if (pixel_mode == 0)
            res = SP_ACCESS( pFormatFeatureCam2 )->SetValue( 17301505  );  //mono frame

            else if (pixel_mode == 1)
            res = SP_ACCESS( pFormatFeatureCam2 )->SetValue( 35127317 );  //color frame
        }

        SP_ACCESS( RightCamera )->GetFeatureByName("Width", pFormatFeatureCam2);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam2 )->GetValue(RightWidth);
        }

        SP_ACCESS( RightCamera )->GetFeatureByName("Height", pFormatFeatureCam2);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam2 )->GetValue(RightHeight);
        }
             // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)
             SP_SET( RightFrameObserver , new FrameObserver( RightCamera ) );

        // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)

    }

//once the configuration of both cameras is done, then we can start the continuous acquisition of both cameras.
    // Start streaming
    res = SP_ACCESS( RightCamera )->StartContinuousImageAcquisition( NUM_FRAMES,  RightFrameObserver );
    return res;
}
VmbErrorType ApiController::StartContinuousAcquisitionOfTwoCameras( const std::string &rStrCameraID ,int pixel_mode, const std::string &rStrCameraID2, int pixel_mode2 )
{
    //VmbErrorType ApiController::StartContinuousImageAcquisition(const char* IP)
    //VmbErrorType res = m_system.OpenCameraByID( IP, VmbAccessModeFull, m_pCamera );
//    VmbErrorType res;
//    qDebug()<<"qq";
//    res = StartContinuousAcquisitionOfLeftCameras(rStrCameraID,pixel_mode);
//    res = StartContinuousAcquisitionOfRightCameras(rStrCameraID2,pixel_mode2);

    QString res1;
    QString res2;
    res1 = QString::fromStdString(rStrCameraID);
    res2 = QString::fromStdString(rStrCameraID2);

    // Open the first Camera: m_pCamera1
    VmbErrorType res = NowSystem.OpenCameraByID( rStrCameraID.c_str(), VmbAccessModeFull, LeftCamera );
    if( VmbErrorSuccess == res )
    {
        FeaturePtr pFeature; // Generic feature pointer
        LeftCamera -> GetFeatureByName("StreamBytesPerSecond",pFeature);

        if(VmbErrorSuccess !=  pFeature -> SetValue(6200000))
        {
            qDebug()<<"left stream bytespersecond not 6200000";
        }
        // Set the GeV packet size to the highest possible value
        // (In this example we do not test whether this cam actually is a GigE cam)
        FeaturePtr pCommandFeature;
        if( VmbErrorSuccess == SP_ACCESS( LeftCamera )->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ) )
        {

            if( VmbErrorSuccess == SP_ACCESS( pCommandFeature )->RunCommand() )
            {
                bool bIsCommandDone = false;
                do
                {
                    if( VmbErrorSuccess != SP_ACCESS( pCommandFeature )->IsCommandDone( bIsCommandDone ) )
                    {
                        break;
                    }
                } while( false == bIsCommandDone );
            }
        }
        FeaturePtr pFormatFeatureCam1;
        res = SP_ACCESS( LeftCamera )->GetFeatureByName( "PixelFormat", pFormatFeatureCam1 );

        if( VmbErrorSuccess == res )
        {
            if (pixel_mode == 0)
            res = SP_ACCESS( pFormatFeatureCam1 )->SetValue( 17301505  );  //mono frame

            else if (pixel_mode == 1)
            res = SP_ACCESS( pFormatFeatureCam1 )->SetValue( 35127317 );  //color frame
        }
        SP_ACCESS( LeftCamera )->GetFeatureByName("Width", pFormatFeatureCam1);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam1 )->GetValue(LeftWidth);
        }

        SP_ACCESS( LeftCamera )->GetFeatureByName("Height", pFormatFeatureCam1);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam1 )->GetValue(LeftHeight);
        }


         // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)
         SP_SET( LeftFrameObserver , new FrameObserver( LeftCamera ) );

    }



    // Open the second Camera: m_pCamera2
    res = NowSystem.OpenCameraByID( rStrCameraID2.c_str(), VmbAccessModeFull, RightCamera );
    if( VmbErrorSuccess == res )
    {

        FeaturePtr pFeature; // Generic feature pointer
        RightCamera -> GetFeatureByName("StreamBytesPerSecond",pFeature);
        if(VmbErrorSuccess !=  pFeature -> SetValue(6200000))
        {
            qDebug()<<"right stream bytespersecond not 6200000";
        }
        // Set the GeV packet size to the highest possible value
        // (In this example we do not test whether this cam actually is a GigE cam)
        FeaturePtr pCommandFeature;
        if( VmbErrorSuccess == SP_ACCESS( RightCamera )->GetFeatureByName( "GVSPAdjustPacketSize", pCommandFeature ) )
        {
            if( VmbErrorSuccess == SP_ACCESS( pCommandFeature )->RunCommand() )
            {
                bool bIsCommandDone = false;
                do
                {
                    if( VmbErrorSuccess != SP_ACCESS( pCommandFeature )->IsCommandDone( bIsCommandDone ) )
                    {
                        break;
                    }
                } while( false == bIsCommandDone );
            }
        }
        FeaturePtr pFormatFeatureCam2;
        res = SP_ACCESS( RightCamera )->GetFeatureByName( "PixelFormat", pFormatFeatureCam2 );

        if( VmbErrorSuccess == res )
        {
            if (pixel_mode2 == 0)
            res = SP_ACCESS( pFormatFeatureCam2 )->SetValue( 17301505  );  //mono frame

            else if (pixel_mode2 == 1)
            res = SP_ACCESS( pFormatFeatureCam2 )->SetValue( 35127317 );  //color frame
        }

        SP_ACCESS( RightCamera )->GetFeatureByName("Width", pFormatFeatureCam2);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam2 )->GetValue(RightWidth);
        }

        SP_ACCESS( RightCamera )->GetFeatureByName("Height", pFormatFeatureCam2);
        if( VmbErrorSuccess == res )
        {
            res = SP_ACCESS( pFormatFeatureCam2 )->GetValue(RightHeight);
        }
             // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)
             SP_SET( RightFrameObserver , new FrameObserver( RightCamera ) );

        // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)

    }

//once the configuration of both cameras is done, then we can start the continuous acquisition of both cameras.
    // Start streaming
    res = SP_ACCESS( LeftCamera )->StartContinuousImageAcquisition( NUM_FRAMES,  LeftFrameObserver );
     if (res == VmbErrorSuccess)
     {
    // Start streaming
        res = SP_ACCESS( RightCamera )->StartContinuousImageAcquisition( NUM_FRAMES,  RightFrameObserver );
     }

    return res;
}

VmbErrorType ApiController::StopLeftContinuousImageAcquisition()
{
    VmbErrorType res;

    // Stop streaming
    SP_ACCESS( LeftCamera )->StopContinuousImageAcquisition();
    //SP_ACCESS( RightCamera )->StopContinuousImageAcquisition();

    // Close cameras
    res = LeftCamera->Close();
    if (res == VmbErrorSuccess){
        //res = RightCamera->Close();
    }
    return  res;
}
VmbErrorType ApiController::StopRightContinuousImageAcquisition()
{
    VmbErrorType res;

    // Stop streaming
    //SP_ACCESS( LeftCamera )->StopContinuousImageAcquisition();
    SP_ACCESS( RightCamera )->StopContinuousImageAcquisition();

    // Close cameras
    res = RightCamera->Close();
    if (res == VmbErrorSuccess){
        //res = RightCamera->Close();
    }

    return  res;
}
VmbErrorType ApiController::StopContinuousImageAcquisition()
{
    VmbErrorType res;

    // Stop streaming
    SP_ACCESS( LeftCamera )->StopContinuousImageAcquisition();
    SP_ACCESS( RightCamera )->StopContinuousImageAcquisition();

    // Close cameras
    res = LeftCamera->Close();
    if (res == VmbErrorSuccess)
        res = RightCamera->Close();

    return  res;
}


//
// Returns the frame observer as QObject pointer to connect their signals to the view's slots
//
QObject* ApiController::GetLeftFrameObserver()
{
    return SP_DYN_CAST( LeftFrameObserver, FrameObserver ).get();
}

//
// Returns the frame observer as QObject pointer to connect their signals to the view's slots
//
QObject* ApiController::GetRightFrameObserver()
{
    return SP_DYN_CAST( RightFrameObserver, FrameObserver ).get();
}

void ApiController::ClearLeftFrameQueue()
{
    SP_DYN_CAST( LeftFrameObserver,FrameObserver )->ClearFrameQueue();
    //SP_DYN_CAST( RightFrameObserver,FrameObserver )->ClearFrameQueue();
}
void ApiController::ClearRightFrameQueue()
{
    //SP_DYN_CAST( LeftFrameObserver,FrameObserver )->ClearFrameQueue();
    SP_DYN_CAST( RightFrameObserver,FrameObserver )->ClearFrameQueue();
}
void ApiController::ClearFrameQueue()
{
    SP_DYN_CAST( LeftFrameObserver,FrameObserver )->ClearFrameQueue();
    SP_DYN_CAST( RightFrameObserver,FrameObserver )->ClearFrameQueue();
}
