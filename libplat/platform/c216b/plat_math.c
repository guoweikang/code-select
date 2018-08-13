
/** 
	* @file 		plat_math.h 
	* @brief		ϵͳ���װ�� ��װ math
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	
#include "plat_math.h"

/*!\brief ƽ̨ pow
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  x:
	* @param[in]  y:
	* @return   
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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


/*!\brief ƽ̨ fabs
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  x:
	* @return   
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

/*!\brief ƽ̨ floor
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  x:
	* @return   
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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




