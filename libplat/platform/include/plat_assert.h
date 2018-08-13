
/** 
	* @file 		plat_assert.h 
	* @brief		系统库包装层 assert 实现
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_ASSERT_H_
#define _PLATFORM_ASSERT_H_




// 各个平台需要定义  
/*
assert实现函数
*/



#if defined(LINUX_PLAT) 

#include "assert.h"

#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif


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






#endif  // _PLATFORM_ASSERT_H_




