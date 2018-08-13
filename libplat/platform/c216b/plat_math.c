
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
#include "plat_math.h"

/*!\brief 平台 pow
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  x:
	* @param[in]  y:
	* @return   
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_pow(double      x,double y)
{
	double ret = 1;
	double i;

	if ((y <= 0) || (plat_floor(y) != y) || (x <= 1))
	{
		return 0;
	}

	for (i = 0; i < y; i++)
	{
		if (ret >= (DBL_MAX / x))
		{
			return 0;
		}
		ret = ret * x;
	}

	return ret;
}


/*!\brief 平台 fabs
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  x:
	* @return   
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_fabs(double      x)
{
	double ret;

	if (x < 0)
	{
		ret = (-1) * x;
	}
	else
	{
		ret = x;
	}

	return ret;
}

/*!\brief 平台 floor
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  x:
	* @return   
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
double    plat_floor(double      x)
{
	int y = 0;
	double ret;

	if (x < 0)
	{
		y = (int)x - 1;
	}
	else
	{
		y = (int)x;
	}

	ret = (double)y;

	return ret;
}




