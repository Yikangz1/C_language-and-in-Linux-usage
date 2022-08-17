#include "osa.h"

/*****************************************************************************
* Function  : osa_get_u16
* Purpose   :从内存里获取一个2字节数据(以大端序存放)。OSA将其转换成主机序返回。
* Relation  : 
* 
* Input Parameters:
*                                                                              
*       Name                Type                In/Out      Description 
*   -----------         --------------          ------      ----------- 
* 
* Return: 
* Note:     
*******************************************************************************/
Uint16 osa_get_u16(Uint8 *ptr)// 该函数传进来一个指针（地址）*ptr
{
    Uint16  temp; // 初始化一个uint16类型的temp变量
    temp = (Uint16)(*ptr);// 将*ptr指向的地址中的数据赋给temp
    temp = ((temp<<8) & 0xFF00) | ((Uint16)(*(ptr+1)));//
    return temp;
}


/*****************************************************************************
* Function  : osa_get_u32
* Purpose   :从内存里获取一个4字节数据(以大端序存放)。OSA将其转换成主机序返回。
* Relation  : 
* 
* Input Parameters:
*                                                                              
*       Name                Type                In/Out      Description 
*   -----------         --------------          ------      ----------- 
* 
* Return: 
* Note:     
*******************************************************************************/
Uint32 osa_get_u32(Uint8 *ptr)
{
    Uint32  temp;
    temp = (((Uint32)*ptr) << 24) & 0xFF000000;
    temp |= (((Uint32)*(ptr+1)) << 16) & 0x00FF0000;
    temp |= (((Uint32)*(ptr+2)) << 8) & 0x0000FF00;
    temp |= ((Uint32)*(ptr+3)) & 0x000000FF;
    
    return temp;
}

void osa_memcpy(void* dest_ptr, CONST void* src_ptr, Uint32 copy_count)
{
    if (dest_ptr == src_ptr)
    {
        return;
    }
    
    memcpy(dest_ptr, src_ptr, copy_count);
}

void osa_memset(void* dest_ptr, Uint8 value, Uint32 count)
{
    memset(dest_ptr, (int)value, (size_t)count);
}

void* osa_malloc(Uint32 size)
{
    return (Uint8*)malloc(size);
}

void osa_free(void* ptr)
{
    free(ptr);
}
