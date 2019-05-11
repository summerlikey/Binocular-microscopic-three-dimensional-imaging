#include "cameraobserver.h"
#include<QDebug>
#include<QThread>
void CameraObserver::CameraListChanged( CameraPtr pCam, UpdateTriggerType reason )
{
    if (UpdateTriggerPluggedIn == reason
         || UpdateTriggerPluggedOut == reason )
    {
        qDebug()<<"cameraobserver thread:"<<QThread::currentThreadId();
        // Emit the new camera signal
        emit CameraListChangedSignal( reason );
    }
}
