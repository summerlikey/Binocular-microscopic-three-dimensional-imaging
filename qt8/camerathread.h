#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <QThread>
#include<QTimer>
#include <QDebug>
#include<QPixmap>
#include "VimbaCPP/Include/VimbaCPP.h"
#include"api.h"
#include "frameobserver.h"

using namespace AVT::VmbAPI;
class CameraThread:public QThread
{
    Q_OBJECT
public:
    CameraThread();

    VmbErrorType threadError;//线程里面的Vmberrortype错误
    bool GetCameraStatus();//获取状态
    void SetCameraStatus(bool sta);//设置状态
    int GetHeight()const;//获取私有数据成员m_nHeight
    int GetWidth()const;//获取室友数据成员m_nWidth
    QImage NowImage;// 左边 qimage,最终采用qlabel显示
    FramePtr GetFrame();//获取私有数据成员，帧观察器对应的帧
    void run();
    void stopImmediately();
    VmbErrorType QueueFrame(FramePtr pFrame);
    void GetNowCamera(VimbaSystem &system,QString CameraId);
    QObject* GetFrameObserver();//QOject帧对象
    VmbInt64_t setFrameSize(VmbInt64_t nPLS);
    VmbPixelFormatType GetPixelFormat() const;//获取私有数据成员m_nPixelFormat
    VmbErrorType CopyToImage(VmbUchar_t *pInBuffer,VmbPixelFormat_t ePixelFormat,QImage &pOutImage, const float *Matrix = NULL);//将缓冲区图像显示到qImage，并返回错误类型VmbErrorType err
signals:
    void QimageIsReady(int ima);
private:
    //QImage Lm_Image;// the qimage,最终采用qlabel显示
    QTimer timer;
    bool CameraisRun;//相机采集状态
    IFrameObserverPtr m_pFrameObserver;//帧观察器
    IFrameObserverPtr *MyFrameObserver;//帧观察器
    CameraPtr ThreadCamera;//左边相机,由GetLeftCamera获得
    static const int add=8;
    static int creatnumber;//只有static const int  类型可以在类内定义，const string const double 都不行
    bool camerathread_canrun;//标记退出
    QMutex m_lock;
    VmbInt64_t m_nWidth;//目前的宽度
    VmbInt64_t m_nHeight;//目前的高度
    VmbInt64_t m_nPixelFormat;//格式
    VmbInt64_t frameSize;
    FramePtrVector frames();

private slots:
    void NowOnFrameReady(int status);//左边帧处理
};

#endif // CAMERATHREAD_H
