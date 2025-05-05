#pragma once
#include "memory.h"
#include "../imgui/imgui.h"
#include <atomic>

namespace cheat
{
	extern int updatedHealth;
	void godmodeon() noexcept;
	void godmodeoff() noexcept;
	extern bool isGodModeOn;
	

	extern int updatedNade;
	void infnadeon() noexcept;
	void infnadeoff() noexcept;
	extern bool isInfNadeOn;

	extern int updatedAmmo;
	void infammoon() noexcept;
	void infammooff() noexcept;
	extern bool isInfAmmoOn;

	void norecoilon() noexcept;
	void norecoiloff() noexcept;
	extern bool isNoRecoilOn;

	void getinfoon() noexcept;
	extern bool isGetInfoOn;
	extern std::uintptr_t headPtr; // Pour stocker les pointeurs vers les têtes des ennemis
	extern std::uintptr_t entityL;
	extern std::uintptr_t entity;
	extern float headValue;

	//void DebugCalculateAngle();
	void CalculateAngle();
	void startAimbot();
	void ToggleHotkeyLoop();
	void stopAimbot();

	extern std::atomic<bool> isAimAssistActive;

	bool WorldToScreen(const float* pos, float* screen, const float* matrix, int windowWidth, int windowHeight);


}


