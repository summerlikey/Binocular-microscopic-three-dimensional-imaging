#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <QThread>
#include<QTimer>
#include <QDebug>
#include<QPixmap>
#include "VimbaCPP/Include/VimbaCPP.h"
#include"api.h"
#include "frameobserver.h"
#include<QMutex>

using namespace AVT::VmbAPI;
class CameraThread:public QThread
{
    Q_OBJECT
public:
    CameraThread();

    VmbErrorType threadError;//线程里面的Vmberrortype错误
    //void setNowSystem(VimbaSystem &system);
    VimbaSystem& GetNowSystem();//获取NowSystem
    void SystemStartup();//打开api
    void SystemShutDown();//关闭CDK
    bool GetCameraStatusIsRun();//获取状态
    void SetCameraStatusIsRun(bool sta);//设置状态
    void SetNowCameraId(QString CameraId);
    QString GetNowCameraId();
    int GetHeight()const;//获取私有数据成员m_nHeight
    int GetWidth()const;//获取室友数据成员m_nWidth
    QImage NowImage;// 左边 qimage,最终采用qlabel显示
    FramePtr GetFrame();//获取私有数据成员，帧观察器对应的帧
    void run();
    void stopImmediately();
    VmbErrorType QueueFrame(FramePtr pFrame);
    void ClearFrameQueue();//清除帧队列
    void GetNowCamera(VimbaSystem &system,QString CameraId);
    void StopNowCamera();
    QObject* GetFrameObserver();//QOject帧对象
    VmbInt64_t setFrameSize(VmbInt64_t nPLS);
    VmbPixelFormatType GetPixelFormat() const;//获取私有数据成员m_nPixelFormat
    void DoNowFrame(FramePtr d_frame,int status);
    VmbErrorType CopyToImage(VmbUchar_t *pInBuffer,VmbPixelFormat_t ePixelFormat,QImage &pOutImage, const float *Matrix = NULL);//将缓冲区图像显示到qImage，并返回错误类型VmbErrorType err
signals:
    void QimageIsReady(int ima);
    void FrameReceivedSignal(int status);
private:
    //QImage Lm_Image;// the qimage,最终采用qlabel显示
    QTimer timer;
    VimbaSystem &NowSystem;//注册线程系统，试试线程能不能创建两个系统.需要初始化     不成功删掉，
    QString NowCameraId;//用于打开相机，通过  setNowCameraId(VimbaSystem &system,QString CameraId)设置
    bool CameraStatusIsRun;//相机采集状态
    IFrameObserverPtr m_pFrameObserver;//帧观察器
    //IFrameObserverPtr *MyFrameObserver;//帧观察器
    CameraPtr ThreadCamera;//相机,由GetNowCamera获得,打开方式中有引用
    static const int add=8;
    static int creatnumber;//只有static const int  类型可以在类内定义，const string const double 都不行
    bool camerathread_canrun;//标记退出
    QMutex m_lock;
    VmbInt64_t m_nWidth;//目前的宽度
    VmbInt64_t m_nHeight;//目前的高度
    VmbInt64_t m_nPixelFormat;//格式
    VmbInt64_t FrameSize;
    std::queue<FramePtr>thread_Frames;//帧队列
    QMutex thread_FramesMutex;//串行线程，锁住帧，不然跟着的帧改变还没处理完全的,只允许一个线程获得互斥量，程序中，两个相机两个线程，如果不用，将会导致帧还没充满，就使得帧被覆盖了，相当于网卡处线程，交替获得网卡的控制权

    //FramePtrVector frames();

private slots:
    void NowOnFrameReady(int status);//左边帧处理
};

#endif // CAMERATHREAD_H
