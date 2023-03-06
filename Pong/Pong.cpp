// Pong.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Pong.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR w_ball_t[] = L"BALLCLASS";
WCHAR w_paddle_t[] = L"PADDLECLASS";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				MyRegisterClass_Ball(HINSTANCE hInstance);
ATOM				MyRegisterClass_Paddle(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    BallProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PaddleProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumChildProcPaddle(HWND hwndChild, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PONG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClass_Ball(hInstance);
    MyRegisterClass_Paddle(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PONG));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

HWND hWnd, hBall, hPaddle;

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex; //main window

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(255,255,0));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterClass_Ball(HINSTANCE hInstance)
{
    WNDCLASSEX ball;

    ball.cbSize = sizeof(WNDCLASSEX);

    ball.style = CS_HREDRAW | CS_VREDRAW;
    ball.lpfnWndProc = BallProc;
    ball.cbClsExtra = 0;
    ball.cbWndExtra = 0;
    ball.hInstance = hInstance;
    ball.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONG));
    ball.hCursor = LoadCursor(nullptr, IDC_ARROW);
    ball.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
    ball.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
    ball.lpszClassName = w_ball_t;
    ball.hIconSm = LoadIcon(ball.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&ball);
}

ATOM MyRegisterClass_Paddle(HINSTANCE hInstance)
{
    WNDCLASSEX paddle;

    paddle.cbSize = sizeof(WNDCLASSEX);

    paddle.style = CS_HREDRAW | CS_VREDRAW;
    paddle.lpfnWndProc = WndProc;
    paddle.cbClsExtra = 0;
    paddle.cbWndExtra = 0;
    paddle.hInstance = hInstance;
    paddle.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONG));
    paddle.hCursor = LoadCursor(nullptr, IDC_ARROW);
    paddle.hbrBackground = (HBRUSH)(COLOR_WINDOW - 1);
    paddle.lpszMenuName = MAKEINTRESOURCEW(IDC_PONG);
    paddle.lpszClassName = w_paddle_t;
    paddle.hIconSm = LoadIcon(paddle.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&paddle);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, L"PONG", WS_OVERLAPPEDWINDOW | WS_EX_CONTROLPARENT,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
  
    hBall = CreateWindow(w_ball_t, L"BALL", WS_CHILD | WS_VISIBLE,
        CW_USEDEFAULT, 0, 50, 100, hWnd, nullptr, hInstance, nullptr);
    
    hPaddle = CreateWindow(w_paddle_t, L"PADDLE", WS_CHILD | WS_VISIBLE,
        58, 226, 80, 15, hWnd, nullptr, hInstance, nullptr);
   
    RECT rc;
    GetWindowRect(hWnd, &rc);
    int x = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

    MoveWindow(hWnd, x, y, 200, 300, 0);

    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, 0, (255 * 80) / 100, LWA_ALPHA);

    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hBall, nCmdShow);
    if (!hBall)
    {
        return FALSE;
    }
    if (!hPaddle)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
            break;

        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
            minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = 200;
            minMaxInfo->ptMaxSize.x = minMaxInfo->ptMinTrackSize.x = 200;
            minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = 300;
            minMaxInfo->ptMaxSize.y = minMaxInfo->ptMinTrackSize.y = 300;
        }
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

LRESULT CALLBACK BallProc(HWND hBall, UINT message, WPARAM wParam, LPARAM lParam)
{
        static int x = 10;
        static int y = 10;
        static int lx = 0;
        static int rx = 20;
        static int ty = 0;
        static int by = 20;
        switch (message)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hBall, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hBall, &ps);
        }
        break;
        case WM_CREATE:
        {
            MoveWindow(hBall, 80, 140, 20, 20, NULL);
            SetTimer(hBall, 7, 50, NULL);
            //Set region on window hBall
            HRGN region = CreateEllipticRgn(0, 0, 20, 20);
            SetWindowRgn(hBall, region, true);
            break;
        }
        case WM_TIMER:
        {
            if (rx >= 190)
                x = -abs(x);
            if (lx <= 0)
                x = abs(x);
            if (by >= 226)
                y = -abs(y);
            if (ty <= 0)
                y = abs(y);

            lx = lx + x;
            ty = ty + y;
            rx = rx + x;
            by = by + y;
            MoveWindow(hBall, lx, ty, rx - lx, by - ty, TRUE);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hBall, message, wParam, lParam);
        }
        return 0;
}

LRESULT CALLBACK PaddleProc(HWND hPaddle, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hPaddle, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hPaddle, &ps);
        }
        break;
        case WM_CREATE:
        {
            TRACKMOUSEEVENT paddle;
            paddle.cbSize = sizeof(TRACKMOUSEEVENT);
            paddle.dwFlags = TME_HOVER;
            paddle.hwndTrack = hWnd;
            paddle.dwHoverTime = HOVER_DEFAULT;

            if (TrackMouseEvent(&paddle) == false)
                SetWindowText(hWnd, L"TrackMouseEvent failed");

            break;
        }
        case WM_MOUSEHOVER:
        {
        case WM_MOUSEMOVE:
        {
           /* if (GetAsyncKeyState('W') & 0x8000)
            {
                hPaddle -= 5;
            }
            if (GetAsyncKeyState('S') & 0x8000)
            {
                hPaddle += 5;
            }*/
            POINT pos;
            GetCursorPos(&pos);
            ScreenToClient(hPaddle, &pos);

            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            EnumChildWindows(hWnd, EnumChildProcPaddle, (LPARAM)&pos);

            //if (res)
            //{
            //    ScreenToClient(hPaddle, &pos);
            //    /*TextOut(GetDC(hPaddle), x, y,
            //        message,
            //        _tcslen(message));*/
            //}
            
            break;
        }
        break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hPaddle, message, wParam, lParam);
        }
        return 0;
}

BOOL CALLBACK EnumChildProcPaddle(HWND hwndChild, LPARAM lParam)
{
    //LPRECT rcParent;
    int i, idChild;

    // Retrieve the child-window identifier. Use it to set the 
    // position of the child window. 

    idChild = GetWindowLong(hwndChild, GWL_ID);



    LPPOINT paddle = (LPPOINT)lParam;
    if (idChild == 100 && 101) {

        MoveWindow(hwndChild, (LONG)paddle->x - 25, 220, 50, 20, TRUE);

    }


    // Make sure the child window is visible. 

    ShowWindow(hwndChild, SW_SHOW);

    return TRUE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
