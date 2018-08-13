
/** 
	* @file 		plat_pthread.c
	* @brief		ϵͳ���װ�� ��װ �߳�
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	





#if defined(LINUX_PLAT) 

#include "plat_pthread.h"
#include "pthread.h"





int plat_pthread_create(PLAT_PTHREAD_T *thread_id, void *attr, void *(*func)(void *), void *arg)
{
	return pthread_create(thread_id,attr,func,arg);

}




int plat_pthread_join(PLAT_PTHREAD_T thread_id,void **retval)
{
	return pthread_join(thread_id,retval);
}



int plat_pthread_mutex_init(PLAT_PTHREAD_MUTEX_T *mutex, void *mutexattr)
{
		return pthread_mutex_init(mutex,mutexattr);
}


int plat_pthread_mutex_destroy(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return pthread_mutex_destroy(mutex);
}

int plat_pthread_mutex_lock(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return pthread_mutex_lock(mutex);

}

int plat_pthread_mutex_unlock(PLAT_PTHREAD_MUTEX_T *mutex)
{
	return pthread_mutex_unlock(mutex);
}





#endif  // _PLATFORM_STDBOOL_H_



