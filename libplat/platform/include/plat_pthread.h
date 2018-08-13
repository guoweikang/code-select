
/** 
	* @file 		plat_pthread.h 
	* @brief		系统库包装层 包装 线程
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_PTHREAD_H_
#define _PLATFORM_PTHREAD_H_




// 各个平台需要定义实现
/*

PLAT_PTHREAD_T       ： 线程ID 类型
PLAT_PTHREAD_MUTEX_T ： 互斥锁类型

函数： plat_pthread_create 
函数： plat_pthread_join 
函数： plat_pthread_mutex_init
函数:    plat_pthread_mutex_destroy
函数:    plat_pthread_mutex_lock
函数:    plat_pthread_mutex_unlock


*/


#if defined(LINUX_PLAT) 

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


#define PLAT_PTHREAD_T pthread_t
#define PLAT_PTHREAD_MUTEX_T pthread_mutex_t




#ifdef __cplusplus
}
#endif


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

#include "cs_types.h"
#include "cos.h"
#define PLAT_PTHREAD_T HANDLE // TODO ： 修改为 HANDLE
#define PLAT_PTHREAD_MUTEX_T HANDLE


#define PLAT_STACK_SIZE 4096

#define PLAT_PRIORITY (COS_MMI_TASKS_PRIORITY_BASE + COS_PRIORITY_NUM - 4)

#define PLAT_SEMINITCOUNT 1

#define PLAT_SEMTIMEOUT 10

#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)

#ifdef __cplusplus
extern "C" {
#endif
#define PLAT_PTHREAD_T HANDLE
#define PLAT_PTHREAD_MUTEX_T HANDLE





#ifdef __cplusplus
}
#endif




#else 

#error NO PLATFORM DEFINE 




#endif   




/*!\brief 平台统一标准接口，创建线程
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[out]   thread_id 	
	* @param[in]    attr:参数
	* @param[in]    func:线程函数
	* @param[in]    arg:线程函数参数
	* @return  成功返回0  失败返回-1
	* @par format   
	*	   保留 
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see
*/
int plat_pthread_create(PLAT_PTHREAD_T *thread_id, void *attr, void *(*func)(void *), void *arg);


/*!\brief 平台统一标准接口，等待线程退出
	* 
	* 平台统一标准输出接口，各个平台内部各自实现	
	* @param[in]    thread_id:线程ID 
	* @param[out]    retval:线程状态返回
	* @return  成功返回0  失败返回-1
	* @par format   
	*	   保留 
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see
*/
int plat_pthread_join(PLAT_PTHREAD_T thread_id,void **retval);


/*!\brief 平台统一标准接口，信号量初始化
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   mutex  	
	* @return  输出长度
	* @par format   
	*	   保留 
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see
*/
int plat_pthread_mutex_init(PLAT_PTHREAD_MUTEX_T *mutex, void *mutexattr);


int plat_pthread_mutex_destroy(PLAT_PTHREAD_MUTEX_T *mutex);

int plat_pthread_mutex_lock(PLAT_PTHREAD_MUTEX_T *mutex);

int plat_pthread_mutex_unlock(PLAT_PTHREAD_MUTEX_T *mutex);





#endif  // _PLATFORM_STDBOOL_H_


