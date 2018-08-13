
/** 
	* @file 		plat_stdio.h 
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
#include "ctype.h"
#include "cmiot_fs.h"
#include "plat_stdio.h"
#include "at_trace.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

extern UINT32 AT_OutputText(UINT16 nIndex, PCSTR fmt, ...);

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

int plat_printf(const char* format,...)
{
	//AT_OutputText(g_sw_CH, "[DM TRACE]>>"#format, ##__VA_ARGS__);
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000001);
	hal_HstSendEvent(0xffffffff);

    va_list ap;
    int len;
	char *str;

	str = plat_malloc(1024);

	memset(str, 0, 1024);

    va_start(ap, format);
    len = vsnprintf(str, -(size_t)str, format, ap);
    va_end(ap);

	AT_WriteUart(str, strlen(str));

	plat_free(str);

	return (0);
}

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

int plat_fprintf(PLAT_FILE *stream,const char* format,...)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000002);
	hal_HstSendEvent(0xffffffff);

	if (stream == PLAT_STDIN)
	{
		
	}
	else if (stream == PLAT_STDOUT)
	{
		//AT_OutputText(g_sw_CH, "[DM TRACE]>>"#format, ##__VA_ARGS__);
	}
	else if (stream == PLAT_STDERR)
	{
		//AT_OutputText(g_sw_CH, "[DM ERROR]>>"#format, ##__VA_ARGS__);
	}
	else
	{

	}
	va_list ap;
	int len;
	char *str;

	str = plat_malloc(1024);

	memset(str, 0, 1024);
	
	va_start(ap, format);
	len = vsnprintf(str, -(size_t)str, format, ap);
	va_end(ap);
	
	AT_WriteUart(str, strlen(str));

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000012);
	hal_HstSendEvent(0xffffffff);

	plat_free(str);

	return (0);
}

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

int plat_sprintf(char *str,const char* format,...)
{
    va_list ap;
    int len;
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000003);
	hal_HstSendEvent(0xffffffff);

    va_start(ap, format);
    len = vsnprintf(str, -(size_t)str, format, ap);
    va_end(ap);

    return (len);
}

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

int plat_snprintf(char *str,SIZE_T size,const char* format,...)
{
    va_list ap;
    int len;
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000004);
	hal_HstSendEvent(0xffffffff);

    va_start(ap, format);
    len = vsnprintf(str, size, format, ap);
    va_end(ap);
    return (len);
}

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

int plat_sscanf(const char  *str,const char* format,...)
{
    va_list args;
    int i;
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000005);
	hal_HstSendEvent(0xffffffff);

    va_start(args,format);
    i = vsscanf(str,format,args);
    va_end(args);
    return i;
}

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

int plat_isprint(int c )
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000006);
	hal_HstSendEvent(0xffffffff);

	return isprint(c);
}

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

int plat_isspace(int c )
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000007);
	hal_HstSendEvent(0xffffffff);

	return isspace(c);
}

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
int plat_isalnum(int c )
{
	return isalnum(c);
}

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

int plat_toupper(int c )
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000008);
	hal_HstSendEvent(0xffffffff);

	return toupper(c);
}


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

PLAT_FILE *plat_fopen(const char *path,const char *mode)
{
	UINT32 open_mode;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x20000009);
	hal_HstSendEvent(0xffffffff);

	if (strcmp(mode, "r") == 0)
	{
		open_mode = FS_O_RDONLY;
	}
	else if (strcmp(mode, "w") == 0)
	{
		open_mode = FS_O_WRONLY;
	}
		
	return CMIOT_FS_Open(path, open_mode);
}

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

int plat_fclose(PLAT_FILE *stream)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x2000000A);
	hal_HstSendEvent(0xffffffff);

	return CMIOT_FS_Close(stream);
}

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

int plat_feof(PLAT_FILE *stream)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x2000000B);
	hal_HstSendEvent(0xffffffff);

	return CMIOT_FS_IsEndOfFile(stream);
}

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
SIZE_T plat_fread(void *ptr,SIZE_T size,SIZE_T nmemb, PLAT_FILE *stream)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x2000000C);
	hal_HstSendEvent(0xffffffff);

	return CMIOT_FS_Read(stream, ptr, size);
}

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

int plat_tolower(int c )
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x2000000d);
	hal_HstSendEvent(0xffffffff);

	return tolower(c);
}


