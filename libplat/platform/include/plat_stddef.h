
/** 
	* @file 		plat_stddef.h 
	* @brief		系统库包装层 包装 stdint 
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	



 // 各个平台需要定义
 /*
 SIZE_T 一般为 unsigned64 或 32 
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

