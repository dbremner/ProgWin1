/* POPPADP.C -- Popup Notepad Printing */

#include <windows.h>

extern char szAppName [] ;              /* in POPPAD.C */

BOOL CALLBACK PrintDlgProc (hDlg, iMessage, wParam, lParam)
     HWND     hDlg ;
     UINT iMessage ;
     WPARAM  wParam ;
     LPARAM  lParam ;
     {
     return FALSE ;
     }

BOOL CALLBACK AbortProc (hPrinterDC, nCode)
     HDC   hPrinterDC ;
     short nCode ;
     {
     return FALSE ;
     }

BOOL PrintFile (hInstance, hWnd, hWndEdit, szFileName)
     HANDLE    hInstance, hWnd, hWndEdit ;
     char      *szFileName ;
     {
     MessageBox (hWnd, "Printing not yet implemented", szAppName,
                         MB_OK | MB_ICONEXCLAMATION) ;
     return FALSE ;
     }
