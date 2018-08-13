
/** 
	* @file 		plat_string.h 
	* @brief		系统库包装层 包装 time
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_STRING_H_
#define _PLATFORM_STRING_H_


#include "plat_stddef.h"

//各平台需要实现 strcmp 等字符串函数 



#if defined(LINUX_PLAT) 

#include <string.h>





#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"



#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#include "string.h"




#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  //_PLATFORM_STRING_H_

