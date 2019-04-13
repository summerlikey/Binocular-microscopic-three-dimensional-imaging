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
			}
		}
	}
	getchar();
	return 0;
}