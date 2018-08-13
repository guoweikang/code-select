
/** 
	* @file 		plat_stdio.h 
	* @brief		系统库包装层 包装 time
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	
#include "ctype.h"
#include "cmiot_fs.h"
#include "plat_stdio.h"
#include "at_trace.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

extern UINT32 AT_OutputText(UINT16 nIndex, PCSTR fmt, ...);

/*!\brief 平台统一标准输出接口，各个平台各自实现
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   format 输出数据格式	
	* @return  输出长度
	* @par format   
	*	   保留 
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准输出接口
	* 
	* 平台统一标准输出接口，各个平台内部各自实现，可以通过PLAT_FILE 指定输出方向
	* @param[in]   stream: 输出数据流
	* @param[in]   format 输出数据格式	
	* @return  输出长度
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准输出接口
	* 
	* 平台统一标准输出接口，各个平台内部各自实现，可以通过PLAT_FILE 指定输出方向
	* @param[in]   str: 输出缓冲区
	* @param[in]   format 输出数据格式	
	* @return  输出长度
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准输出接口
	* 
	* 平台统一标准输出接口，各个平台内部各自实现，可以通过PLAT_FILE 指定输出方向
	* @param[in]   str: 输出缓冲区
	* @param[in]   size: 缓冲区长度
	* @param[in]   format 输出数据格式	
	* @return  输出长度
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口，扫描字符串
	* @param[in]   str: 待分析字符串
	* @param[in]   format 需要提取格式
	* @param[out]  ...：可变参数 输出扫描参数
	* @return  读取参数个数 
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 判断字符是否可打印显示
	* @param[in]   c:  待判断字符
	* @return   可打印（1） 不可打印（0）
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 判断字符是否是 space
	* @param[in]   c:  待判断字符
	* @return   是 (1) 不是（0）
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 判断数字
	* @param[in]   c:  待判断字符
	* @return   是 (1) 不是（0）
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see plat_printf()
*/
int plat_isalnum(int c )
{
	return isalnum(c);
}

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 转换字符为大写 
	* @param[in]   c:  待转字符
	* @return   大写字符
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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


/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 打开文件
	* @param[in]   path:  路径
	* @param[in]   mode:  打开模式
	* @return   大写字符
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 关闭文件
	* @param[in]   stream:  关闭文件句柄

	* @return   成功或失败
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口  判断是否到达文件末尾
	* @param[in]   stream:  文件句柄

	* @return   1 表示到达文件末尾  0 表示没有
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 打开文件
	* @param[in]   ptr:  缓冲区
	* @param[in]   size:  元素大小
	* @param[in]   nmemb:  元素个数
	* @param[in]   stream:  文件句柄
	* @return   读取长度
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 转换字符为大写 
	* @param[in]   c:  待转字符
	* @return   大写字符
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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


