#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),LeftisRun(false),RightisRun(false)
{
    qDebug()<<"mainwindow currentThread ID"<<QThread::currentThreadId();
    ui->setupUi(this);
    VmbErrorType err=m_api.ApiStartUp();//Api启动
    Log("Vimbaopen",err);//api启动成功

    //传入左边相机id和右边相机id,如果需要换相机直接这换相机Id
    QString str="DEV_000F315BA9A2";
    SetLeftCameraId(str);
    str="DEV_000F315BA9A3";
    SetRightCameraId(str);

    this->ui->LeftCameraLight_label->setPixmap(QPixmap(":/image/red").scaled(15,15));//左边相机状态灯
    this->ui->RightCameraLight_label->setPixmap(QPixmap(":/image/red").scaled(15,15));//右边相机状态灯
    //this->ui->Projector_Status->setPixmap(QPixmap(":/image/red").scaled(15,15));//投影仪状态灯
    QObject::connect(m_api.GetCameraObserver(),SIGNAL(CameraListChangedSignal(int)),this,SLOT( OnCameraListChanged(int)));//相机发现
    UpdateCameraListBox();//初始化相机盒子,同时返回到m_camera;


    LeftCameraThread = new CameraThread();
    RightCameraThread = new CameraThread();

    //QObject::connect(Test,SIGNAL(QimageIsReady(int)),this ,SLOT( LeftCameraQimage(int) ) );//测试图片链接，无相机链接系统会崩，注意进行错误返回，防止系统，因为掉帧，掉相机崩溃，原因容易访问不存在指针。
    QObject::connect(LeftCameraThread,SIGNAL(QimageIsReady(int)),this ,SLOT( LeftCameraQimage(int) ) );//测试图片链接，无相机链接系统会崩，注意进行错误返回，防止系统，因为掉帧，掉相机崩溃，原因容易访问不存在指针。
    QObject::connect(RightCameraThread,SIGNAL(QimageIsReady(int)),this ,SLOT( RightCameraQimage(int) ) );//测试图片链接，无相机链接系统会崩，注意进行错误返回，防止系统，因为掉帧，掉相机崩溃，原因容易访问不存在指针。
    QObject::connect(this->ui->leftcamera_btn,SIGNAL(clicked()),this,SLOT(LeftCamera_StartStop()));
    QObject::connect(this->ui->rightcamera_btn,SIGNAL(clicked()),this,SLOT(RightCamera_StartStop()));


}

MainWindow::~MainWindow()
{
    delete ui;
    LeftCameraThread->quit();
    RightCameraThread->quit();
    m_api.ApiShutDown();
}



void MainWindow::SetLeftCameraId(QString id)
{
    LeftCameraId = id ;
}
void MainWindow::SetRightCameraId(QString id)
{
    RightCameraId = id ;
}
QString MainWindow::GetLeftCameraId()
{
    return LeftCameraId;
}
QString MainWindow::GetRightCameraId()
{
    return RightCameraId;
}

void MainWindow::OnCameraListChanged(int reason)//相机列表
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
void MainWindow::UpdateCameraListBox()//更新相机列表
{
    // Get all cameras currently connected to Vimba
    CameraPtrVector cameras = m_api.GetCameraList();

    // Simply forget about all cameras known so far
    ui->camera_lw->clear();
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
            ui->camera_lw->addItem(QString::fromStdString(strCameraName + " " +strCameraID));
            m_cameras.push_back(strCameraID);
        }
    }
    SetLeftCameraStatusLight();
    SetRightCameraStatusLight();
}


void MainWindow::SetLeftCameraStatusLight()//获取左边相机Id
{
    QString str=GetLeftCameraId();
    std::string leftcameraid=str.toStdString();//左边相机id
    int t=0;
        for(std::vector<std::string>::iterator it=m_cameras.begin();it!=m_cameras.end();it++)
        {
            if(*it==leftcameraid){
                Log("Left camera is found");
                this->ui->LeftCameraLight_label->setPixmap(QPixmap(":/image/green").scaled(15,15));//左边相机状态灯
                t=1;
            }
        }
        if(t==0){
            Log("ERROR Left camera is not found");
            this->ui->LeftCameraLight_label->setPixmap(QPixmap(":/image/red").scaled(15,15));//左边相机状态灯
        }
}

void MainWindow::SetRightCameraStatusLight()//获取右边相机Id
{
    QString str=GetRightCameraId();
    std::string rightcameraid=str.toStdString();//左边相机id
    int t=0;
    for(std::vector<std::string>::iterator it=m_cameras.begin();it!=m_cameras.end();it++)
    {

        if(*it==rightcameraid){
            Log("Right camera is found");
            this->ui->RightCameraLight_label->setPixmap(QPixmap(":/image/green").scaled(15,15));//右边相机状态灯
            t=1;
        }
    }
    if(t==0)
    {
        Log("ERROR Right camera is not found");
        this->ui->RightCameraLight_label->setPixmap(QPixmap(":/image/red").scaled(15,15));//右边相机状态灯
    }
}
void MainWindow::LeftCamera_StartStop()
{
    if(false==LeftisRun)
    {
        LeftisRun=true;
        LeftCameraThread->SetCameraStatusIsRun(LeftisRun);
        //Lm_Image = QImage( LeftCameraThread.GetWidth(),LeftCameraThread.GetHeight(),QImage::Format_RGB888);
        RunLeftCamera();//左边相机过的运行
        Log("left camera is run");

        //Log("Left Camera display");
    }
    else {
        //停止线程
//        LeftCameraThread.stopImmediately();//停止线程
        LeftisRun=false;
        LeftCameraThread->SetCameraStatusIsRun(LeftisRun);

        StopLeftCamera();
        LeftCameraThread->quit();
        if(LeftCameraThread->isRunning()==true)
        {
            Log("is runing,not quit success");
        }
        else {
            Log("is quit,quit success");
        }
    }

    if(LeftisRun==true)
    {
        this->ui->leftcamera_btn->setText(QString("关闭左边相机"));
    }
    if(LeftisRun==false)
    {
        this->ui->leftcamera_btn->setText(QString("打开左边相机"));
    }
}

