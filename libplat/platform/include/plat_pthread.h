
/** 
	* @file 		plat_pthread.h 
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




#ifndef _PLATFORM_PTHREAD_H_
#define _PLATFORM_PTHREAD_H_




// ����ƽ̨��Ҫ����ʵ��
/*

PLAT_PTHREAD_T       �� �߳�ID ����
PLAT_PTHREAD_MUTEX_T �� ����������

������ plat_pthread_create 
������ plat_pthread_join 
������ plat_pthread_mutex_init
����:    plat_pthread_mutex_destroy
����:    plat_pthread_mutex_lock
����:    plat_pthread_mutex_unlock


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
#define PLAT_PTHREAD_T HANDLE // TODO �� �޸�Ϊ HANDLE
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
int plat_pthread_create(PLAT_PTHREAD_T *thread_id, void *attr, void *(*func)(void *), void *arg);


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
int plat_pthread_join(PLAT_PTHREAD_T thread_id,void **retval);


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
int plat_pthread_mutex_init(PLAT_PTHREAD_MUTEX_T *mutex, void *mutexattr);


int plat_pthread_mutex_destroy(PLAT_PTHREAD_MUTEX_T *mutex);

int plat_pthread_mutex_lock(PLAT_PTHREAD_MUTEX_T *mutex);

int plat_pthread_mutex_unlock(PLAT_PTHREAD_MUTEX_T *mutex);





#endif  // _PLATFORM_STDBOOL_H_


