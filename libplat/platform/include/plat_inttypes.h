
/** 
	* @file 		plat_inttypes.h 
	* @brief		系统库包装层 包装 inttypes
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_INTTYPES_H_
#define _PLATFORM_INTTYPES_H_




// 各个平台当前需要定义实现  
/*
 __PRI64_PREFIX
 PRId64
*/



#if defined(LINUX_PLAT) 

#include <inttypes.h>


#define PLAT_PRId64     PRI64  


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

#define __PLAT_PRI64_PREFIX    "ll" 

#define PLAT_PRId64     __PLAT_PRI64_PREFIX "d"  






#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#ifdef __cplusplus
extern "C" {
#endif

#define __PLAT_PRI64_PREFIX    "ll" 

#define PLAT_PRId64     __PLAT_PRI64_PREFIX "d"  





#ifdef __cplusplus
}
#endif






#else 

#error NO PLATFORM DEFINE 


#endif   





#endif  // _PLATFORM_INTTYPES_H_


