#define _CRT_SECURE_NO_WARNINGS
#include "systemclass.h"
#include <fstream> 
using namespace std; 
#pragma warning(disable : 4005 )

  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	SystemClass* System;
	bool result;

	//	Create the system object. 
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}
	
	//	Initialize and run the system object. 
	result = System->Initialize();
	
	if(result)
	{
		System->Run();
	}

	//	Shutdown and release the system object. 
	System->Shutdown();
	delete System;

	System = 0;

	return 0;
}