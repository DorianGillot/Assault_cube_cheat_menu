#include <Windows.h>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <CommCtrl.h>
#include <cmath>
#include <thread>
#include <cfloat>
#define PI 3.14159265f
#include "../cheat/offset.h"
#include "../cheat/memory.h"
#include "../cheat/cheat.h"
#include "../imgui/imgui.h"


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
static std::atomic<bool> isAimbotRunning = false;
std::atomic<bool> cheat::isAimAssistActive = false;
//initialisation




struct Vec3 {
    float x, y, z;
};

struct Entity {
    std::uintptr_t base;
    int health;
    int team;
    Vec3 position;
};


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



void cheat::CalculateAngle() {

    auto& memory = getMemory();
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);

    // 📍 Lecture de la tête d’un ennemi (premier dans la liste)
    const auto entityListPtr = memory.Read<std::uintptr_t>(moduleBase + entityList);
    const auto numberPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + numberPlayer);
    const auto playerCount = memory.Read<std::uintptr_t>(numberPlayerPtr + m_count);

    float posX = memory.Read<float>(localPlayerPtr + m_XCoord); // ma tete à moi
    float posY = memory.Read<float>(localPlayerPtr + m_YCoord);
    float posZ = memory.Read<float>(localPlayerPtr + m_ZCoord);

    float viewYaw = memory.Read<float>(localPlayerPtr + m_ViewYaw);
    float viewPitch = memory.Read<float>(localPlayerPtr + m_ViewPitch);

    float smallestFOV = FLT_MAX;
    float closestDistance = FLT_MAX;
    float targetHead[3] = { 0, 0, 0 };



    

    for (int i = 1; i < playerCount; ++i) { //ici la boucle fonctionne de sorte a trouver la les cordonné de chaque ennemie

        printf("[INFO] playerCount = %d\n", playerCount);

        const auto enemyPtr = memory.Read<std::uintptr_t>(entityListPtr + i * 0x4); // Premier ennemi
        if (!enemyPtr) return;

        int health = memory.Read<int>(enemyPtr + m_iHealth);
        if (health <= 0) continue; // ❗ Ne vise que les ennemis vivants


        float eX = memory.Read<float>(enemyPtr + m_XCoord); //coordonné tete ennemie
        float eY = memory.Read<float>(enemyPtr + m_YCoord);
        float eZ = memory.Read<float>(enemyPtr + m_ZCoord);


        //if (screenPos[2] > 1.0f) continue; // trop profond : probablement dans un mur

        float dx = eX - posX;
        float dy = eY - posY;
        float dz = eZ - posZ;

        /*float dist = sqrtf(dx * dx + dy * dy + dz * dz); //un tri (sort by lowest to hightest) rage bot
        if (dist < closestDistance) {
            closestDistance = dist;
            targetHead[0] = eX;
            targetHead[1] = eY;
            targetHead[2] = eZ;
        }*/

        float flatDist = sqrtf(dx * dx + dy * dy+ dz * dz);  // 🎯 FOV max configurable il vise l'ennmie avec langle le plus petit en prmier pas l'ennmie le plus proche (configurable à loisir)

        float yaw = (atan2f(dy, dx)) * (180.0f / PI) + 90.0f; 
        float pitch = (asinf(dz/ flatDist)) * (180.0f / PI); 

        float fovYaw = fabs(viewYaw - yaw); 
        float fovPitch = fabs(viewPitch - pitch); 
        float totalFov = sqrtf(fovYaw * fovYaw + fovPitch * fovPitch); 

        if (totalFov < smallestFOV && totalFov < 15.0f) {  
            smallestFOV = totalFov;
            targetHead[0] = eX;
            targetHead[1] = eY;
            targetHead[2] = eZ;
        }

    }
    
        if (smallestFOV == FLT_MAX)
            return; // ❌ Aucun ennemi dans le champ de vision
        //coder un calcule de distace
    

        float angles[2] = { 0, 0 };

        float f_DeltaX = (targetHead[0] - posX);
        float f_DeltaY = (targetHead[1] - posY);
        float f_DeltaZ = (targetHead[2] - posZ);

        float f_hypothenuse = sqrt(f_DeltaX * f_DeltaX + f_DeltaY * f_DeltaY + f_DeltaZ * f_DeltaZ);

        angles[0] = atan2f(f_DeltaY, f_DeltaX) * (180.0f / PI) + 90.0f; // yaw

        angles[1] = (asinf(f_DeltaZ / f_hypothenuse)) * (180.0f / PI);    // pitch


    // et ici on écrit les le yawn et le pitch pour deplacer le viseur sur l'ennemie le plus proche

    memory.Write<float>(localPlayerPtr + m_ViewYaw, angles[0]); //horizontale
    memory.Write<float>(localPlayerPtr + m_ViewPitch, angles[1]); //verticale


}


void cheat::startAimbot() {
    if (isAimbotRunning) return;
    isAimbotRunning = true;

    std::thread([] {
        while (cheat::isAimAssistActive.load()) {
            cheat::CalculateAngle();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        isAimbotRunning = false;
    }).detach();
}

void cheat::ToggleHotkeyLoop() {
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 1) { // Touche F1, &1 = changement d'état (pas maintenu)
            isAimAssistActive = !isAimAssistActive;

            if (isAimAssistActive) {
                printf("[HOTKEY] Aim Assist ACTIVÉ !\n");
                cheat::startAimbot();
            }
            else {
                printf("[HOTKEY] Aim Assist DÉSACTIVÉ !\n");
                cheat::stopAimbot();
            }
        }
        Sleep(100); // Anti-bounce + pas trop de CPU
    }
}

void cheat::stopAimbot() {
    isAimbotRunning = false;
}



