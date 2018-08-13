
/** 
	* @file 		plat_assert.h 
	* @brief		ϵͳ���װ�� assert ʵ��
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_ASSERT_H_
#define _PLATFORM_ASSERT_H_




// ����ƽ̨��Ҫ����  
/*
assertʵ�ֺ���
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




