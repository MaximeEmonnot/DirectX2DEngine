#include "Window.h"
#include "EngineException.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try {
		Game theGame;
		while (WND.ProcessMessages()) theGame.Go();
	}
	catch(EngineException& e)
	{
		SHOW_MESSAGE("DirectX 2D Engine - Engine Exception", e.what());
	}
	catch(std::exception& e)
	{
		SHOW_MESSAGE("DirectX 2D Engine - Standard Library Exception", e.what());
	}
	catch(...)
	{
		SHOW_MESSAGE("DirectX 2D Engine - Unknown Exception", "An unknown exception has been caught.");
	}
	
	return 0;
}
