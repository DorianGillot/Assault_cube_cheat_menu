#include <Windows.h>
#include <vector>
#include <iostream>
#include <CommCtrl.h>
#include "../cheat/offset.h"
#include "../cheat/memory.h"
#include "../cheat/cheat.h"


int cheat::updatedHealth = 100; //initalisation
int cheat::updatedNade = 0;
int cheat::updatedAmmo = 20;
int initialHealth = 0;
int initialNade = 0;
int initialAmmo = 0;
bool cheat::isNoRecoilOn = false; // check 
bool cheat::isInfNadeOn = false;
bool cheat::isInfAmmoOn = false;
bool cheat::isGodModeOn = false;  
bool cheat::isGetInfoOn = false;
std::uintptr_t cheat::headPtr = 0;
std::uintptr_t cheat::entityL = 0;
std::uintptr_t cheat::entity = 0;
float cheat::headValue = 0.0f;

void cheat::godmodeon() noexcept
{
    if (isGodModeOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto healthAddress = localPlayerPtr + m_iHealth;

    initialHealth = memory.Read<int>(healthAddress);

    isGodModeOn = true;

    int updatedHealth = 9999;
    memory.Write<int>(healthAddress, updatedHealth);
}


void cheat::godmodeoff() noexcept
{
    if (!isGodModeOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto healthAddress = localPlayerPtr + m_iHealth;

    isGodModeOn = false;

    memory.Write<int>(healthAddress, initialHealth);
}

void cheat::infnadeon() noexcept 
{
    if (isInfNadeOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto nadeAddress = localPlayerPtr + m_Nades;

    initialNade = memory.Read<int>(nadeAddress);

    isInfNadeOn = true;

    int updatedNade = 9999;
    memory.Write<int>(nadeAddress, updatedNade);
}


void cheat::infnadeoff() noexcept
{
    if (!isInfNadeOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto nadeAddress = localPlayerPtr + m_Nades;
    
    isInfNadeOn = false;

    memory.Write<int>(nadeAddress, updatedNade);
}

void cheat::infammoon() noexcept
{
    if (isInfAmmoOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto AmmoAddress = localPlayerPtr + m_Ammo;

    initialAmmo = memory.Read<int>(AmmoAddress);

    isInfAmmoOn = true;

    int updatedAmmo = 9999;
    memory.Write<int>(AmmoAddress, updatedAmmo);
}

void cheat::infammooff() noexcept
{
    if (!isInfAmmoOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto AmmoAddress = localPlayerPtr + m_Ammo;

    isInfAmmoOn = false;

    memory.Write<int>(AmmoAddress, updatedAmmo);
}



void cheat::norecoilon() noexcept 
{
    if (isNoRecoilOn)
        return;

    auto& memory = getMemory();
    DWORD_PTR moduleBase = memory.GetModuleAddress("ac_client.exe");
    DWORD oldProtect;
    //BYTE nopArray[10] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    std::vector<BYTE> NOPBytes = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    
    if (memory.ChangeMemoryProtection(moduleBase + 0x63786, NOPBytes.size(), PAGE_EXECUTE_READWRITE, oldProtect)) {
        memory.Write3(moduleBase + 0x63786, NOPBytes);
        memory.ChangeMemoryProtection(moduleBase + 0x63786, NOPBytes.size(), oldProtect, oldProtect);;
        isNoRecoilOn = true;
    }
}

void cheat::norecoiloff() noexcept 
{
    if (!isNoRecoilOn)
        return;

    auto& memory = getMemory();
    DWORD_PTR moduleBase = memory.GetModuleAddress("ac_client.exe");
    DWORD oldProtect;
    std::vector<BYTE> originalBytes = { 0x50, 0x8D, 0x4C, 0x24, 0x1C, 0x51, 0x8B, 0xCE, 0xFF, 0xD2 };

    if (memory.ChangeMemoryProtection(moduleBase + 0x63786, originalBytes.size(), PAGE_EXECUTE_READWRITE, oldProtect)) {
        memory.Write3(moduleBase + 0x63786, originalBytes);
        memory.ChangeMemoryProtection(moduleBase + 0x63786, originalBytes.size(), oldProtect, oldProtect);
        isNoRecoilOn = false;
    }
}


void cheat::getinfoon() noexcept {
    if (isGetInfoOn)
        return;

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto entityListPtr = memory.Read<std::uintptr_t>(moduleBase + entityList);
    const auto entityPtr = memory.Read<std::uintptr_t>(entityListPtr + 0x4);
    headPtr = entityPtr + 0xC;
    cheat::headValue = memory.Read<float>(cheat::headPtr);


    entity = headPtr;
    entityL = entityList;

    isGetInfoOn = true; // Indique que les informations sont récupérées
}


