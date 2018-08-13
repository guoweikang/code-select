
/** 
	* @file 		plat_math.h 
	* @brief		系统库包装层 包装 math
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_MATH_H_
#define _PLATFORM_MATH_H_




// 各个平台当前需要定义实现  
/*
函数 plat_pow
函数 plat_floor
函数 plat_fabs
*/



#if defined(LINUX_PLAT) 

#include <math.h>


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif
#include "plat_limits.h"

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




/*!\brief 平台 pow
	* 
	* C 库函数 double pow(double x, double y) 返回 x 的 y 次幂
	* @param[in]  x:代表基数的浮点值
	* @param[in]  y:代表指数的浮点值
	* @return ：  该函数返回 x 的 y 次幂的结果
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_pow(double      x,double y);


/*!\brief 平台 fabs
	* 
	* C 库函数 double fabs(double x) 返回 x 的绝对值。
	* @param[in]  x:浮点值。
	* @return 该函数返回 x 的绝对值。  
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_fabs(double      x);

/*!\brief 平台 floor
	* 
	* C 库函数 double floor(double x) 返回小于或等于 x 的最大的整数值
	* @param[in]  x:浮点值。
	* @return 该函数返回不大于 x 的最大整数值。  
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_floor(double      x);



#endif  // _PLATFORM_MATH_H_




