#include <windows.h>
#include <string>
#include <iostream>
#include "ProcessManager.h"
#include "DllInjector.h"

// Global variables
HWND hLuaInput, hAttachButton, hExecuteButton;
HINSTANCE hInst;

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    const char CLASS_NAME[] = "LuaExecutorClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Lua Executor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        hLuaInput = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
            10, 10, 460, 200,
            hwnd, NULL, hInst, NULL);

        hAttachButton = CreateWindow("BUTTON", "Attach",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 220, 100, 30,
            hwnd, (HMENU)1, hInst, NULL);

        hExecuteButton = CreateWindow("BUTTON", "Execute",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            120, 220, 100, 30,
            hwnd, (HMENU)2, hInst, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            DWORD processId = FindProcessId("RobloxPlayerBeta.exe");
            if (processId) {
                InjectDll(processId, "InjectedDll.dll");
            } else {
                MessageBox(hwnd, "Roblox process not found!", "Error", MB_OK | MB_ICONERROR);
            }
        }
        else if (LOWORD(wParam) == 2) {
            char luaCode[1024];
            GetWindowText(hLuaInput, luaCode, sizeof(luaCode));
            // Here, you would execute the Lua code in the injected process
            // For demonstration, we just print it
            std::cout << "Executing Lua Code: " << luaCode << std::endl;
        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
