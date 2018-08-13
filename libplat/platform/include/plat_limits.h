
/** 
	* @file 		plat_limits.h 
	* @brief		系统库包装层 包装 limits
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_LIMITS_H_
#define _PLATFORM_LIMITS_H_




// 各个平台当前需要定义实现  
/*
 INT_MAX
 INT_MIN 
 FLT_MAX	
 DBL_MAX 
*/



#if defined(LINUX_PLAT) 

#include <limits.h>

#define PLAT_INT_MAX INT_MAX
#define PLAT_INT_MIN (-INT_MAX-1)
#define PLAT_FLT_MAX	FLT_MAX
#define PLAT_DBL_MAX DBL_MAX



#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif


#define PLAT_INT_MAX 2147483647
#define PLAT_INT_MIN (-INT_MAX-1)

#define PLAT_FLT_MAX	3.40282347e+38F

#define PLAT_DBL_MAX 1.7976931348623158e+308 /* max value */



#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#ifdef __cplusplus
extern "C" {
#endif






#ifdef __cplusplus
}
#endif



#else 

#error NO PLATFORM DEFINE 


#endif   





#endif  // _PLATFORM_LIMITS_H_



