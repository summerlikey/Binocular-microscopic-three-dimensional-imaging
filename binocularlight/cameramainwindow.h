#ifndef MAINWINDOW_H
#define CAMERAMAINWINDOW_H

#include <QMainWindow>
#include "apicontroller.h"

namespace Ui {
class CameraMainWindow;
}

class CameraMainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraMainWindow(QWidget *parent = nullptr);
    ~CameraMainWindow();
    void Log( std::string strMsg, VmbErrorType eErr );
    void Log( std::string strMsg);
    QImage Image_LC;// 左边 qimage,最终采用qlabel显示
    QImage Image_RC;// 右边label图片显示，采用多线程方式
    QImage Image_SI;// show image
    void GetLeftCameraId();
    void GetRightCameraId();
    void SetChangeExposure();
private slots:
    void OnCameraListChanged(int reason );
    void OnCLickedBtn_LeftCameraStartStop();
    void OnCLickedBtn_RightCameraStartStop();
    void OnClickedBtn_TwoStartStop();
    void OnLeftFrameReady(int status, QString camera );
    void OnRightFrameReady(int status, QString camera );
    void setLeftExposure(int value);
    void setRightExposure(int value);

private:
    Ui::CameraMainWindow *ui;
    //Left camera DEV_000F315BA9A2
    //Right camera DEV_000F315BA9A3
    QString LeftCameraId = "DEV_000F315BA9A2";
    QString RightCameraId = "DEV_000F315BA9A3";
    // Our controller that wraps API access
    ApiController NowApiController;
    // A list of known camera IDs
    std::vector<std::string> AllCamerasId;
    void UpdateCameraListBox();

    VmbErrorType CopyToImage( VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, VmbUint32_t nHeight, VmbUint32_t nWidth, QImage &pOutImage, const float *Matrix /*= NULL */ );
    bool LeftIsStream;
    bool RightIsStream;
    QImage LeftQImage;
    QImage RightQImage;
    VmbUint32_t			LeftFrameWidth;
    VmbUint32_t			LeftFrameHeight;
    VmbPixelFormatType	LeftPixelFormat;

    VmbUint32_t			RightFrameWidth;
    VmbUint32_t			RightFrameHeight;
    VmbPixelFormatType	RightPixelFormat;

};

#endif // CAMERAMAINWINDOW_H
