#include "gui.h"
#include "cheat.h"
#include "overlay.h"
#include "../cheat/memory.h"
#include <thread>


int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	gui::CreateHWindow("Cheat Menu");
	gui::CreateDevice();
	gui::CreateImGui();

	cheat::getinfoon();
	overlay::InitOverlay();
	std::thread(overlay::OverlayLoop).detach();
	std::thread(cheat::ToggleHotkeyLoop).detach();


	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();


		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();
	overlay::ShutdownOverlay();

	return EXIT_SUCCESS;
}



