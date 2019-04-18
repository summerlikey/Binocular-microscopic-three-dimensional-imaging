#include "QtVmbViewer.h"
#include <QApplication>
#include<stdio.h>
#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    cout<<"51"<<endl;
    w.show();

    return a.exec();
}
