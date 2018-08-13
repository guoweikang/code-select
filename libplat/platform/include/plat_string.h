
/** 
	* @file 		plat_string.h 
	* @brief		ϵͳ���װ�� ��װ time
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_STRING_H_
#define _PLATFORM_STRING_H_


#include "plat_stddef.h"

//��ƽ̨��Ҫʵ�� strcmp ���ַ������� 



#if defined(LINUX_PLAT) 

#include <string.h>





#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"



#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#include "string.h"




#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  //_PLATFORM_STRING_H_

