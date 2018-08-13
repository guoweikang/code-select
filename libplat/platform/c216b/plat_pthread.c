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
#include "plat_pthread.h"


/*!\brief ƽ̨ͳһ��׼�ӿڣ������߳�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[out]   thread_id 	
	* @param[in]    attr:����
	* @param[in]    func:�̺߳���
	* @param[in]    arg:�̺߳�������
	* @return  �ɹ�����0  ʧ�ܷ���-1
	* @par format   
	*	   ���� 
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

/*!\brief ƽ̨ͳһ��׼�ӿڣ��ȴ��߳��˳�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��	
	* @param[in]    thread_id:�߳�ID 
	* @param[out]    retval:�߳�״̬����
	* @return  �ɹ�����0  ʧ�ܷ���-1
	* @par format   
	*	   ���� 
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

/*!\brief ƽ̨ͳһ��׼�ӿڣ��ź�����ʼ��
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   mutex  	
	* @return  �������
	* @par format   
	*	   ���� 
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

