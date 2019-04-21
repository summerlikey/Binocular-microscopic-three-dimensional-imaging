#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include"vmbcamera.h"
namespace Ui {
class MainWindow;
}

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
    //void SetExposure();
private:
    Ui::MainWindow *ui;
    QLabel* label;
    QWidget* widget;
    vmbcamera cameraX;
};

#endif // MAINWINDOW_H
