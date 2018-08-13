
/** 
	* @file 		plat_stdlib.h 
	* @brief		ϵͳ���װ�� ��װ stdlib
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_STDLIB_H_
#define _PLATFORM_STDLIB_H_




#include "plat_stddef.h"

//��ƽ̨��Ҫʵ��

/*
������ plat_malloc
������ plat_free
������ plat_strdup
������ plat_atoi
������ plat_srand
������ plat_rand
������ plat_srand
������ plat_exit



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




/*!\brief ƽ̨ �����ڴ溯��
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   size: �����ڴ��С
	* @return  ָ���ַ
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

void*  plat_malloc(SIZE_T size);





/*!\brief ƽ̨ �ͷ��ڴ溯��
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   ptr: ���ͷ�ָ���ַ
	* @return  ��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
void  plat_free(void* ptr);

/*!\brief ƽ̨ �����ڴ沢�������ݺ���
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   str: ������ָ���ַ
	* @return  �ɹ�:ָ���ַ ʧ��:NULL
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

char * plat_strdup(const char * str);

/*!\brief ƽ̨ �ַ���ת����
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   ptr: ��ת���ַ���
	* @return  ��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int  plat_atoi(const char *ptr);


/*!\brief ƽ̨ ����rand seed 
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   seed: ���������
	* @return  ��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
void  plat_srand(unsigned int seed);

/*!\brief ƽ̨ �������� 
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��

	* @return  �����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int  plat_rand(void);




/*!\brief ƽ̨˯�ߺ���
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��

	* @return  �����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int  plat_sleep(int seconds);


/*!\brief ƽ̨�����˳�����
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]:num �˳���
	* @return  �����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
void  plat_exit(int num);


#endif  //_PLATFORM_STDLIB_H_

