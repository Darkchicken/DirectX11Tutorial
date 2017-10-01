#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* system;
	bool result;

	//create system object
	system = new SystemClass;
	if (!system)
	{
		return 0;
	}

	//initialize and run system object
	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	//shutdown and release system object
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}