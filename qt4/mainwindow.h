#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vmbimage.h"
#include"frameobserver.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    //成功,需要在mainwindow中声明，不然会有连接错误
    //void UpdateImage();
private:
    //vmbimage imageY;
    Ui::MainWindow *ui;
    //FrameObserver CameraF;
};

#endif // MAINWINDOW_H
