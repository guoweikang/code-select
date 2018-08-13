
/** 
	* @file 		plat_stdio.h 
	* @brief		ϵͳ���װ�� ��װ ��׼io
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_STDIO_H_
#define _PLATFORM_STDIO_H_





#include "plat_stddef.h"
//��ƽ̨��Ҫʵ�� 
/*
PLAT_FILE 
PLAT_STDERR
PLAT_STDIN
PLAT_STDOUT


������  plat_printf
������  plat_fprintf
������  plat_sprintf
������  plat_snprintf 
������  plat_sscanf
������  plat_isprint
������  plat_isspace 
������  plat_isalnum 
������  plat_toupper 
������  plat_tolower 
������  plat_fopen 
������  plat_fclose 
������  plat_feof 
������  plat_fread 

*/


#if defined(LINUX_PLAT) 

#include <stdio.h>





#define PLAT_FILE   FILE 
#define PLAT_STDERR stderr
#define PLAT_STDOUT stdout


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif

typedef   unsigned int  PLAT_FILE;

#define PLAT_STDIN  (PLAT_FILE *)0
#define PLAT_STDOUT (PLAT_FILE *)1
#define PLAT_STDERR (PLAT_FILE *)2 


#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   





    

/*!\brief ƽ̨ͳһ��׼����ӿڣ�����ƽ̨����ʵ��
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   format ������ݸ�ʽ	
	* @return  �������
	* @par format   
	*	   ���� 
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see
*/

int plat_printf(const char* format,...);


/*!\brief ƽ̨ͳһ��׼����ӿ�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ�֣�����ͨ��PLAT_FILE ָ���������
	* @param[in]   stream: ���������
	* @param[in]   format ������ݸ�ʽ	
	* @return  �������
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_fprintf(PLAT_FILE *stream,const char* format,...);




/*!\brief ƽ̨ͳһ��׼����ӿ�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ�֣�����ͨ��PLAT_FILE ָ���������
	* @param[in]   str: ���������
	* @param[in]   format ������ݸ�ʽ	
	* @return  �������
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_sprintf(char *str,const char* format,...);



/*!\brief ƽ̨ͳһ��׼����ӿ�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ�֣�����ͨ��PLAT_FILE ָ���������
	* @param[in]   str: ���������
	* @param[in]   size: ����������
	* @param[in]   format ������ݸ�ʽ	
	* @return  �������
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_snprintf(char *str,SIZE_T size,const char* format,...);



/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿڣ�ɨ���ַ���
	* @param[in]   str: �������ַ���
	* @param[in]   format ��Ҫ��ȡ��ʽ
	* @param[out]  ...���ɱ���� ���ɨ�����
	* @return  ��ȡ�������� 
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_sscanf(const char  *str,const char* format,...);


/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� �ж��ַ��Ƿ�ɴ�ӡ��ʾ
	* @param[in]   c:  ���ж��ַ�
	* @return   �ɴ�ӡ��1�� ���ɴ�ӡ��0��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_isprint(int c );

/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� �ж��ַ��Ƿ��� space
	* @param[in]   c:  ���ж��ַ�
	* @return   �� (1) ���ǣ�0��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_isspace(int c );

/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� �ж�����
	* @param[in]   c:  ���ж��ַ�
	* @return   �� (1) ���ǣ�0��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_isalnum(int c );



/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� ת���ַ�Ϊ��д 
	* @param[in]   c:  ��ת�ַ�
	* @return   ��д�ַ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_toupper(int c );

/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� ת���ַ�ΪСд 
	* @param[in]   c:  ��ת�ַ�
	* @return   ��д�ַ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_tolower(int c );


/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� ���ļ�
	* @param[in]   path:  ·��
	* @param[in]   mode:  ��ģʽ
	* @return   ��д�ַ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

PLAT_FILE *plat_fopen(const char *path,const char *mode);


/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� �ر��ļ�
	* @param[in]   stream:  �ر��ļ����

	* @return   �ɹ���ʧ��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_fclose(PLAT_FILE *stream);



/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ�  �ж��Ƿ񵽴��ļ�ĩβ
	* @param[in]   stream:  �ļ����

	* @return   1 ��ʾ�����ļ�ĩβ  0 ��ʾû��
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/

int plat_feof(PLAT_FILE *stream);


/*!\brief ƽ̨ͳһ��׼�ӿ�
	* 
	* ƽ̨ͳһ��׼�ӿ� ���ļ�
	* @param[in]   ptr:  ������
	* @param[in]   size:  Ԫ�ش�С
	* @param[in]   nmemb:  Ԫ�ظ���
	* @param[in]   stream:  �ļ����
	* @return   ��ȡ����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see plat_printf()
*/
SIZE_T plat_fread(void *ptr,SIZE_T size,SIZE_T nmemb, PLAT_FILE *stream);



#endif  //_PLATFORM_STDIO_H_

