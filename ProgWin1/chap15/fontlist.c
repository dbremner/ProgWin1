/* FONTLIST.C -- Font Enumeration Program */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "winundoc.h"
#include "fontlist.h"

typedef struct {
     GLOBALHANDLE hGMem ;
     short        nCount ;
     } ENUMER ;

typedef struct {
     short        nFontType ;
     LOGFONT      lf ;
     TEXTMETRIC   tm ;
     } FONT ;

long CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
int  CALLBACK EnumAllFaces (LPLOGFONT, LPTEXTMETRIC, short, ENUMER *) ;
int  CALLBACK EnumAllFonts (LPLOGFONT, LPTEXTMETRIC, short, ENUMER *) ;

char szAppName [] = "FontList" ;

int CALLBACK WinMain (hInstance, hPrevInstance, lpszCmdLine, nCmdShow)
     HANDLE   hInstance, hPrevInstance ;
     LPSTR    lpszCmdLine ;
     int      nCmdShow ;
     {
     HWND     hWnd ;
     MSG      msg ;
     WNDCLASS wndclass ;

     if (!hPrevInstance) 
          {
          wndclass.style         = CS_HREDRAW | CS_VREDRAW;
          wndclass.lpfnWndProc   = WndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = 0 ;
          wndclass.hInstance     = hInstance ;
          wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
          wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
          wndclass.lpszMenuName  = szAppName ;
          wndclass.lpszClassName = szAppName ;

          if (!RegisterClass (&wndclass))
               return FALSE ;
          }
     hWnd = CreateWindow (szAppName, "Font Enumeration",
                         WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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

int CALLBACK EnumAllFaces (lf, tm, nFontType, enumer)
     LPLOGFONT    lf ;
     LPTEXTMETRIC tm ;
     short        nFontType ;
     ENUMER   *enumer ;
     {
     LPSTR        lpFaces ;

     if (NULL == GlobalReAlloc (enumer->hGMem,
                         (DWORD) LF_FACESIZE * (1 + enumer->nCount),
                         GMEM_MOVEABLE))
          return 0 ;

     lpFaces = GlobalLock (enumer->hGMem) ;
     lstrcpy (lpFaces + enumer->nCount * LF_FACESIZE, lf->lfFaceName) ;
     GlobalUnlock (enumer->hGMem) ;
     enumer->nCount ++ ;
     return 1 ;
     }

int CALLBACK EnumAllFonts (lf, tm, nFontType, enumer)
     LPLOGFONT    lf ;
     LPTEXTMETRIC tm ;
     short        nFontType ;
     ENUMER   *enumer ;
     {
     FONT     *font ;     

     if (NULL == GlobalReAlloc (enumer->hGMem,
                         (DWORD) sizeof (FONT) * (1 + enumer->nCount),
                         GMEM_MOVEABLE))
          return 0 ;

     font = (FONT *) GlobalLock (enumer->hGMem) + enumer->nCount ;
     font->nFontType = nFontType ;
     font->lf = *lf ;
     font->tm = *tm ;

     GlobalUnlock (enumer->hGMem) ;
     enumer->nCount ++ ;
     return 1 ;
     }

void Display (hDC, xChar, yChar, font)
     HDC         hDC ;
     short       xChar, yChar ;
     FONT    *font ;
     {
     static FONT f ;

     static char *szYN [] = { "No",         "Yes" } ;
     static char *szCS [] = { "ANSI",       "?????",   "Kanji",    "OEM" } ;
     static char *szOP [] = { "Default",    "String",  "Char",    "Stroke" } ;
     static char *szCP [] = { "Default",    "Char",    "Stroke",   "?????" } ;
     static char *szQU [] = { "Draft",      "Default", "Proof",    "?????" } ;
     static char *szP1 [] = { "Default",    "Fixed",   "Variable", "?????" } ;
     static char *szP2 [] = { "Fixed",      "Variable" } ;
     static char *szFA [] = { "Don't Care", "Roman",      "Swiss", "Modern",
                              "Script",     "Decorative", "?????", "?????" } ;
     static char *szVR [] = { "Vector",     "Raster" } ;
     static char *szGD [] = { "GDI",        "Device" } ;

     static struct 
          {
          short x ;
          short y ;
          char  *szFmt ;
          short *pData ;
          }
          shorts [] = 
          {
           1,  1, "LOGFONT",            NULL, 
           1,  2, "-------",            NULL,
           1,  3, "Height:      %10d",  &f.lf.lfHeight,
           1,  4, "Width:       %10d",  &f.lf.lfWidth,
           1,  5, "Escapement:  %10d",  &f.lf.lfEscapement,
           1,  6, "Orientation: %10d",  &f.lf.lfOrientation,
           1,  7, "Weight:      %10d",  &f.lf.lfWeight,
          28,  1, "TEXTMETRIC",         NULL,
          28,  2, "----------",         NULL,
          28,  3, "Height:       %5d",  &f.tm.tmHeight,
          28,  4, "Ascent:       %5d",  &f.tm.tmAscent,
          28,  5, "Descent:      %5d",  &f.tm.tmDescent,
          28,  6, "Int. Leading: %5d",  &f.tm.tmInternalLeading,
          28,  7, "Ext. Leading: %5d",  &f.tm.tmExternalLeading,
          28,  8, "Ave. Width:   %5d",  &f.tm.tmAveCharWidth,
          28,  9, "Max. Width:   %5d",  &f.tm.tmMaxCharWidth,
          28, 10, "Weight:       %5d",  &f.tm.tmWeight,
          51, 10, "Overhang:     %10d", &f.tm.tmOverhang,
          51, 11, "Digitized X:  %10d", &f.tm.tmDigitizedAspectX,
          51, 12, "Digitized Y:  %10d", &f.tm.tmDigitizedAspectY
          } ;

     static struct 
          {
          short x ;
          short y ;
          char  *szFmt ;
          BYTE  *pData ;
          }
          bytes [] = 
          {
          51,  3, "First Char:   %10d", &f.tm.tmFirstChar,
          51,  4, "Last Char:    %10d", &f.tm.tmLastChar,
          51,  5, "Default Char: %10d", &f.tm.tmDefaultChar,
          51,  6, "Break Char:   %10d", &f.tm.tmBreakChar
          } ;

     static struct 
          {
          short x ;
          short y ;
          char  *szFmt ;
          BYTE  *pData ;
          char  **szArray ;
          short sAnd ;
          short sShift ;
          }
          strings [] = 
          {
           1,  8, "Italic:      %10s",  &f.lf.lfItalic,         szYN, 1,    0,
           1,  9, "Underline:   %10s",  &f.lf.lfUnderline,      szYN, 1,    0,
           1, 10, "Strike-Out:  %10s",  &f.lf.lfStrikeOut,      szYN, 1,    0,
           1, 11, "Char Set:    %10s",  &f.lf.lfCharSet,        szCS, 0xC0, 6,
           1, 12, "Out  Prec:   %10s",  &f.lf.lfOutPrecision,   szOP, 3,    0,
           1, 13, "Clip Prec:   %10s",  &f.lf.lfClipPrecision,  szCP, 3,    0, 
           1, 14, "Quality:     %10s",  &f.lf.lfQuality,        szQU, 3,    0,
           1, 15, "Pitch:       %10s",  &f.lf.lfPitchAndFamily, szP1, 3,    0,
           1, 16, "Family:      %10s",  &f.lf.lfPitchAndFamily, szFA, 0x70, 4,
          28, 11, "Italic:       %5s",  &f.tm.tmItalic,         szYN, 1,    0,
          28, 12, "Underline:    %5s",  &f.tm.tmUnderlined,     szYN, 1,    0,
          28, 13, "Strike-Out:   %5s",  &f.tm.tmStruckOut,      szYN, 1,    0,
          51,  7, "Pitch:        %10s", &f.tm.tmPitchAndFamily, szP2, 1,    0,
          51,  8, "Family:       %10s", &f.tm.tmPitchAndFamily, szFA, 0x70, 4,
          51,  9, "Char Set:     %10s", &f.tm.tmCharSet,        szCS, 0xC0, 6,
          36, 15, "Font Type:  %6s",    (BYTE *) &f.nFontType,  szVR, 1,    0,
          55, 15, "%s",                 (BYTE *) &f.nFontType,  szGD, 2,    1
          } ;
    
     char szBuffer [80] ;
     int  i ;

     f = *font ;

     for (i = 0 ; i < sizeof shorts / sizeof shorts [0] ; i++)
          TextOut (hDC, xChar * shorts[i].x, yChar * shorts[i].y, szBuffer,
                    sprintf (szBuffer, shorts[i].szFmt, *shorts[i].pData)) ;

     for (i = 0 ; i < sizeof bytes / sizeof bytes [0] ; i++)
          TextOut (hDC, xChar * bytes[i].x, yChar * bytes[i].y, szBuffer,
                    sprintf (szBuffer, bytes[i].szFmt, *bytes[i].pData)) ;

     for (i = 0 ; i < sizeof strings / sizeof strings [0] ; i++)
          TextOut (hDC, xChar * strings[i].x, yChar * strings[i].y, szBuffer,
                    sprintf (szBuffer, strings[i].szFmt, (strings[i].szArray)
                              [(*strings[i].pData & strings[i].sAnd) >>
                                             strings[i].sShift])) ;

     TextOut (hDC, xChar, yChar * 17, szBuffer,
               sprintf (szBuffer, "Face Name:   %10s", f.lf.lfFaceName)) ;  
     }

HDC GetPrinterIC ()
     {
     char szPrinter [64] ;
     char *szDevice, *szDriver, *szOutput ;

     GetProfileString ("windows", "device", "", szPrinter, 64) ;

     if ((szDevice = strtok (szPrinter, "," )) &&
         (szDriver = strtok (NULL,      ", ")) && 
         (szOutput = strtok (NULL,      ", ")))
          
               return CreateIC (szDriver, szDevice, szOutput, NULL) ;

     return NULL ;
     }

long CALLBACK WndProc (hWnd, iMessage, wParam, lParam)
     HWND           hWnd ;
     unsigned       iMessage ;
     WPARAM wParam ;
     LPARAM lParam ;
     {
     static BOOL    bHaveInfo = FALSE ;
     static ENUMER  enumer1, enumer2 ;
     static short   xChar, yChar, nCurrent ;
     static WORD    wCurrentDC = IDM_SCREEN ;
     HANDLE         hInstance ;
     HDC            hDC ;
     HFONT          hFont ;
     HMENU          hMenu ;
     FONT       *font ;
     LPSTR          lpFaces ;
     PAINTSTRUCT    ps ;
     short          i ;
     TEXTMETRIC     tm ;

     switch (iMessage)
          {
          case WM_CREATE:
               hInstance = ((LPCREATESTRUCT) lParam)-> hInstance ;
               hDC = GetDC (hWnd) ;
               GetTextMetrics (hDC, (LPTEXTMETRIC) &tm) ;
               xChar = tm.tmAveCharWidth ;
               yChar = tm.tmHeight + tm.tmExternalLeading ;
               ReleaseDC (hWnd, hDC) ;
               break ;
                              
          case WM_COMMAND:
               if (wParam == IDM_EXIT)
                    {
                    SendMessage (hWnd, WM_CLOSE, 0, 0L) ;
                    break ;
                    }
               else if (wParam == wCurrentDC)
                    break ;

               hMenu = GetMenu (hWnd) ;
               CheckMenuItem (hMenu, wCurrentDC, MF_UNCHECKED) ;
               CheckMenuItem (hMenu, wCurrentDC = wParam, MF_CHECKED) ;

                                        /* fall through */

          case WM_DEVMODECHANGE:
          case WM_FONTCHANGE:
               bHaveInfo = FALSE ;               
               InvalidateRect (hWnd, NULL, TRUE) ;
               break ;

          case WM_PAINT:
               if (!bHaveInfo)
                    {
                    if (enumer2.hGMem)
                         GlobalFree (enumer2.hGMem) ;

                    enumer1.hGMem  = GlobalAlloc (GHND, 1L) ;
                    enumer1.nCount = 0 ;

                    enumer2.hGMem  = GlobalAlloc (GHND, 1L) ;
                    enumer2.nCount = 0 ;

                    if (NULL == enumer1.hGMem || NULL == enumer2.hGMem)
                         goto MEMORY_ERROR ;

                    if (wCurrentDC == IDM_SCREEN)
                         hDC = CreateIC ("DISPLAY", NULL, NULL, NULL) ;
                    else
                         hDC = GetPrinterIC () ;

                    if (hDC)
                         {
                         if (0 == EnumFonts (hDC, NULL, EnumAllFaces,
                                                  (LPSTR) &enumer1))
                              goto MEMORY_ERROR ;

                         lpFaces = GlobalLock (enumer1.hGMem) ;

                         for (i = 0 ; i < enumer1.nCount ; i++)
                              if (0 == EnumFonts (hDC,
                                             lpFaces + i * LF_FACESIZE,
                                             EnumAllFonts,
                                             (LPSTR) &enumer2))
                                   goto MEMORY_ERROR ;

                         GlobalUnlock (enumer1.hGMem) ;
                         enumer2.nCount-- ;
     
                         DeleteDC (hDC) ;
                         bHaveInfo = TRUE ;
                         }
                    GlobalFree (enumer1.hGMem) ;
                    SetScrollRange (hWnd, SB_VERT, 0, enumer2.nCount, FALSE) ;
                    SetScrollPos   (hWnd, SB_VERT, nCurrent = 0, TRUE) ;
                    }

               hDC = BeginPaint (hWnd, &ps) ;

               if (bHaveInfo)
                    {
                    font = (FONT *) GlobalLock (enumer2.hGMem) + nCurrent ;
                    Display (hDC, xChar, yChar, font) ;
                    hFont = SelectObject (hDC, CreateFontIndirect (&font->lf));

                    TextOut (hDC, 1 * xChar, 19 * yChar,
                        "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz",
                         52) ;

                    GlobalUnlock (enumer2.hGMem) ;
                    DeleteObject (SelectObject (hDC, hFont)) ;
                    }

               EndPaint (hWnd, &ps) ;
               break ;

          case WM_KEYDOWN:
               switch (wParam)
                    {
                    case VK_HOME:
                         SendMessage (hWnd, WM_VSCROLL, SB_TOP, 0L) ;
                         break ;
                    case VK_END:
                         SendMessage (hWnd, WM_VSCROLL, SB_BOTTOM, 0L) ;
                         break ;
                    case VK_LEFT:
                    case VK_UP:
                    case VK_PRIOR:
                         SendMessage (hWnd, WM_VSCROLL, SB_LINEUP, 0L) ;
                         break ;
                    case VK_RIGHT:
                    case VK_DOWN:
                    case VK_NEXT:
                         SendMessage (hWnd, WM_VSCROLL, SB_LINEDOWN, 0L) ;
                         break ;
                    }
               break ;

          case WM_VSCROLL:
               switch (wParam)
                    {
                    case SB_TOP:
                         nCurrent = 0 ;
                         break ;
                    case SB_BOTTOM:
                         nCurrent = enumer2.nCount ;
                         break ;
                    case SB_LINEUP:
                    case SB_PAGEUP:
                         nCurrent -- ;
                         break ;
                    case SB_LINEDOWN:
                    case SB_PAGEDOWN:
                         nCurrent ++ ;
                         break ;
                    case SB_THUMBPOSITION:
                         nCurrent = LOWORD (lParam) ;
                         break ;
                    default:
                         return 0L ;
                    }
               nCurrent = min (max (0, nCurrent), enumer2.nCount) ;
               SetScrollPos (hWnd, SB_VERT, nCurrent, TRUE) ;
               InvalidateRect (hWnd, NULL, TRUE) ;
               break ;

          MEMORY_ERROR:
               MessageBox (hWnd, "Cannot allocate memory, must end.",
                    szAppName, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL) ;

                                             /* fall through */
          case WM_CLOSE:
               DestroyWindow (hWnd) ;
               break ;

          case WM_DESTROY:
               PostQuitMessage (0) ;
               break ;

          default:
               return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
          }
     return 0L ;
     }
