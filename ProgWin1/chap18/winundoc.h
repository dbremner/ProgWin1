/* Declarations for KERNEL File I/O and String Functions */

int   CALLBACK _lopen  (LPSTR, int) ;
int   CALLBACK _lclose (int) ;
int   CALLBACK _lcreat (LPSTR, int) ;
LONG  CALLBACK _llseek (int, long, int) ;
WORD  CALLBACK _lread  (int, LPSTR, int) ;
WORD  CALLBACK _lwrite (int, LPSTR, int) ;

#define READ       0
#define WRITE      1
#define READ_WRITE 2

int   CALLBACK lstrlen (LPSTR) ;
LPSTR CALLBACK lstrcpy (LPSTR, LPSTR) ;
LPSTR CALLBACK lstrcat (LPSTR, LPSTR) ;
int   CALLBACK lstrcmp (LPSTR, LPSTR) ;
