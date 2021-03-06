/* SYSMETS3.C -- System Metrics Display Program No. 3 */

#include <windows.h>
#include <stdio.h>
#include "sysmets.h"

long CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int CALLBACK WinMain (hInstance, hPrevInstance, lpszCmdLine, nCmdShow)
     HANDLE   hInstance, hPrevInstance ;
     LPSTR    lpszCmdLine ;
     int      nCmdShow ;
     {
     WNDCLASS wndclass ;
     HWND     hWnd ;
     MSG      msg ;
     static   char szAppName [] = "SysMets3" ;

     if (!hPrevInstance) 
          {
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
          wndclass.lpfnWndProc   = WndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = 0 ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szAppName ;

          if (!RegisterClass (&wndclass))
               return FALSE ;
          }

     hWnd = CreateWindow (szAppName, "Get System Metrics",
                         WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
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

#define MAXWIDTH 60      /* maximum width of formatted output */

long CALLBACK WndProc (hWnd, iMessage, wParam, lParam)
     HWND         hWnd ;
     unsigned     iMessage ;
     WPARAM  wParam ;
     LPARAM  lParam ;
     {
     static short xChar, yChar ;
     static short xClient, yClient ;
     static short nVscrollMax, nHscrollMax ;
     static short nVscrollPos, nHscrollPos ;
     char         szBuffer [80] ;
     HDC          hDC ;
     PAINTSTRUCT  ps ;
     short        nVscrollInc, nHscrollInc ;
     short        nPaintBeg, nPaintEnd, i ;
     TEXTMETRIC   tm ;

     switch (iMessage)
          {
          case WM_CREATE:
               hDC = GetDC (hWnd) ;
               GetTextMetrics (hDC, &tm) ;
               xChar = tm.tmAveCharWidth ;
               yChar = tm.tmHeight + tm.tmExternalLeading ;
               ReleaseDC (hWnd, hDC) ;
               break ;

          case WM_SIZE:
               yClient = HIWORD (lParam) ;
               xClient = LOWORD (lParam) ;

               nVscrollMax = max (0, NUMLINES + 2 - yClient / yChar) ;
               nVscrollPos = min (nVscrollPos, nVscrollMax) ;
               nHscrollMax = max (0, MAXWIDTH + 2 - xClient / xChar) ;
               nHscrollPos = min (nHscrollPos, nHscrollMax) ;

               SetScrollRange (hWnd, SB_VERT, 0, nVscrollMax, FALSE) ;
               SetScrollPos   (hWnd, SB_VERT, nVscrollPos, TRUE) ;
               SetScrollRange (hWnd, SB_HORZ, 0, nHscrollMax, FALSE) ;
               SetScrollPos   (hWnd, SB_HORZ, nHscrollPos, TRUE) ;
               break ;

          case WM_VSCROLL:
               switch (wParam)
                    {
                    case SB_TOP:
                         nVscrollInc = -nVscrollPos ;
                         break ;

                    case SB_BOTTOM:
                         nVscrollInc = nVscrollMax - nVscrollPos ;
                         break ;

                    case SB_LINEUP:
                         nVscrollInc = -1 ;
                         break ;

                    case SB_LINEDOWN:
                         nVscrollInc = 1 ;
                         break ;

                    case SB_PAGEUP:
                         nVscrollInc = min (-1, -yClient / yChar) ;
                         break ;

                    case SB_PAGEDOWN:
                         nVscrollInc = max (1, yClient / yChar) ;
                         break ;

                    case SB_THUMBTRACK:
                         nVscrollInc = LOWORD (lParam) - nVscrollPos ;
                         break ;

                    default:
                         nVscrollInc = 0 ;
                    }
               if (nVscrollInc = max (-nVscrollPos,
                         min (nVscrollInc, nVscrollMax - nVscrollPos)))
                    {
                    nVscrollPos += nVscrollInc ;
                    ScrollWindow (hWnd, 0, -yChar * nVscrollInc, NULL, NULL) ;
                    SetScrollPos (hWnd, SB_VERT, nVscrollPos, TRUE) ;
                    UpdateWindow (hWnd) ;
                    }
               break ;

          case WM_HSCROLL:
               switch (wParam)
                    {
                    case SB_LINEUP:
                         nHscrollInc = -1 ;
                         break ;

                    case SB_LINEDOWN:
                         nHscrollInc = 1 ;
                         break ;

                    case SB_PAGEUP:
                         nHscrollInc = -8 ;
                         break ;

                    case SB_PAGEDOWN:
                         nHscrollInc = 8 ;
                         break ;

                    case SB_THUMBPOSITION:
                         nHscrollInc = LOWORD (lParam) - nHscrollPos ;
                         break ;

                    default:
                         nHscrollInc = 0 ;
                    }
               if (nHscrollInc = max (-nHscrollPos,
                         min (nHscrollInc, nHscrollMax - nHscrollPos)))
                    {
                    nHscrollPos += nHscrollInc ;
                    ScrollWindow (hWnd, -xChar * nHscrollInc, 0, NULL, NULL) ;
                    SetScrollPos (hWnd, SB_HORZ, nHscrollPos, TRUE) ;
                    }
               break ;

          case WM_PAINT:
               BeginPaint (hWnd, &ps) ;

               nPaintBeg = max (0, nVscrollPos + ps.rcPaint.top / yChar - 1) ;
               nPaintEnd = min (NUMLINES,
                              nVscrollPos + ps.rcPaint.bottom / yChar) ;

               for (i = nPaintBeg ; i < nPaintEnd ; i++)
                    TextOut (ps.hdc,
                              xChar * (1 - nHscrollPos),
                              yChar * (1 - nVscrollPos + i),
                              szBuffer, 
                         sprintf (szBuffer, "%-20s%-35s%5d",
                              sysmetrics[i].szLabel,
                              sysmetrics[i].szDesc,
                              GetSystemMetrics (sysmetrics[i].nIndex))) ;

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
