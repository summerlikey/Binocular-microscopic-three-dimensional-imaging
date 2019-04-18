#ifndef QTVMBVIEWER_H
#define QTVMBVIEWER_H

#include <QMainWindow>
#include<QLabel>
#include<QSlider>
#include<QWidget>
#include "VmbCamera.h"
namespace Ui{
class MainWindow;
}

// Qt widget to display the images from an Allied Vision camera through the Vimba SDK
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//QT slots
private slots:
    //不成功
    void UpdateImage();
    void SetExposure();
private:
    Ui::MainWindow *ui;
    QLabel* label;
    QSlider* slider;
    VmbCamera camera;
    QWidget* widget;

};

#endif // QTVMBVIEWER_H
