/* FILEDLG.C -- Open and Close File Dialog Boxes */

#include <windows.h>
#include <string.h>
#include "filedlg.h"

BOOL CALLBACK FileOpenDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK FileSaveDlgProc (HWND, UINT, WPARAM, LPARAM) ;

static char      szDefExt   [5]  ;
static char      szFileName [96] ;
static char      szFileSpec [16] ;
static POFSTRUCT pof ;
static WORD      wFileAttr, wStatus ;

int DoFileOpenDlg (hInst, hWnd, szFileSpecIn, szDefExtIn, wFileAttrIn,
                                szFileNameOut, pofIn)
     HANDLE    hInst ;
     HWND      hWnd ;
     char      *szFileSpecIn, *szDefExtIn, *szFileNameOut ;
     WORD      wFileAttrIn ;
     POFSTRUCT pofIn ;
     {
     int       iReturn ;

     strcpy (szFileSpec, szFileSpecIn) ;
     strcpy (szDefExt,   szDefExtIn) ;
     wFileAttr = wFileAttrIn ;
     pof = pofIn ;

     iReturn = DialogBox (hInst, "FileOpen", hWnd, FileOpenDlgProc) ;

     strcpy (szFileNameOut, szFileName) ;
     return iReturn ;
     }

int DoFileSaveDlg (hInst, hWnd, szFileSpecIn, szDefExtIn, pwStatusOut,
                                szFileNameOut, pofIn)
     HANDLE    hInst ;
     HWND      hWnd ;
     char      *szFileSpecIn, *szDefExtIn, *szFileNameOut ;
     WORD      *pwStatusOut ;
     POFSTRUCT pofIn ;
     {
     int       iReturn ;

     strcpy (szFileSpec, szFileSpecIn) ;
     strcpy (szDefExt,   szDefExtIn) ;
     pof = pofIn ;

     iReturn = DialogBox (hInst, "FileSave", hWnd, FileSaveDlgProc) ;

     strcpy (szFileNameOut, szFileName) ;
     *pwStatusOut = wStatus ;
     return iReturn ;
     }

BOOL CALLBACK FileOpenDlgProc (hDlg, iMessage, wParam, lParam)
     HWND      hDlg ;
     unsigned  iMessage ;
     WPARAM wParam ;
     LPARAM lParam ;
     {
     char      cLastChar ;
     short     nEditLen ;

     switch (iMessage)
        {
        case WM_INITDIALOG:
           SendDlgItemMessage (hDlg, IDD_FNAME, EM_LIMITTEXT, 80, 0L) ;
           DlgDirList (hDlg, szFileSpec, IDD_FLIST, IDD_FPATH, wFileAttr) ;
           SetDlgItemText (hDlg, IDD_FNAME, szFileSpec) ;
           return TRUE ;

        case WM_COMMAND:

           switch (wParam)
              {
              case IDD_FLIST:

                 switch (HIWORD (lParam))
                    {
                    case LBN_SELCHANGE:
                       if (DlgDirSelect (hDlg, szFileName, IDD_FLIST))
                          strcat (szFileName, szFileSpec) ;
                       SetDlgItemText (hDlg, IDD_FNAME, szFileName) ;
                       break ;

                    case LBN_DBLCLK:
                       if (DlgDirSelect (hDlg, szFileName, IDD_FLIST))
                          {
                          strcat (szFileName, szFileSpec) ;
                          DlgDirList (hDlg, szFileName, IDD_FLIST, IDD_FPATH,
                                                              wFileAttr) ;
                          SetDlgItemText (hDlg, IDD_FNAME, szFileSpec) ;
                          }
                       else
                          {
                          SetDlgItemText (hDlg, IDD_FNAME, szFileName) ;
                          SendMessage (hDlg, WM_COMMAND, IDOK, 0L) ;
                          }
                       break ;
                    }
                 break ;

              case IDD_FNAME:
                 if (HIWORD (lParam) == EN_CHANGE)
                    EnableWindow (GetDlgItem (hDlg, IDOK),
                       (BOOL) SendMessage (LOWORD (lParam),
                                             WM_GETTEXTLENGTH, 0, 0L)) ;
                 break ;

              case IDOK:
                 GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80) ;

                 nEditLen  = strlen (szFileName) ;
                 cLastChar = *AnsiPrev (szFileName, szFileName + nEditLen) ;
               
                 if (cLastChar == '\\' || cLastChar == ':')
                    strcat (szFileName, szFileSpec) ;

                 if (strchr (szFileName, '*') || strchr (szFileName, '?'))
                    {
                    if (DlgDirList (hDlg, szFileName, IDD_FLIST,
                                                  IDD_FPATH, wFileAttr))
                       {
                       strcpy (szFileSpec, szFileName) ;
                       SetDlgItemText (hDlg, IDD_FNAME, szFileSpec) ;
                       }
                    else
                       MessageBeep (0) ;               
                    break ;
                    }

                 strcat (strcat (szFileName, "\\"), szFileSpec) ;

                 if (DlgDirList (hDlg, szFileName, IDD_FLIST,
                                                  IDD_FPATH, wFileAttr))
                    {
                    strcpy (szFileSpec, szFileName) ;
                    SetDlgItemText (hDlg, IDD_FNAME, szFileSpec) ;
                    break ;
                    }

                 szFileName [nEditLen] = '\0' ;

                 if (-1 == OpenFile (szFileName, pof, OF_READ | OF_EXIST))
                    {
                    strcat (szFileName, szDefExt) ;
                    if (-1 == OpenFile (szFileName, pof, OF_READ | OF_EXIST))
                       {
                       MessageBeep (0) ;
                       break ;
                       }
                    }
                    strcpy (szFileName,
                         (PSTR) AnsiNext (strrchr (pof->szPathName, '\\'))) ;
                 
                 OemToAnsi (szFileName, szFileName) ;
                 EndDialog (hDlg, TRUE) ;
                 break ;

              case IDCANCEL:
                 EndDialog (hDlg, FALSE) ;
                 break ;

              default:
                 return FALSE ;
              }
        default:
           return FALSE ;            
        }
     return TRUE ;
     }

