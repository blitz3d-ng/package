
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define BASE_VER	200

#ifdef	PRO
#define	PRO_F	0x010000
#else
#define	PRO_F	0
#endif

#ifdef	DEMO
#define	DEMO_F	0x080000
#else
#define	DEMO_F	0
#endif

#ifdef	EDU
#define EDU_F	0x200000
#else
#define	EDU_F	0
#endif

#define VERSION		(BASE_VER|PRO_F|DEMO_F|EDU_F)

#if INTPTR_MAX == INT64_MAX
#define BB64
typedef long   bb_int_t;
typedef double bb_float_t;
#else
#define BB32
typedef int    bb_int_t;
typedef float  bb_float_t;
#endif
typedef bb_int_t bb_ptr_t;

#endif
