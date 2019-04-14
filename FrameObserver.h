#include "pch.h"
#include <tchar.h>
#include <iostream >
#include <stdio.h>
#include "VimbaCPP/Include/VimbaCPP.h"
//对新帧的反应 类定义
using namespace AVT::VmbAPI;
class FrameObserver : public IFrameObserver
{
public:
	FrameObserver(CameraPtr pCamera);
	/*:IFrameObserver(pCamera)//构造函数
	
	{
		//初始化构造函数，自己写
	}
	*/
	void FrameReceived(const FramePtr pFrame);//成员函数
	/*
	{
		VmbFrameStatusType eReceiveStatus;
		if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus))
		{
			if (VmbFrameStatusComplete == eReceiveStatus)
			{
				//成功接受帧上做出反应，自己写
			}
			else
			{
				//接受失败的反应
			}
		}
		//当完成帧处理后，重新排队
		m_pCamera->QueueFrame(pFrame);
	}
	*/
    
};
