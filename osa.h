#ifndef OSA_H
#define OSA_H

#include <string.h>
#include <stdlib.h>

typedef signed char Sint8;
typedef signed short int Sint16;
typedef signed int Sint32;
typedef unsigned char Uint8;
typedef unsigned short int Uint16;
typedef unsigned int Uint32;

#define NULLPTR          ((void *)0)
#define CONST             const

typedef int Bool;
#define True 1
#define False 0

typedef Sint32                  Osa_status;
/*Osa_status返回值*/
#define OSA_SUCCESS             0/*成功*/
#define OSA_FAILURE             -1/*失败*/

Uint16 osa_get_u16(Uint8 *ptr);
Uint32 osa_get_u32(Uint8 *ptr);
void osa_memcpy(void* dest_ptr, CONST void* src_ptr, Uint32 copy_count);
void osa_memset(void* dest_ptr, Uint8 value, Uint32 count);
void* osa_malloc(Uint32 size);
void osa_free(void* ptr);

#endif
