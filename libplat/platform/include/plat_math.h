
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




#ifndef _PLATFORM_MATH_H_
#define _PLATFORM_MATH_H_




// ����ƽ̨��ǰ��Ҫ����ʵ��  
/*
���� plat_pow
���� plat_floor
���� plat_fabs
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




/*!\brief ƽ̨ pow
	* 
	* C �⺯�� double pow(double x, double y) ���� x �� y ����
	* @param[in]  x:��������ĸ���ֵ
	* @param[in]  y:����ָ���ĸ���ֵ
	* @return ��  �ú������� x �� y ���ݵĽ��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
double    plat_pow(double      x,double y);


/*!\brief ƽ̨ fabs
	* 
	* C �⺯�� double fabs(double x) ���� x �ľ���ֵ��
	* @param[in]  x:����ֵ��
	* @return �ú������� x �ľ���ֵ��  
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
double    plat_fabs(double      x);

/*!\brief ƽ̨ floor
	* 
	* C �⺯�� double floor(double x) ����С�ڻ���� x ����������ֵ
	* @param[in]  x:����ֵ��
	* @return �ú������ز����� x ���������ֵ��  
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
double    plat_floor(double      x);



#endif  // _PLATFORM_MATH_H_




