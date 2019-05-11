#include "frameobserver.h"
#include<QDebug>
#include<QThread>
void FrameObserver::FrameReceived(const FramePtr pFrame)
{
   bool bQueueDirectly = true;
   VmbFrameStatusType eReceiveStatus;
   if(0 != receivers(SIGNAL(FrameReceivedSignal(int)))
           &&  VmbErrorSuccess == pFrame->GetReceiveStatus( eReceiveStatus ))
   {
       m_FramesMutex.lock();//lock
       m_Frames.push(pFrame);
       m_FramesMutex.unlock();//unlock
       //qDebug()<<"frame data ";
       qDebug()<<"frame currentThread id"<<QThread::currentThreadId();
       emit FrameReceivedSignal( eReceiveStatus );//emit singal
       bQueueDirectly = false;
   }
   if(bQueueDirectly==true)
   {
       m_pCamera->QueueFrame(pFrame);
   }
}
FramePtr FrameObserver::GetFrame()
{
    m_FramesMutex.lock();
    FramePtr res;
    if(!m_Frames.empty())
    {
         res = m_Frames.front();//取出队首
         m_Frames.pop();//队首出队
    }
    m_FramesMutex.unlock();
    return res;//取出队首
}
void FrameObserver::ClearFrameQueue()
{
    m_FramesMutex.lock();
    std::queue<FramePtr>empty;
    std::swap(m_Frames,empty);//释放队列？
    m_FramesMutex.unlock();
}
