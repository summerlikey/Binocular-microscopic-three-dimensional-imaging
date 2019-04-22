#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<stdio.h>
#include"vmbimage.h"
#include"frameobserver.h"
#include<QLabel>

#include <synchapi.h>
using namespace std;

void sleep(int msec)     //sleep(函数定义)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //FrameObserver CameraF;

    VimbaSystem &sys = VimbaSystem::GetInstance();//系统初始化
    cout<<"112"<<endl;
    CameraPtrVector cameras;//所有相机
    CameraPtr camera;//单个相机
    std::string name;
    string UniqueId;
    string ModelName;
    string SerialNum;
    VmbAccessModeType ModeType;
    string IdOfInterface;
    FeaturePtr feature;//共享功能，可以用于访问相机功能和设置功能
    FramePtrVector frames(15); // Frame array,帧个数
    VmbInt64_t width;//宽度像素
    VmbInt64_t height;//高度像素
    VmbInt64_t nPLS; // Payload size value，帧大小？
    if (VmbErrorSuccess == sys.Startup())//打开系统
    {
        cout<<"sss"<<endl;
        if (VmbErrorSuccess == sys.GetCameras(cameras))//寻找所有相机
        {
            for (CameraPtrVector::iterator iter = cameras.begin();
                cameras.end() != iter;
                ++iter)
            {
                if (VmbErrorSuccess == (*iter)->GetName(name))
                {
                    std::cout << name << std::endl;//厂商
                }
                if (VmbErrorSuccess == (*iter)->GetID(UniqueId))
                {
                    std::cout << UniqueId << std::endl;//
                }
                if (VmbErrorSuccess == (*iter)->GetModel(ModelName))
                {
                    std::cout << ModelName << std::endl;//型号
                }
                if (VmbErrorSuccess == (*iter)->GetSerialNumber(SerialNum))
                {
                    std::cout << SerialNum << std::endl;//serialNumber
                }
                if (VmbErrorSuccess == (*iter)->GetPermittedAccess(ModeType))
                {
                    std::cout << ModeType << std::endl;
                }
                if (VmbErrorSuccess == (*iter)->GetInterfaceID(IdOfInterface))
                {
                    std::cout << IdOfInterface << std::endl;
                }
            }

            camera=cameras[0];//单个相机等于第一个相机

            if(VmbErrorSuccess==camera ->Open(VmbAccessModeFull))//相机打开方式,有三种打开方式
            {
                cout<<"camera[0] is open "<<endl;
                //设置相机功能，采用feature公共指针进行命令和读取操作，可以设置，有的命令读取即可设置
                camera ->GetFeatureByName("AcquisitionMode", feature);//采集模式
                feature ->SetValue( "Continuous");//连续采集模式
                camera ->GetFeatureByName("Width", feature);
                feature ->GetValue(width);//宽度

                camera ->GetFeatureByName("Height", feature);
                feature ->GetValue(height);//高度

                camera ->GetFeatureByName("PayloadSize", feature);
                feature ->GetValue(nPLS);//PayoadSize设置为nPLS，有些功能访问就会改变它的值

                cout<<width<<endl;//输出宽
                cout<<height<<endl;//输出高
                cout<<nPLS<<endl;//输出大小

                //创造qt 灰度图像
                /*
                CameraF.imageX=QImage(static_cast<int>(width),static_cast<int>(height),QImage::Format_Indexed8);
                //为qt 创建索引颜色表
                CameraF.imageX.setColorCount(256);
                for(int i=0;i<256;i++)
                    CameraF.imageX.setColor(i,qRgb(i,i,i));
                */

                //宣布帧
                for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                {
                    (*iter).reset(new Frame(nPLS));//每一帧给定大小nPLS
                    (*iter)->RegisterObserver(IFrameObserverPtr(new FrameObserver(camera)));//注册观察器，能够观察到相机功能的更改，暂时不考虑相机变化
                    //(*iter)->RegisterObserver(new FrameDoneCallback(camera,*iter));//在写重要，每一帧绑定一个回调
                    camera ->AnnounceFrame(*iter);//相机定义帧
                }
                //开始引擎，将帧放入队列
                camera->StartCapture();//开始引擎
                for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                {
                    //Put frame into the frame queue
                    camera ->QueueFrame(*iter);//帧放入队列
                }
                //开始采集
                if(VmbErrorSuccess==camera ->GetFeatureByName("AcquisitionStart", feature))//开始采集
                {
                    feature ->RunCommand();//命令输入
                    cout<<"AcquisitionStart"<<endl;
                }
                Sleep(5000);

                cout<<"the camera are working"<<endl;



            }

            // Stop the acquisition engine (camera)
            if(VmbErrorSuccess==camera ->GetFeatureByName("AcquisitionStop", feature))
            {
                feature->RunCommand();
                cout<<"AcquisitionStop"<<endl;
            }
            if(VmbErrorSuccess==camera ->EndCapture())//停止采集引擎
            {
                cout<<"EndCapture"<<endl;
            }
            if(VmbErrorSuccess==camera ->FlushQueue())//释放队列
            {
                cout<<"FlushQueue"<<endl;
            }
            if(VmbErrorSuccess==camera ->RevokeAllFrames())//释放所有帧
            {
                cout<<"RevokeAllFrames"<<endl;
            }
            /*
            for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)//还为成功和前面的回调函数
            {
                // Unregister the frame observer/callback
                (*iter)->UnregisterObserver();//释放所有观察器或者回调函数
            }
            */
            if(VmbErrorSuccess==camera ->Close())//关闭当前相机
            {
                cout<<"camera is close"<<endl;
            }

        }
        else
        {
            cout<<"there is no cameras."<<endl;
        }
        if(VmbErrorSuccess==sys.Shutdown()) // Always pair sys.Startup and sys.Shutdown,所有相机处理完成，关闭API系统
        {
            cout<<"sys is shutdown "<<endl;
        }
    }
    else
    {
        cout<<"system is not opne"<<endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::UpdateImage(){
    //将图像放在label中
    label->setPixmap(QPixmap::fromImage(Image()));
    //缩放label
    //label->resize(0.3 * label->pixmap()-size());
    label->update();
}
*/
