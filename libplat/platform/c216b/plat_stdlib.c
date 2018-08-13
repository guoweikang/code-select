/** 
	* @file 		plat_stdlib.c 
	* @brief		ϵͳ���װ�� ��װ time
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	
#include "cos.h"
#include "ctype.h"
#include "stdlib.h"
#include "plat_stdlib.h"

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

void*  plat_malloc(SIZE_T size)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000001);
	hal_HstSendEvent(0xffffffff);

	return COS_Malloc(size,COS_MMI_HEAP);
}

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
void  plat_free(void* ptr)
{	
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000002);
	hal_HstSendEvent(0xffffffff);

	return COS_Free(ptr);
}

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

char * plat_strdup(const char * str)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000003);
	hal_HstSendEvent(0xffffffff);
#if 0
	if (str == NULL)
	{
		hal_HstSendEvent(0xffffffff);
		hal_HstSendEvent(0x30000013);
		hal_HstSendEvent(0xffffffff);
		return NULL;
	}

	char *p = COS_Malloc((strlen(str) + 1),COS_MMI_HEAP);
	if (p == NULL)
	{
		hal_HstSendEvent(0xffffffff);
		hal_HstSendEvent(0x30000033);
		hal_HstSendEvent(0xffffffff);
		return p;
	}

	strcpy(p,str);
#endif
	if(NULL == str)
	{
		return NULL;
	}
	char *p_src = NULL;
	p_src = str ;
	int count = 0 ;
	while(*p_src != '\0')
	{
		 count++;
		 p_src++;
	
	}
	char * new_addr  = (char *)COS_Malloc(count+1,COS_MMI_HEAP);
	p_src = str;
	while(*p_src  != '\0')
	{
		*new_addr++ = *p_src++;
	
	}
	*new_addr = '\0';
	return (new_addr -(count));
}


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
int  plat_atoi(const char *ptr)
{
    int i = 0;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000004);
	hal_HstSendEvent(0xffffffff);

    while (isdigit(*ptr))
        i = i*10 + (*ptr++) - '0';

    return i;
}

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
void  plat_srand(unsigned int seed)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000005);
	hal_HstSendEvent(0xffffffff);

	srand(seed);

	return;
}

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
int  plat_rand(void)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000006);
	hal_HstSendEvent(0xffffffff);

	return rand();
}

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
int  plat_sleep(int seconds)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000007);
	hal_HstSendEvent(0xffffffff);

	return COS_Sleep(1000 * seconds);
}

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
void  plat_exit(int num)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x30000008);
	hal_HstSendEvent(0xffffffff);

	return;
}

