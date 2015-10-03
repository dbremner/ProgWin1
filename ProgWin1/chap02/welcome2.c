/*  WELCOME2.C -- Welcome to Windows Program No. 2 */

#include <windows.h>

long CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int CALLBACK WinMain (hInstance, hPrevInstance, lpszCmdLine, nCmdShow)
     HANDLE      hInstance, hPrevInstance ;
     LPSTR       lpszCmdLine ;
     int         nCmdShow ;
     {
     static char szAppName [] = "Welcome2" ;
     HWND        hWnd ;
     MSG         msg ;
     WNDCLASS    wndclass ;

     if (!hPrevInstance) 
          {
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
          wndclass.lpfnWndProc   = WndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = 0 ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szAppName ;

          if (!RegisterClass (&wndclass))
               return FALSE ;
          }

     hWnd = CreateWindow (szAppName,         /* window class            */
                    "Welcome to Windows",    /* window caption          */
                    WS_OVERLAPPEDWINDOW,     /* window style            */
                    CW_USEDEFAULT,           /* initial x position      */
                    0,                       /* initial y position      */
                    CW_USEDEFAULT,           /* initial x size          */
                    0,                       /* initial y size          */
                    NULL,                    /* parent window handle    */
                    NULL,                    /* window menu handle      */
                    hInstance,               /* program instance handle */
                    NULL) ;                  /* create parameters       */

     ShowWindow (hWnd, nCmdShow) ;
     UpdateWindow (hWnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

long CALLBACK WndProc (hWnd, iMessage, wParam, lParam)
     HWND     hWnd ;
     UINT iMessage ;
     WPARAM  wParam ;
     LPARAM  lParam ;
     {
     switch (iMessage)
          {
          case WM_DESTROY:
               PostQuitMessage (0) ;
               break ;

          default:
               return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
          }
     return 0L ;
     }
