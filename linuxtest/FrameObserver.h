#include "pch.h"
#include <tchar.h>
#include <iostream >
#include <stdio.h>
#include "VimbaCPP/Include/VimbaCPP.h"
//����֡�ķ�Ӧ �ඨ��
using namespace AVT::VmbAPI;
class FrameObserver : public IFrameObserver
{
public:
	FrameObserver(CameraPtr pCamera);
	/*:IFrameObserver(pCamera)//���캯��
	
	{
		//��ʼ�����캯�����Լ�д
	}
	*/
	void FrameReceived(const FramePtr pFrame);//��Ա����
	/*
	{
		VmbFrameStatusType eReceiveStatus;
		if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus))
		{
			if (VmbFrameStatusComplete == eReceiveStatus)
			{
				//�ɹ�����֡��������Ӧ���Լ�д
			}
			else
			{
				//����ʧ�ܵķ�Ӧ
			}
		}
		//�����֡����������Ŷ�
		m_pCamera->QueueFrame(pFrame);
	}
	*/
    
};
