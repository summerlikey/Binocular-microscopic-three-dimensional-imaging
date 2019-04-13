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
	VimbaSystem& system = VimbaSystem::GetInstance();
	VmbVersionInfo_t version;
	if (VmbErrorSuccess == system.QueryVersion(version))
	{
		std::cout << "Version:" << version.major << "." << version.minor << std::endl;
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

	getchar();
	return 0;
}