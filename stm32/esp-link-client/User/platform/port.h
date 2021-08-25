
#ifndef _PORT_H
#define _PORT_H

#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION()    ( portENTER_CRITICAL())      
#define EXIT_CRITICAL_SECTION()     ( portEXIT_CRITICAL())    

typedef unsigned char BOOL;
typedef unsigned char UCHAR;
typedef char CHAR;

typedef unsigned short USHORT;
typedef short SHORT;

typedef unsigned long ULONG;
typedef long LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#endif
