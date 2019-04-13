#include "pch.h"
#include <tchar.h>
#include <iostream >
#include <stdio.h>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "FrameObserver.h"
using namespace std;
using namespace AVT::VmbAPI;
//初始化新帧处理类的构造函数
FrameObserver1::FrameObserver1(CameraPtr pCamera) :IFrameObserver(pCamera)
{
	cout << "111" << endl;
}
//帧回调函数
void FrameObserver1::FrameReceived(const FramePtr pFrame)
{
	//想工作线程发送通知
	//不要在此回调函数中处理图像
	//处理帧后，重新查询
	cout << "222" << endl;
	m_pCamera->QueueFrame(pFrame);
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Hello Vimba" << std::endl;
	/*
	VimbaSystem& system0 = VimbaSystem::GetInstance();
	VmbVersionInfo_t version;
	if (VmbErrorSuccess == system0.QueryVersion(version))
	{
		std::cout << "Version:" << version.major << "." << version.minor << std::endl;
	}
	if (VmbErrorSuccess == system0.Shutdown())
	{
		cout << "system0 is shutdown" << endl;
	}

	//get camera

	std::string name1;
	string UniqueId;
	string ModelName;
	string SerialNum;
	VmbAccessModeType ModeType;
	string IdOfInterface;
	CameraPtrVector cameras1;
	VimbaSystem &system1 = VimbaSystem::GetInstance();
	if (VmbErrorSuccess == system1.Startup())
	{
		if (VmbErrorSuccess == system1.GetCameras(cameras1))
		{
			for (CameraPtrVector::iterator iter = cameras1.begin();
				cameras1.end() != iter;
				++iter)
			{
				if (VmbErrorSuccess == (*iter)->GetName(name1))
				{
					std::cout << name1 << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->GetID(UniqueId))
				{
					std::cout << UniqueId << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->GetModel(ModelName))
				{
					std::cout << ModelName << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->GetSerialNumber(SerialNum))
				{
					std::cout << SerialNum << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->GetPermittedAccess(ModeType))
				{
					std::cout << ModeType << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->GetInterfaceID(IdOfInterface))
				{
					std::cout << IdOfInterface << std::endl;
				}
			}
		}
	}
	if (VmbErrorSuccess == system1.Shutdown())
	{
		cout << "system1 is shutdown" << endl;
	}

	//Open camera
	CameraPtrVector cameras2;
	VimbaSystem &system2 = VimbaSystem::GetInstance();
	if (VmbErrorSuccess == system2.Startup())
	{
		if (VmbErrorSuccess == system2.GetCameras(cameras2))
		{
			for (CameraPtrVector::iterator iter = cameras2.begin();
				cameras2.end() != iter;
				++iter)
			{
				if (VmbErrorSuccess == (*iter)->Open(VmbAccessModeFull))
				{
					std::cout << "Cameras2 opened" << std::endl;
				}
				if (VmbErrorSuccess == (*iter)->Close())
				{
					std::cout << "Cameras2 closed by Close()" << std::endl;
				}

			}
		}
	}
	if (VmbErrorSuccess == system2.Shutdown())
	{
		cout << "system2 is shutdown" << endl;
	}
	//Open a camera by IP
	CameraPtr camera3;

	VimbaSystem &system3 = VimbaSystem::GetInstance();
	if (VmbErrorSuccess == system3.Startup())
	{
		if (VmbErrorSuccess == system3.OpenCameraByID("169.254.127.127",
			VmbAccessModeFull,
			camera3))
		{
			std::cout << "Camera3 opened by IP" << std::endl;
		}
		if (VmbErrorSuccess == camera3->Close())
		{
			std::cout << "Camera3 is closed by Close()" << std::endl;
		}
	}
	if (VmbErrorSuccess == system3.Shutdown())
	{
		cout << "system3 is shutdown" << endl;
	}

	//read camera feature

	FeaturePtr feature4;
	VmbInt64_t width;
	VmbInt64_t height;

	CameraPtr camera4;
	VimbaSystem &system4 = VimbaSystem::GetInstance();
	system4.Startup();
	system4.OpenCameraByID("169.254.127.127", VmbAccessModeFull, camera4);
	if (VmbErrorSuccess == camera4->GetFeatureByName("Width", feature4))//通过名字访问 27-28页
	{
		if (VmbErrorSuccess == feature4->GetValue(width))
		{
			cout << width << endl;
		}
	}
	if (VmbErrorSuccess == camera4->GetFeatureByName("Height", feature4))
	{
		if (VmbErrorSuccess == feature4->GetValue(height))
		{
			cout << height << endl;
		}
	}

	if (VmbErrorSuccess == camera4->Close())
	{
		std::cout << "Camera4 is closed by Close()" << std::endl;
	}
	if (VmbErrorSuccess == system4.Shutdown())
	{
		cout << "system4 is shutdown" << endl;
	}

	//writing features to camera and running a command feature

	FeaturePtr feature5;
	CameraPtr camera5;
	VimbaSystem &system5 = VimbaSystem::GetInstance();
	system5.Startup();
	system5.OpenCameraByID("169.254.127.127", VmbAccessModeFull, camera5);
	if (VmbErrorSuccess == camera5->GetFeatureByName("AcquisitionMode", feature5))
	{
		if (VmbErrorSuccess == feature5->SetValue("Continuous"))
		{
			if (VmbErrorSuccess == camera5->GetFeatureByName("AcquisitionStart",
				feature5))
			{
				if (VmbErrorSuccess == feature5->RunCommand())
				{
					cout << "Acquisition started" << endl;
				}
			}
		}
	}
	if (VmbErrorSuccess == camera5->Close())
	{
		std::cout << "Camera5 is closed by Close()" << std::endl;
	}
	if (VmbErrorSuccess == system4.Shutdown())
	{
		cout << "system5 is shutdown" << endl;
	}
	*/
	//异步抓取图像
	VmbInt64_t nPLS; // 有效负债大小值
	FeaturePtr pFeature; // 通用功能指针
	
	VimbaSystem &sys = VimbaSystem::GetInstance();//系统初始化
	CameraPtrVector cameras;//相机向量数组
	CameraPtr camera;//单个相机
	FramePtrVector frames(15);//帧向量数组
	sys.Startup();//打开API
	sys.GetCameras(cameras);//获取所有相机
	camera = cameras[0];//将相机一赋值给当个相机量
	camera->Open(VmbAccessModeFull);//打开相机，按照读写模式
	camera->GetFeatureByName("PayloadSize", pFeature);//有效字节数
	pFeature->GetValue(nPLS);//有效负载大小值

	for (FramePtrVector::iterator iter = frames.begin(); frames.end() != iter; ++iter)
	{
		(*iter).reset(new Frame(nPLS));
		(*iter)->RegisterObserver(IFrameObserverPtr(new  FrameObserver1(camera)));
		camera->AnnounceFrame(*iter);
	}



	/*
	for (CameraPtrVector::iterator iter = cameras.begin();
		cameras.end() != iter;
		++iter)
	{
		if (VmbErrorSuccess == (*iter)->Close())
		{
			std::cout << "Cameras closed by Close()" << std::endl;
		}
	}
	if (VmbErrorSuccess == sys.Shutdown())
	{
		cout << "sys is shut down" << endl;
	}
	*/

	getchar();
	return 0;
}