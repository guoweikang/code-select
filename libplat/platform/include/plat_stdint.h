
/** 
	* @file 		plat_stdint.h 
	* @brief		系统库包装层 包装 stdint 
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_STDINT_H_
#define _PLATFORM_STDINT_H_



 // 各个平台需要定义
 /*

int8_t;
int16_t;	
int32_t;	  
int64_t;
uint8_t;	
uint16_t;	  
uint32_t;		
uint64_t;	

Minimum of signed integral types.
INT8_MIN       
INT16_MIN     
INT32_MIN     
INT64_MIN     

Maximum of signed integral types
INT8_MAX      
INT16_MAX      
INT32_MAX     
INT64_MAX      
UINT64_MAX		
*/


#if defined(LINUX_PLAT) 

#include <stdint.h>


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

typedef char 					int8_t;
typedef short int				int16_t;	
typedef int 					int32_t;	  
typedef long long int 			int64_t;

typedef unsigned char			uint8_t;	
typedef unsigned short int		uint16_t;	  
typedef unsigned int			uint32_t;		
typedef unsigned long long int	uint64_t;	


#ifndef __INT64_C

#define __INT64_C(c)  (c ## LL)
#define __UINT64_C(c) (c ## ULL)

#endif 

/* Minimum of signed integral types.  */
#define INT8_MIN       (-128)
#define INT16_MIN      (-32767-1)
#define INT32_MIN      (-2147483647-1)
#define INT64_MIN      (-__INT64_C(9223372036854775807)-1)
 /* Maximum of signed integral types.  */                                                                                                   
#define INT8_MAX       (127)
#define INT16_MAX      (32767)
#define INT32_MAX      (2147483647)
#define INT64_MAX      (__INT64_C(9223372036854775807))
#define UINT64_MAX		(__UINT64_C(18446744073709551615))

#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

typedef signed char 			int8_t; 	
typedef short int				int16_t;	
typedef int 					int32_t;
typedef long long int 			int64_t;

typedef unsigned char			uint8_t;


typedef unsigned short int		uint16_t;	  
typedef unsigned int			uint32_t;		
typedef unsigned long long int	uint64_t;	 




#else 

#error NO PLATFORM DEFINE 




#endif   





#endif  // _PLATFORM_STDINT_H_

