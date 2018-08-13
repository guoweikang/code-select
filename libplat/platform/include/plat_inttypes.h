
/** 
	* @file 		plat_inttypes.h 
	* @brief		ϵͳ���װ�� ��װ inttypes
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_INTTYPES_H_
#define _PLATFORM_INTTYPES_H_




// ����ƽ̨��ǰ��Ҫ����ʵ��  
/*
 __PRI64_PREFIX
 PRId64
*/



#if defined(LINUX_PLAT) 

#include <inttypes.h>


#define PLAT_PRId64     PRI64  


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

#define __PLAT_PRI64_PREFIX    "ll" 

#define PLAT_PRId64     __PLAT_PRI64_PREFIX "d"  






#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#ifdef __cplusplus
extern "C" {
#endif

#define __PLAT_PRI64_PREFIX    "ll" 

#define PLAT_PRId64     __PLAT_PRI64_PREFIX "d"  





#ifdef __cplusplus
}
#endif






#else 

#error NO PLATFORM DEFINE 


#endif   





#endif  // _PLATFORM_INTTYPES_H_


