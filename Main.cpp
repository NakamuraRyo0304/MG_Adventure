//
// Main.cpp
//

#include "pch.h"
#include "Game.h"

using namespace DirectX;

#define         WS_MY_FULL       {SWP_NOMOVE        |\
                                  SWP_NOSIZE        |\
                                  SWP_NOZORDER      |\
                                  SWP_FRAMECHANGED  }

namespace
{
    std::unique_ptr<Game> g_game;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;           // nVidia
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;     // AMD
}


// ���[�N�G���A�̈�̎擾
#define GetMonitorRect(rc)  SystemParametersInfo(SPI_GETWORKAREA,0,rc,0)

// �f�X�N�g�b�v�̉�ʒ����Ɉړ�
bool DesktopCenterWindow(HWND hWnd)
{
    RECT    rc1;                                            // �f�X�N�g�b�v�̈�
    RECT    rc2;                                            // �E�C���h�E�̈�
    INT     cx, cy;                                         // �E�C���h�E�ʒu
    INT     sx, sy;                                         // �E�C���h�E�T�C�Y

    // �T�C�Y�̎擾
    GetMonitorRect(&rc1);                                   // �f�X�N�g�b�v�̃T�C�Y
    GetWindowRect(hWnd, &rc2);                              // �E�C���h�E�̃T�C�Y

    // �T�C�Y�ƒ��S�ʒu���Z�o
    sx = (rc2.right - rc2.left);                            // �E�C���h�E�̉���
    sy = (rc2.bottom - rc2.top);                            // �E�C���h�E�̍���
    cx = (((rc1.right - rc1.left) - sx) / 2 + rc1.left);    // �������̒������W��
    cy = (((rc1.bottom - rc1.top) - sy) / 2 + rc1.top);     // �c�����̒������W��

    // ��ʒ����Ɉړ�
    return SetWindowPos(hWnd, NULL, cx, cy, 0, 0, (SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER));
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
//!! �ǋL
#if defined(_DEBUG)
    ::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!XMVerifyCPUSupport())
        return 1;

    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr))
        return 1;

    HWND g_screen;
    g_game = std::make_unique<Game>();

    // Register class and create window
    {
        // Register class
        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
        wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        wcex.lpszClassName = L"_3DPGWindowClass";
        wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
        if (!RegisterClassExW(&wcex))
            return 1;

        // Create window
        int w, h;
        g_game->GetDefaultSize(w, h);

        RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };

        AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

        // �X�N���[���p�t���O
        bool fullFlag = false;

        // ��ʃ��[�h�I��
        if (MessageBox(NULL,
            L"�t���X�N���[���ɂ��܂����H", L"��ʃ��[�h�ݒ�",
            MB_YESNO) == IDYES)
        {
            // �t���X�N���[���ɂ���
            fullFlag = true;
        }
        else
        {
            // �t���X�N���[���ɂ��Ȃ�
            fullFlag = false;
        }      
        
        HWND hwnd = CreateWindowExW(
            0, 
            L"_3DPGWindowClass", 
            Game::TITLE, //L"3DPG", 
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            rc.right - rc.left, 
            rc.bottom - rc.top, 
            nullptr, 
            nullptr, 
            hInstance,
            nullptr
        );

    	// �f�X�N�g�b�v�̒��S�ɃX�N���[���������Ă���
        DesktopCenterWindow(hwnd);
    	
        if (!hwnd)
            return 1;

        // �t���X�N���[�����[�h
        if (fullFlag)
        {
            SetWindowLongPtr(hwnd, GWL_STYLE, 0);
            SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

            SetWindowPos(hwnd, 
                HWND_TOP, 
                0, 0, 0, 0, 
                WS_MY_FULL);

            ShowWindow(hwnd, SW_SHOWMAXIMIZED);
        }
        // �E�B���h�E���[�h
        else
        {
            ShowWindow(hwnd, nCmdShow);
        }

        g_screen = hwnd;

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()) );

        GetClientRect(hwnd, &rc);

        g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
    }

    // ���C�����b�Z�[�W���[�v
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_game->Tick();
        }
    }

    ShowWindow(g_screen, nCmdShow);
    g_game.reset();

    CoUninitialize();

    return static_cast<int>(msg.wParam);
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    static bool s_fullscreen = false;

    auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_PAINT:
        if (s_in_sizemove && game)
        {
            game->Tick();
        }
        else
        {
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOVE:
        if (game)
        {
            game->OnWindowMoved();
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && game)
                    game->OnSuspending();
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && game)
                game->OnResuming();
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && game)
        {
            game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (game)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_GETMINMAXINFO:
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_ACTIVATEAPP:
        if (game)
        {
            if (wParam)
            {
                game->OnActivated();
            }
            else
            {
                game->OnDeactivated();
            }
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && game)
                game->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && game)
                    game->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        DirectX::Mouse::ProcessMessage(message, wParam, lParam);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
        break;


    case WM_SYSKEYDOWN:
        if (s_fullscreen)
        {
            SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

            int width = 1280;
            int height = 720;
            if (game)
                game->GetDefaultSize(width, height);

            ShowWindow(hWnd, SW_SHOWNORMAL);

            SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
        break;

    case WM_MENUCHAR:
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


// Exit helper
void ExitGame() noexcept
{
    PostQuitMessage(0);
}