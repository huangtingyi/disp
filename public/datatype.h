#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__


typedef long		int8b;
typedef int		int4b;
typedef unsigned long	uint8b;
typedef unsigned int	uint4b;
typedef unsigned char	uint1b;

typedef long	LONGINT;
typedef long	POINTER;

#define	PRIdLI	"%ld"
#define	PRIdPT	"%ld"
#define ATOLI	atol

#include <time.h>

#ifndef FOUND
#define FOUND		1
#endif
#ifndef NOTFOUND
#define NOTFOUND	0
#endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef false
#define false		0
#endif

#ifndef true
#define true		1
#endif

#define MY_MAX(t1,t2)	(t1>t2?t1:t2)
#define MY_MIN(t1,t2)	(t1<t2?t1:t2)

#endif
