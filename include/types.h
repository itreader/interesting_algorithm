/*
 * types.h
 *
 *  Created on: 2019年1月6日
 *      Author: huangbin
 */

#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

typedef unsigned int          T_BOOL;
typedef unsigned char         T_UINT8;
typedef signed char           T_SINT8;
typedef unsigned short        T_UINT16;
typedef signed short          T_SINT16;
typedef unsigned int          T_UINT32;
typedef signed int            T_SINT32;
typedef unsigned long long    T_UINT64;
typedef signed long long      T_SINT64;
typedef float                 T_SINGLEFLOAT;
typedef double                T_DOUBLEFLOAT;
typedef long double           T_QUADFLOAT;
typedef unsigned char         T_CHAR;
typedef unsigned short        T_WCHAR;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* TYPES_H_ */
