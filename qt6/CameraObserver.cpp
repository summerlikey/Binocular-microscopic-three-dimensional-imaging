#include "CameraObserver.h"

void CameraObserver::CameraListChanged( CameraPtr pCam, UpdateTriggerType reason )
{
    if (UpdateTriggerPluggedIn == reason
         || UpdateTriggerPluggedOut == reason )
    {
        // Emit the new camera signal
        emit CameraListChangedSignal( reason );
    }
}
