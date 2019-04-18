#include "QtVmbViewer.h"
#include "ui_QtVmbViewer.h"
#include "VmbCamera.h"
#include <QPixmap>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    //打开相机
    camera.Startup();
    //创建一个qlabel
    label = new QLabel();
    //缩放标签
    label->setScaledContents(true);
    //创建槽改变曝光值
    slider =new QSlider(Qt::Horizontal);
    //根据相机曝光参数设置滑块
    slider->setRange(64,10000);
    slider->setValue(camera.Exposure());
    slider->update();

    //窗口布局
    widget = new QWidget();
    this->setCentralWidget(widget);
    QVBoxLayout* layout =new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(slider);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    widget->setLayout(layout);
    //连接信号和新收到的图片
    connect(&camera,&VmbCamera::ImageReady,this,&MainWindow::UpdateImage);
    //连接槽和滑块和曝光值
    connect(slider,&QSlider::sliderReleased,this,&MainWindow::SetExposure);
    camera.StartCapture();
}

MainWindow::~MainWindow()
{
    //delete ui;
    camera.StopCapture();
    camera.Shutdown();
}
void MainWindow::UpdateImage(){
    //将图像放在label中
    label->setPixmap(QPixmap::fromImage(camera.Image()));
    //缩放label
    //label->resize(0.3 * label->pixmap()-size());
    label->update();
}
void MainWindow::SetExposure(){
    camera.SetExposure(slider->value());
}

