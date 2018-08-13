
/** 
	* @file 		plat_stdbool.h 
	* @brief		系统库包装层 包装 stdbool
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_STDBOOL_H_
#define _PLATFORM_STDBOOL_H_




// 各个平台需要定义bool true false 
/*
#define bool    int
#define false   0
#define true    1
*/



#if defined(LINUX_PLAT) 

#include <stdbool.h>
#define plat_bool    bool
#define plat_false   false
#define plat_true    true


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

//#define bool    int
#include "cs_types.h"
#define plat_false   0
#define plat_true    1





#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  // _PLATFORM_STDBOOL_H_

