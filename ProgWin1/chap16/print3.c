/* PRINT3.C -- Printing with Dialog Box */

#include <windows.h>

HDC  GetPrinterDC (void) ;              /* in PRINT.C */
void PageGDICalls (HDC, short, short) ;

HANDLE hInst ;
char   szAppName [] = "Print3" ;
char   szCaption [] = "Print Program 3 (Dialog Box)" ;

BOOL   bUserAbort ;
HWND   hDlgPrint ;

BOOL CALLBACK PrintDlgProc (hDlg, iMessage, wParam, lParam)
     HWND     hDlg ;
     UINT iMessage ;
     WPARAM  wParam ;
     DWORD  lParam ;
     {
     switch (iMessage)
          {
          case WM_INITDIALOG:
               SetWindowText (hDlg, szAppName) ;
               EnableMenuItem (GetSystemMenu (hDlg, FALSE), SC_CLOSE,
                                                            MF_GRAYED) ;
               break ;

          case WM_COMMAND:
               bUserAbort = TRUE ;
               EnableWindow (GetParent (hDlg), TRUE) ;
               DestroyWindow (hDlg) ;
               hDlgPrint = 0 ;
               break ;

          default:
               return FALSE ;
          }
     return TRUE ;
     }          

BOOL CALLBACK AbortProc (hPrnDC, nCode)
     HDC   hPrnDC ;
     short nCode ;
     {
     MSG   msg ;

     while (!bUserAbort && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
          {
          if (!hDlgPrint || !IsDialogMessage (hDlgPrint, &msg))
               {
               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
               }
          }
     return !bUserAbort ;
     }

BOOL PrintMyPage (hWnd)
     HWND        hWnd ;
     {
     static char szMessage [] = "Print3: Printing" ; 
     BOOL        bError = FALSE ;
     HDC         hPrnDC ;
     RECT        rect ;
     short       xPage, yPage ;

     if (NULL == (hPrnDC = GetPrinterDC ()))
          return TRUE ;

     xPage = GetDeviceCaps (hPrnDC, HORZRES) ;
     yPage = GetDeviceCaps (hPrnDC, VERTRES) ;

     EnableWindow (hWnd, FALSE) ;

     bUserAbort = FALSE ;
     hDlgPrint = CreateDialog (hInst, "PrintDlgBox", hWnd, PrintDlgProc) ;

     Escape (hPrnDC, SETABORTPROC, 0, (LPSTR) AbortProc, NULL) ;

     if (Escape (hPrnDC, STARTDOC, sizeof szMessage - 1, szMessage, NULL) > 0)
          {
          PageGDICalls (hPrnDC, xPage, yPage) ;

          if (Escape (hPrnDC, NEWFRAME, 0, NULL, NULL) > 0)
               Escape (hPrnDC, ENDDOC, 0, NULL, NULL) ;
          else
               bError = TRUE ;
          }
     else
          bError = TRUE ;
                                        
     if (!bUserAbort)
          {
          EnableWindow (hWnd, TRUE) ;     
          DestroyWindow (hDlgPrint) ;
          }

     FreeProcInstance (lpfnPrintDlgProc) ;
     FreeProcInstance (lpfnAbortProc) ;
     DeleteDC (hPrnDC) ;

     return bError || bUserAbort ;
     }
