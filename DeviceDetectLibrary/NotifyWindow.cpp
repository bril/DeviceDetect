#include "StdAfx.h"
#include "NotifyWindow.h"

using namespace DeviceDetectLibrary;

static const wchar_t winClass[] = L"NotifyWindow";
static const wchar_t winTitle[] = L"winTitle";

static NotifyWindow* g_NotifyWindow = NULL;

NotifyWindow::NotifyWindow(IDeviceChanged& deviceChanged)
    : hWnd_(NULL)
    , destroy_(false)
    , thread_(ThreadProc)
    , started_()
    , deviceChanged_(deviceChanged)
{
    if (g_NotifyWindow != NULL)
    {
        throw std::runtime_error("Only one instance of the NotifyWindow can be created");
    }
    g_NotifyWindow = this;
    thread_.Start(this);
    started_.Wait(INFINITE);
}

NotifyWindow::~NotifyWindow()
{
    destroy_ = true;
    PostMessage(hWnd_, WM_CLOSE, 0, 0);
    thread_.Wait(INFINITE);
    g_NotifyWindow = NULL;
}

void NotifyWindow::NotifyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= NotifyWindow::WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= NULL;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= winClass;
    wcex.hIconSm		= NULL;

    RegisterClassEx(&wcex);
}

bool NotifyWindow::InitInstance(HINSTANCE hInstance)
{
    hWnd_ = CreateWindow(winClass, winTitle, WS_ICONIC, 0, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd_)
    {
        return false;
    }

    ShowWindow(hWnd_, SW_HIDE);
    UpdateWindow(hWnd_);

    return true;
}

std::vector<std::wstring> DrivesFromMask(DWORD unitmask)
{
    std::vector<std::wstring> result;
    std::wstring path(L"?:\\");
    DWORD localUnitmask(unitmask);
    for (wchar_t i = 0; i < 26; ++i)
    {
        if (0x01 == (localUnitmask & 0x1))
        {
            path[0] = i + L'A';
            result.push_back(path);
        }
        localUnitmask >>= 1;
    }

    return result;
}

LRESULT CALLBACK NotifyWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DEVICECHANGE:
        {
            if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
            {
                try
                {
                    DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
                    if (header->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        DEV_BROADCAST_VOLUME* devNot = reinterpret_cast<DEV_BROADCAST_VOLUME*>(lParam);
                        std::vector<std::wstring> drives = DrivesFromMask(devNot->dbcv_unitmask);
                        for (std::vector<std::wstring>::const_iterator it = drives.begin(); it != drives.end(); ++it)
                        {
                            if (DBT_DEVICEARRIVAL == wParam)
                            {
                                g_NotifyWindow->deviceChanged_.VolumeArrival(*it);
                            }
                            else
                            {
                                g_NotifyWindow->deviceChanged_.VolumeRemoved(*it);
                            }
                        }
                    }
                    if (header->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    {
                        DEV_BROADCAST_DEVICEINTERFACE* devNot = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParam);
                        if (DBT_DEVICEARRIVAL == wParam)
                        {
                            g_NotifyWindow->deviceChanged_.InterfaceArrival(devNot->dbcc_classguid);
                        }
                        else
                        {
                            g_NotifyWindow->deviceChanged_.InterfaceRemoved(Utilities::StringUpper(devNot->dbcc_name));
                        }
                    }
                }
                catch(const std::runtime_error& /*ex*/)
                {
                    //"ERROR: Processing WM_DEVICECHANGE failed
                }
            }
        }
        break;
    case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
    default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

HWND NotifyWindow::GetHWND() const
{
    return hWnd_;
}


void  NotifyWindow::ThreadProc( void* context )
{
    NotifyWindow* This = static_cast<NotifyWindow*>(context);
    HINSTANCE hInstance = ::GetModuleHandle(NULL);
    This->NotifyRegisterClass(hInstance);
    if (!This->InitInstance (hInstance))
    {
        return;
    }
    This->started_.Set();
    // Main message loop:
    MSG msg;
    BOOL bRet(FALSE);
    while( (bRet = GetMessage( &msg, This->hWnd_, 0, 0 )) != 0)
    { 
        if(This->destroy_)
        {
            break;
        }
        if (bRet == -1)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    }
    return;
}