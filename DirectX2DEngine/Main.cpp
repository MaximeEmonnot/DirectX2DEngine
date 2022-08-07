#include "Window.h"
#include "EngineException.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try {
		while (WND.ProcessMessages())
		{

		}
	}
	catch(...)
	{
		SHOW_MESSAGE("DirectX 2D Engine - Unknown Exception", "An unknown exception has been caught.");
	}
	
	return 0;
}