BOOL CALLBACK FileSaveDlgProc (hDlg, iMessage, wParam, lParam)
     HWND      hDlg ;
     unsigned  iMessage ;
     WPARAM wParam ;
     LPARAM lParam ;
     {
     switch (iMessage)
        {
        case WM_INITDIALOG:
           SendDlgItemMessage (hDlg, IDD_FNAME, EM_LIMITTEXT, 80, 0L) ;
           DlgDirList (hDlg, szFileSpec, 0, IDD_FPATH, 0) ;
           SetDlgItemText (hDlg, IDD_FNAME, szFileSpec) ;
           return TRUE ;

        case WM_COMMAND:

           switch (wParam)
              {
              case IDD_FNAME:
                 if (HIWORD (lParam) == EN_CHANGE)
                    EnableWindow (GetDlgItem (hDlg, IDOK),
                       (BOOL) SendMessage (LOWORD (lParam),
                                             WM_GETTEXTLENGTH, 0, 0L)) ;
                 break ;

              case IDOK:
                 GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80) ;

                 if (-1 == OpenFile (szFileName, pof, OF_PARSE))
                    {
                    MessageBeep (0) ;
                    break ;
                    }

                 if (!strchr ((PSTR) AnsiNext
                                   (strrchr (pof->szPathName, '\\')), '.'))
                    strcat (szFileName, szDefExt) ;

                 if (-1 != OpenFile (szFileName, pof, OF_WRITE | OF_EXIST))
                    wStatus = 1 ;

                 else if (-1 != OpenFile (szFileName, pof,
                                                  OF_CREATE | OF_EXIST))
                    wStatus = 0 ;

                 else
                    {
                    MessageBeep (0) ;
                    break ;
                    }

                    strcpy (szFileName, 
                         (PSTR) AnsiNext (strrchr (pof->szPathName, '\\'))) ;

                 OemToAnsi (szFileName, szFileName) ;
                 EndDialog (hDlg, TRUE) ;
                 break ;

              case IDCANCEL:
                 EndDialog (hDlg, FALSE) ;
                 break ;

              default:
                 return FALSE ;
              }
        default:
           return FALSE ;            
        }
     return TRUE ;
     }

