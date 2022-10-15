#include "MainEngine/Engine.h"
#include "CoreEngine/Window.h"
#include "DebugEngine/EngineException.h"
#include "MainEngine/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try {
		Game theGame;
		while (WND.ProcessMessages()) theGame.Go();
		ENGINE.Clear();
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
