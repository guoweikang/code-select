
/** 
	* @file 		plat_stdbool.h 
	* @brief		ϵͳ���װ�� ��װ stdbool
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_STDBOOL_H_
#define _PLATFORM_STDBOOL_H_




// ����ƽ̨��Ҫ����bool true false 
/*
#define bool    int
#define false   0
#define true    1
*/



#if defined(LINUX_PLAT) 

#include <stdbool.h>
#define plat_bool    bool
#define plat_false   false
#define plat_true    true


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

//#define bool    int
#include "cs_types.h"
#define plat_false   0
#define plat_true    1





#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  // _PLATFORM_STDBOOL_H_

