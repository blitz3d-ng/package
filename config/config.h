
#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

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
typedef long   bb_int_t;
typedef double bb_float_t;
#else
typedef int    bb_int_t;
typedef float  bb_float_t;
#endif
typedef bb_int_t bb_ptr_t;

#endif
