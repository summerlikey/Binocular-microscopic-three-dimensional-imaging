#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include "errorcodetomessage.h"
#include "cameraobserver.h"
#include "frameobserver.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <string>
using namespace AVT::VmbAPI;

class ApiController: public QObject
{
public:
    ApiController();
    ~ApiController();
    std::string ErrorCodeToMessages(VmbErrorType eErr);//error code message

    VmbErrorType StartUp();//start the api
    void ShutDown();//shut down the api
    QObject* GetCameraObeserver();//get camera observer
    CameraPtrVector GetCameraList();// return cameras
    int GetLeftWidth() const;
    int GetLeftHeight() const;
    int GetRightWidth() const;
    int GetRightHeight() const;
    VmbUint32_t GetLeftWidthVmb32() const;
    VmbUint32_t GetLeftHeightVmb32() const;
    VmbUint32_t GetRightWidthVmb32() const;
    VmbUint32_t GetRightHeightVmb32() const;
    VmbErrorType StartContinuousAcquisitionOfLeftCameras( const std::string &lStrCameraID ,int pixel_mode);
    VmbErrorType StartContinuousAcquisitionOfRightCameras( const std::string &rStrCameraID ,int pixel_mode);
    VmbErrorType StartContinuousAcquisitionOfTwoCameras( const std::string &rStrCameraID ,int pixel_mode, const std::string &rStrCameraID2, int pixel_mode2 );
    VmbErrorType StopLeftContinuousImageAcquisition();
    VmbErrorType StopRightContinuousImageAcquisition();
    VmbErrorType StopContinuousImageAcquisition();



    QObject*            GetLeftFrameObserver();
    QObject*            GetRightFrameObserver();
    void ClearLeftFrameQueue();
    void ClearRightFrameQueue();
    void ClearFrameQueue();
    VmbPixelFormatType  GetLeftPixelFormat() const;
    VmbPixelFormatType  GetRightPixelFormat() const;
    FramePtr            GetLeftFrame();
    FramePtr            GetRightFrame();
    VmbErrorType        QueueLeftFrame( FramePtr pFrame );
    VmbErrorType        QueueRightFrame( FramePtr pFrame );

    VmbErrorType SetApiLeftExposure(int exposure_value);
    VmbErrorType SetApiRightExposure(int exposure_value);
    //void setLeftExposure(int value);

    static int leftexposure;
signals :


private:
  // A reference to our Vimba singleton
  VimbaSystem& NowSystem;

  ICameraListObserverPtr m_pCameraObserver;//Camera Observer
  VmbInt64_t                  StreamBytes;

  VmbInt64_t                  LeftWidth;
  // The current height
  VmbInt64_t                  LeftHeight;

  VmbInt64_t                  RightWidth;
  // The current height
  VmbInt64_t                  RightHeight;

  // The current pixel format
  VmbInt64_t                  LeftPixelFormat;
  VmbInt64_t                  RightPixelFormat;

  CameraPtr LeftCamera;//left
  CameraPtr RightCamera;

  IFrameObserverPtr           LeftFrameObserver;   //FrameObserver*            m_pFrameObserver; // Every camera has its own frame observer
  IFrameObserverPtr           RightFrameObserver;   //FrameObserver*            m_pFrameObserver; // Every camera has its own frame observer
};

#endif // APICONTROLLER_H
