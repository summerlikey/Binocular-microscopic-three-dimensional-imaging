#ifndef CAMERAOBSERVER_H
#define CAMERAOBSERVER_H

#include <QObject>
#include"VimbaCPP/Include/VimbaCPP.h"
using namespace AVT::VmbAPI;
class CameraObserver:public QObject,public ICameraListObserver
{
    Q_OBJECT
public:
    virtual void CameraListChanged( CameraPtr pCamera, UpdateTriggerType reason );//观察器
signals:
    void CameraListChangedSignal(int reason);//信号
};

#endif // CAMERAOBSERVER_H
