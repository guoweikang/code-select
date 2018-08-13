
/** 
	* @file 		plat_float.h 
	* @brief		系统库包装层 包装 float
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_FLOAT_H_
#define _PLATFORM_FLOAT_H_




// 当前各个平台需要定义实现 
/*
FLT_EPSILON
DBL_EPSILON
*/



#if defined(LINUX_PLAT) 

#include <float.h>

#define PLAT_FLT_EPSILON              FLT_EPSILON
#define PLAT_DBL_EPSILON              DBL_EPSILON


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif
#define PLAT_FLT_EPSILON                1.19209290E-07F
#define PLAT_DBL_EPSILON  2.2204460492503131e-016


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





#endif  // _PLATFORM_FLOAT_H_



