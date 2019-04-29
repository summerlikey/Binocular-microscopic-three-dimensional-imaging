#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <QObject>
#include <QMutex>

#include<queue>

#include"VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;
class FrameObserver : public QObject, virtual public IFrameObserver
{
    Q_OBJECT
public:

    FrameObserver( CameraPtr pCamera ) : IFrameObserver( pCamera ) {;}
    virtual void FrameReceived( const FramePtr pFrame );
    FramePtr GetFrame();//获取帧，从帧队列中，帧队列是私有的所以要类内访问
    void ClearFrameQueue();//清除队列
private:
    std::queue<FramePtr>m_Frames;//帧队列
    QMutex m_FramesMutex;//串行线程，锁住帧，不然跟着的帧改变还没处理完全的
signals:
    void FrameReceivedSignal(int status);
};

#endif // FRAMEOBSERVER_H
