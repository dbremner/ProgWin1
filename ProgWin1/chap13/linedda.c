/* LINEDDA.C -- LineDDA Demonstration */

#include <windows.h>

long CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void CALLBACK LineProc (short, short, LPSTR) ;

HANDLE hInst ;

int CALLBACK WinMain (hInstance, hPrevInstance, lpszCmdLine, nCmdShow)
     HANDLE      hInstance, hPrevInstance ;
     LPSTR       lpszCmdLine ;
     int         nCmdShow ;
     {
     static char szAppName [] = "LineDDA" ;
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
          wndclass.hIcon         = NULL ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szAppName ;

          if (!RegisterClass (&wndclass))
               return FALSE ;
          }

     hInst = hInstance ;

     hWnd = CreateWindow (szAppName, "LineDDA Demonstration",
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, 0,
                         CW_USEDEFAULT, 0,
                         NULL, NULL, hInstance, NULL) ;

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
     HWND           hWnd ;
     unsigned       iMessage ;
     WPARAM wParam ;
     LPARAM lParam ;
     {
     static short   xClient, yClient, xL, xR, yT, yB ;
     HDC            hDC ;
     PAINTSTRUCT    ps ;

     switch (iMessage)
          {
          case WM_CREATE:
               break ;

          case WM_SIZE:
               xR = 3 * (xL = (xClient = LOWORD (lParam)) / 4) ;
               yB = 3 * (yT = (yClient = HIWORD (lParam)) / 4) ;
               break ;

          case WM_PAINT:
               hDC = BeginPaint (hWnd, &ps) ;

               LineDDA (xL, yT, xR, yT, LineProc, (LPSTR) &hDC) ;
               LineDDA (xR, yT, xR, yB, LineProc, (LPSTR) &hDC) ;
               LineDDA (xR, yB, xL, yB, LineProc, (LPSTR) &hDC) ;
               LineDDA (xL, yB, xL, yT, LineProc, (LPSTR) &hDC) ;

               LineDDA (0,       0,       xL, yT, LineProc, (LPSTR) &hDC) ;
               LineDDA (xClient, 0,       xR, yT, LineProc, (LPSTR) &hDC) ;
               LineDDA (xClient, yClient, xR, yB, LineProc, (LPSTR) &hDC) ;
               LineDDA (0,       yClient, xL, yB, LineProc, (LPSTR) &hDC) ;

               EndPaint (hWnd, &ps) ;
               break ;

          case WM_DESTROY:
               PostQuitMessage (0) ;
               break ;

          default:
               return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
          }
     return 0L ;
     }

void CALLBACK LineProc (x, y, lpData)
     short        x, y ;
     LPSTR        lpData ;
     {
     static short nCounter = 0 ;

     if (nCounter == 2)
          Ellipse (* (HDC far *) lpData, x - 2, y - 2, x + 3, y + 3) ;

     nCounter = (nCounter + 1) % 4 ;
     }
