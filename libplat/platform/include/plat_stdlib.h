
/** 
	* @file 		plat_stdlib.h 
	* @brief		系统库包装层 包装 stdlib
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_STDLIB_H_
#define _PLATFORM_STDLIB_H_




#include "plat_stddef.h"

//各平台需要实现

/*
函数： plat_malloc
函数： plat_free
函数： plat_strdup
函数： plat_atoi
函数： plat_srand
函数： plat_rand
函数： plat_srand
函数： plat_exit



*/

#if defined(LINUX_PLAT) 

#include <stdlib.h>




#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif





#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   




/*!\brief 平台 申请内存函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   size: 申请内存大小
	* @return  指针地址
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

void*  plat_malloc(SIZE_T size);





/*!\brief 平台 释放内存函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   ptr: 待释放指针地址
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
void  plat_free(void* ptr);

/*!\brief 平台 申请内存并复制数据函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   str: 待复制指针地址
	* @return  成功:指针地址 失败:NULL
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

char * plat_strdup(const char * str);

/*!\brief 平台 字符串转整型
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   ptr: 带转换字符串
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int  plat_atoi(const char *ptr);


/*!\brief 平台 设置rand seed 
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]   seed: 随机数种子
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
void  plat_srand(unsigned int seed);

/*!\brief 平台 获得随机数 
	* 
	* 平台统一标准接口，各个平台内部各自实现

	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int  plat_rand(void);




/*!\brief 平台睡眠函数
	* 
	* 平台统一标准接口，各个平台内部各自实现

	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int  plat_sleep(int seconds);


/*!\brief 平台程序退出函数
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]:num 退出码
	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
void  plat_exit(int num);


#endif  //_PLATFORM_STDLIB_H_

