#include "pch.h"
#include <tchar.h>
#include <iostream >
#include <stdio.h>
#include "VimbaCPP/Include/VimbaCPP.h"
using namespace std;
using namespace AVT::VmbAPI;
int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Hello Vimba" << std::endl;
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
	VimbaSystem &system2=VimbaSystem::GetInstance();
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



	getchar();
	return 0;
}