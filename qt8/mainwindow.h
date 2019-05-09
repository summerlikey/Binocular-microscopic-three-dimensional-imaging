
#ifndef MAINWINDOW_H

#define MAINWINDOW_H
#include <QMainWindow>
#include"api.h"
#include"errorcodetomessage.h"
#include<QString>
#include"camerathread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RunLeftCamera();//左线程运行
    void RunRightCamera();//右线程运行
private:
    Ui::MainWindow *ui;
    Api m_api;
    CameraThread LeftCameraThread;//左相机线程
    CameraThread RightCameraThread;//右相机线程
    CameraThread *Test;
    bool LeftisRun;//左相机在工作？
    bool RightisRun;//右相机在工作？
    QImage Lm_Image;// 左边 qimage,最终采用qlabel显示
    QImage Rm_Image;// 右边label图片显示，采用多线程方式
    //Are we streaming?
    bool m_bIsStreaming;
    void Log( std::string strMsg, VmbErrorType eErr );//输出错误信息
    void Log(std::string strMsg);//输出错误信息
    std::vector<std::string> m_cameras;// A list of known camera IDs,用于后续识别
    void UpdateCameraListBox();//更新相机列表到QListWidget
    QString GetLeftCameraId();//获取左边相机Id;目前左边相机DEV_000F315BA9A2
    QString GetRightCameraId();//获取右边相机Id;目前右边相机DEV_000F315BA9A3
private slots:
    void OnCameraListChanged(int reason);//槽，相机改变
    void LeftCamera_StartStop();//单独打开左相机
    void RightCamera_StartStop();//单独打开右相机
    void LeftCameraFrameReady(int status);//左边相机帧收到信号
    void RightCameraFrameReady(int status);//右边相机帧收到信号
    void LeftCameraQimage(int status);
    void RightCameraQimage(int status);
};

#endif // MAINWINDOW_H


