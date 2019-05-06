#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include<QThread>

class camerathread:public QThread
{
    Q_OBJECT
public:
    camerathread();
    void run();
};

#endif // CAMERATHREAD_H
