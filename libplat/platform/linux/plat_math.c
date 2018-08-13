
/** 
	* @file 		plat_floor.c 
	* @brief		系统库包装层 包装 网络通信相关接口
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	

#if defined(LINUX_PLAT) 
#include "math.h"



double    plat_pow(double      x,double y)
{
	return pow(x,y);
}



double    plat_fabs(double      x)
{
	return fabs(x);
}

double    plat_floor(double      x)
{
	return floor(x);
}


#endif 

