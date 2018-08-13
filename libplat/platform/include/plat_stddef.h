
/** 
	* @file 		plat_stddef.h 
	* @brief		ϵͳ���װ�� ��װ stdint 
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	



 // ����ƽ̨��Ҫ����
 /*
 SIZE_T һ��Ϊ unsigned64 �� 32 
 NULL
*/


#ifndef _PLATFORM_STDDEF_H_
#define _PLATFORM_STDDEF_H_





#if defined(LINUX_PLAT) 

#include <stddef.h>

#define SIZE_T size_t


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned int SIZE_T;
//#undef NULL 
//#if defined(__cplusplus)
//#define NULL 0
//#else
//#define NULL ((void *)0)
//#endif
#include "cs_types.h"


#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  // _PLATFORM_STDDEF_H_

