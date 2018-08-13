/** 
	* @file 		plat_pthread.c 
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
#include "plat_pthread.h"


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
int plat_pthread_create(PLAT_PTHREAD_T *thread_id, void *attr, void *(*func)(void *), void *arg)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x50000001);
	hal_HstSendEvent(0xffffffff);

	*thread_id = COS_CreateTask(func, NULL, NULL, PLAT_STACK_SIZE, PLAT_PRIORITY,  0, 0, "DM Task");
	if (*thread_id != NULL)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

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
int plat_pthread_join(PLAT_PTHREAD_T thread_id,void **retval)
{
	BOOL ret;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x50000002);
	hal_HstSendEvent(0xffffffff);

	COS_StopTask(thread_id);

	ret = COS_DeleteTask(thread_id);

	return ((int)ret);
}

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
int plat_pthread_mutex_init(PLAT_PTHREAD_MUTEX_T *mutex, void *mutexattr)
{
	*mutex = COS_CreateSemaphore(PLAT_SEMINITCOUNT);
	return 0;
}

int plat_pthread_mutex_destroy(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return COS_DeleteSemaphore(*mutex);
}

int plat_pthread_mutex_lock(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return COS_WaitForSemaphore(*mutex, PLAT_SEMTIMEOUT);
}

int plat_pthread_mutex_unlock(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return COS_ReleaseSemaphore(*mutex);
}

