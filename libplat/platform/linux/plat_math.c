
/** 
	* @file 		plat_floor.c 
	* @brief		ϵͳ���װ�� ��װ ����ͨ����ؽӿ�
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
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