void MainWindow::RightCamera_StartStop()
{
    if(false==RightisRun)
    {
        RightisRun=true;
        RightCameraThread->SetCameraStatusIsRun(RightisRun);
        RunRightCamera();//左边相机过的运行
        Log("right camera is run");
    }
    else {
        //停止线程
//        LeftCameraThread.stopImmediately();//停止线程
        StopRightCamera();
        RightCameraThread->quit();
        if(RightCameraThread->isRunning()==true)
        {
            Log(" Right is runing,not quit success");
        }
        else {
            Log("Right is quit,quit success");
        }
        RightisRun=false;
        RightCameraThread->SetCameraStatusIsRun(false);
    }

    if(RightisRun==true)
    {
        this->ui->rightcamera_btn->setText(QString("关闭右边相机"));
    }
    if(RightisRun==false)
    {
        this->ui->rightcamera_btn->setText(QString("打开右边相机"));
    }
}

void MainWindow::RunLeftCamera()
{

    LeftCameraThread->SystemStartup();

    LeftCameraThread->SetNowCameraId(GetLeftCameraId());

    //LeftCameraThread->GetNowCameraId();//获得左边相机

    //LeftCameraThread.GetCameraStatus();
    LeftCameraThread->start();//开始run

    Log("runleftcamera");
}
void MainWindow::RunRightCamera()
{
    RightCameraThread->SystemStartup();
    RightCameraThread->SetNowCameraId(GetRightCameraId());//获得右边相机

    //RightCameraThread.GetCameraStatus();
    RightCameraThread->start();//开始run
    Log("runrightcamera");
}

void MainWindow::StopLeftCamera()
{
    //左边相机Id  , DEV_000F315BA9A2
    //LeftCameraThread->StopNowCamera();
    LeftCameraThread->SystemShutDown();
}
void MainWindow::StopRightCamera()
{
    //右边相机Id  , DEV_000F315BA9A3
    //RightCameraThread->StopNowCamera();
    RightCameraThread->SystemShutDown();
}

void MainWindow::LeftCameraFrameReady(int status)
{
    if(true==LeftisRun)
    {
        FramePtr s_frame;
        s_frame=LeftCameraThread->GetFrame();
        if(SP_ISNULL(s_frame))//没有获取到帧
        {
            Log("Left frame pointer is NULL, late frame ready message");
            return;
        }
        if( VmbFrameStatusComplete == status )//完全帧
        {
            VmbUchar_t *pBuffer;
            VmbErrorType err = s_frame->GetImage(pBuffer);//获取帧图像数据，没有标识位，全图像数据
            if(VmbErrorSuccess==err)//缓冲区获取成功
            {
                VmbUint32_t nSize;
                s_frame->GetImageSize(nSize);
                VmbPixelFormatType ePixelFormat = LeftCameraThread->GetPixelFormat();//获取相机目前图像格式
            }
        }
        LeftCameraThread->QueueFrame(s_frame);
    }
}
void MainWindow::RightCameraFrameReady(int status)
{
    if(true==RightisRun)
    {
        FramePtr s_frame;
        s_frame=RightCameraThread->GetFrame();
        if(SP_ISNULL(s_frame))//没有获取到帧
        {
            Log("Right frame pointer is NULL, late frame ready message");
            return;
        }
        if( VmbFrameStatusComplete == status )//完全帧
        {
            VmbUchar_t *pBuffer;
            VmbErrorType err = s_frame->GetImage(pBuffer);//获取帧图像数据，没有标识位，全图像数据
            if(VmbErrorSuccess==err)//缓冲区获取成功
            {
                VmbUint32_t nSize;
                s_frame->GetImageSize(nSize);
                VmbPixelFormatType ePixelFormat = RightCameraThread->GetPixelFormat();//获取相机目前图像格式
            }
        }
        RightCameraThread->QueueFrame(s_frame);
    }
}
void MainWindow::LeftCameraQimage(int sta)
{
    const QSize s = ui->leftcamera_label->size();
    ui->leftcamera_label->setStyleSheet("border:2px solid red;");
    ui->leftcamera_label->setPixmap(QPixmap::fromImage(LeftCameraThread->NowImage).scaled(s,Qt::KeepAspectRatio));//自适应QLabel的大小
}
void MainWindow::RightCameraQimage(int sta )
{
    const QSize s = ui->rightcamera_label->size();
    ui->rightcamera_label->setStyleSheet("border:2px solid red;");
    ui->rightcamera_label->setPixmap(QPixmap::fromImage(RightCameraThread->NowImage).scaled(s,Qt::KeepAspectRatio));//自适应QLabel的大小
}

//日志输出和错误输出，进行排查
void MainWindow::Log(std::string strMsg, VmbErrorType eErr)
{
    strMsg += "..."+ m_api.ErrorCodeToMessages(eErr);
    ui->log_lv->insertItem(0, QString::fromStdString(strMsg));
}
void MainWindow::Log( std::string strMsg)
{
    ui->log_lv->insertItem( 0, QString::fromStdString( strMsg ) );
}
