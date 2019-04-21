#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QImage>
#include <QVBoxLayout>
#include <iostream>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "vmbcamera.h"
using namespace AVT::VmbAPI;
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
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
    FramePtrVector frames(1); // Frame array,帧个数
    VmbInt64_t width;//宽度像素
    VmbInt64_t height;//高度像素
    VmbInt64_t nPLS; // Payload size value，帧大小？
    vmbcamera cameraX;



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
            if(VmbErrorSuccess==camera ->Open(VmbAccessModeFull))//相机打开方式
            {
                cout<<"camera[0] is open "<<endl;
                if(VmbErrorSuccess == camera ->GetFeatureByName("AcquisitionMode", feature))//采集模式
                {
                    if (VmbErrorSuccess == feature ->SetValue( "Continuous"))//连续采集模式，
                    {
                        camera ->GetFeatureByName("Width", feature);
                        feature ->GetValue(width);//宽度

                        camera ->GetFeatureByName("Height", feature);
                        feature ->GetValue(height);//高度
                        camera ->GetFeatureByName("PayloadSize", feature);
                        feature ->GetValue(nPLS);//PayoadSize设置为nPLS，有些功能访问就会改变它的值

                        cout<<width<<endl;//输出宽
                        cout<<height<<endl;//输出高
                        cout<<nPLS<<endl;//输出大小
                        for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                        {
                            (*iter).reset(new Frame(nPLS));//每一帧给定大小
                            //(*iter)->RegisterObserver(IFrameObserverPtr(new FrameObserver(camera)));//注册观察器，能够观察到相机功能的更改，暂时不考虑相机变化
                            camera ->AnnounceFrame(*iter);//相机定义帧
                        }
                        //创造qt 灰度图像
                        QImage image;
                        //创造qt 灰度图像
                        image=QImage(static_cast<int>(width),static_cast<int>(height),QImage::Format_Indexed8);//static_cast 强制转换类型
                        //为qt 创建索引颜色表
                        image.setColorCount(256);
                        for(int i=0;i<256;i++)
                            image.setColor(i,qRgb(i,i,i));

                        // Start the capture engine (API)
                        camera ->StartCapture();//开启捕获引擎
                        for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                        {
                            //Put frame into the frame queue
                            camera ->QueueFrame(*iter);//将帧放入队列
                            FrameDoneCallback(camera,*iter);//回调函数
                        }
                        // Start the acquisition engine (camera)开始采集
                        if (VmbErrorSuccess == camera ->GetFeatureByName( "AcquisitionStart",feature))//开始采集引擎
                        {
                            if ( VmbErrorSuccess == feature ->RunCommand())//写入命令
                            {
                                cout << "Acquisition started" << std::endl;
                            }
                        }

                        VmbUint32_t nTimeout=10;
                        camera->AcquireMultipleImages(frames,nTimeout);
                        VmbUchar_t* pBuffer;
                        FramePtr pframe;

                        for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                        {
                            pframe=*iter;
                            //FrameDoneCallback(camera,*iter);//回调函数
                            pframe->GetBuffer(pBuffer);
                            for(int i=0;i<nPLS;i++)
                            {
                                cout<<*pBuffer<<endl;
                                pBuffer++;
                            }
                            memcpy(image.bits(),pBuffer,nPLS);
                        }
                        label->setPixmap(QPixmap::fromImage(image));
                        label = new QLabel();
                        //缩放标签
                        label->setScaledContents(true);
                        //窗口布局
                        widget = new QWidget();
                        this->setCentralWidget(widget);
                        QVBoxLayout* layout =new QVBoxLayout(this);
                        layout->addWidget(label);
                        layout->setSizeConstraint(QLayout::SetFixedSize);
                        widget->setLayout(layout);
                        label->update();
                        //connect(&cameraX,&vmbcamera::ImageReady,this,&MainWindow::UpdateImage);

                        camera ->EndCapture();//停止引擎
                        camera ->FlushQueue();//释放队列
                        camera ->RevokeAllFrames();//释放帧
                        for(FramePtrVector::iterator iter=frames.begin(); frames.end()!=iter; ++iter)
                        {
                            //Unregister the frame observer/callback
                            //(*iter)->UnregisterObserver();//释放观察器或者回调函数，前面未定义观察器，暂时不考虑观察器
                            //回调函数
                            //FrameDoneCallback(camera,*iter);//回调函数
                        }
                        camera ->Close();//关闭相机
                        if(VmbErrorSuccess==sys.Shutdown()) // Always pair sys.Startup and sys.Shutdown关闭API
                            cout<<"system is shutdown"<<endl;
                    }
                }
            }
        }
        else
        {
            cout<<"there is no cameras."<<endl;
        }
    }
    else {
        cout<<"the system is not open"<<endl;
    }
}

MainWindow::~MainWindow()
{
    //delete ui;
}
void MainWindow::UpdateImage(){
    //将图像放在label中
    label->setPixmap(QPixmap::fromImage(cameraX.Image()));
    //缩放label
    //label->resize(0.3 * label->pixmap()-size());
    label->update();
}
void FrameDoneCallback(CameraPtr camera,FramePtr frame)//回调函数
{
    cout<<"huitiao"<<endl;
    VmbUchar_t* pBuffer;
    FeaturePtr feature;//共享功能，可以用于访问相机功能和设置功能
    VmbInt64_t nPLS; // Payload size value，帧大小？
    VmbFrameStatusType status;
    camera ->GetFeatureByName("PayloadSize", feature);
    feature ->GetValue(nPLS);//PayoadSize设置为nPLS，有些功能访问就会改变它的值
    cout<<nPLS<<endl;
   frame->GetReceiveStatus(status);
   if(status==VmbFrameStatusComplete)
   {
       cout<<"a frame complete"<<endl;
       //获取帧的相机对象
       frame->GetBuffer(pBuffer);
       for(int i=0;i<nPLS;i++)
       {

           cout<<*pBuffer<<endl;
           pBuffer++;
       }
       //vmbcamera* cameraX=(vmbcamera*)camera;
       //memcpy(cameraX->image.bits(),frame->GetBuffer(pBuffer),nPLS);
       //emit camera->ImageReady();
   }
}

