/* Declarations for KERNEL File I/O and String Functions */

int   FAR CALLBACK _lopen  (LPSTR, int) ;
int   FAR CALLBACK _lclose (int) ;
int   FAR CALLBACK _lcreat (LPSTR, int) ;
LONG  FAR CALLBACK _llseek (int, long, int) ;
WORD  FAR CALLBACK _lread  (int, LPSTR, int) ;
WORD  FAR CALLBACK _lwrite (int, LPSTR, int) ;

#define READ       0
#define WRITE      1
#define READ_WRITE 2

int   FAR CALLBACK lstrlen (LPSTR) ;
LPSTR FAR CALLBACK lstrcpy (LPSTR, LPSTR) ;
LPSTR FAR CALLBACK lstrcat (LPSTR, LPSTR) ;
int   FAR CALLBACK lstrcmp (LPSTR, LPSTR) ;
