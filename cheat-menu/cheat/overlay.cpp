// overlay.cpp
#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "overlay.h"
#include "cheat.h"

HWND overlayWindow = nullptr;
ID2D1Factory* pFactory = nullptr;
ID2D1HwndRenderTarget* pRenderTarget = nullptr;
ID2D1SolidColorBrush* pBrush = nullptr;
bool running = true;

HWND FindGameWindow() {
    return FindWindowA(nullptr, "AssaultCube");
}

void overlay::InitOverlay() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        "OverlayWindow", NULL };
    RegisterClassEx(&wc);

    HWND gameWnd = FindGameWindow();
    if (!gameWnd) return;

    RECT gameRect;
    GetWindowRect(gameWnd, &gameRect);

    overlayWindow = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName, "", WS_POPUP,
        gameRect.left, gameRect.top,
        gameRect.right - gameRect.left, gameRect.bottom - gameRect.top,
        NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(overlayWindow, SW_SHOW);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

    RECT rect;
    GetClientRect(overlayWindow, &rect);
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(overlayWindow,
            D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top));

    pFactory->CreateHwndRenderTarget(props, hwndProps, &pRenderTarget);
    pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pBrush);
}

void overlay::ShutdownOverlay() {
    if (pBrush) pBrush->Release();
    if (pRenderTarget) pRenderTarget->Release();
    if (pFactory) pFactory->Release();
    if (overlayWindow) DestroyWindow(overlayWindow);
}

void overlay::DrawFovCircle(float radius, const RECT& rect)
{
    if (!pRenderTarget || !pBrush) return;

    float width = static_cast<float>(rect.right - rect.left);
    float height = static_cast<float>(rect.bottom - rect.top);

    float centerX = width / 2.0f;
    float centerY = height / 2.0f;

    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(centerX, centerY), radius, radius);

    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(0, 0));
    pRenderTarget->DrawEllipse(&ellipse, pBrush, 2.0f);
    pRenderTarget->EndDraw();
}


void overlay::OverlayLoop() {
    HWND gameWnd = FindGameWindow();
    if (!gameWnd) return;

    static RECT lastRect = { 0 };

    while (running) {
        HWND foreground = GetForegroundWindow();

        if (foreground != gameWnd) {
            ShowWindow(overlayWindow, SW_HIDE); // Masquer quand pas dans le jeu
            Sleep(50);
            continue;
        }
        else {
            ShowWindow(overlayWindow, SW_SHOW); // Afficher si en focus
        }

        // Suivre la position du jeu
        RECT gameRect;
        GetWindowRect(gameWnd, &gameRect);
        int width = gameRect.right - gameRect.left;
        int height = gameRect.bottom - gameRect.top;

        if (memcmp(&lastRect, &gameRect, sizeof(RECT)) != 0) {
            MoveWindow(overlayWindow, gameRect.left, gameRect.top,
                gameRect.right - gameRect.left, gameRect.bottom - gameRect.top, TRUE);
            lastRect = gameRect;

            // ✅ Resize render target when resolution changes
            if (pRenderTarget) pRenderTarget->Release();

            RECT clientRect;
            GetClientRect(overlayWindow, &clientRect);
            D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
                D2D1::HwndRenderTargetProperties(overlayWindow,
                    D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));

            pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                hwndProps,
                &pRenderTarget
            );

            if (pBrush) pBrush->Release();
            pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pBrush);
        }

        // ✅ Afficher le cercle uniquement si l'aim assist est activé
        if (cheat::isAimAssistActive)
            overlay::DrawFovCircle(120.0f, gameRect);
        else {
            // Nettoyage de l'écran sinon (pas obligatoire, mais utile visuellement)
            pRenderTarget->BeginDraw();
            pRenderTarget->Clear(D2D1::ColorF(0, 0));
            pRenderTarget->EndDraw();
        }

        Sleep(5); // ne pas surcharger le CPU
    }
}
