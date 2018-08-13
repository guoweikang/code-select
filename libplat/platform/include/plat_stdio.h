
/** 
	* @file 		plat_stdio.h 
	* @brief		系统库包装层 包装 标准io
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	




#ifndef _PLATFORM_STDIO_H_
#define _PLATFORM_STDIO_H_





#include "plat_stddef.h"
//各平台需要实现 
/*
PLAT_FILE 
PLAT_STDERR
PLAT_STDIN
PLAT_STDOUT


函数：  plat_printf
函数：  plat_fprintf
函数：  plat_sprintf
函数：  plat_snprintf 
函数：  plat_sscanf
函数：  plat_isprint
函数：  plat_isspace 
函数：  plat_isalnum 
函数：  plat_toupper 
函数：  plat_tolower 
函数：  plat_fopen 
函数：  plat_fclose 
函数：  plat_feof 
函数：  plat_fread 

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

int plat_printf(const char* format,...);


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

int plat_fprintf(PLAT_FILE *stream,const char* format,...);




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

int plat_sprintf(char *str,const char* format,...);



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

int plat_snprintf(char *str,SIZE_T size,const char* format,...);



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

int plat_sscanf(const char  *str,const char* format,...);


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

int plat_isprint(int c );

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

int plat_isspace(int c );

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

int plat_isalnum(int c );



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

int plat_toupper(int c );

/*!\brief 平台统一标准接口
	* 
	* 平台统一标准接口 转换字符为小写 
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

int plat_tolower(int c );


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

PLAT_FILE *plat_fopen(const char *path,const char *mode);


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

int plat_fclose(PLAT_FILE *stream);



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

int plat_feof(PLAT_FILE *stream);


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
SIZE_T plat_fread(void *ptr,SIZE_T size,SIZE_T nmemb, PLAT_FILE *stream);



#endif  //_PLATFORM_STDIO_H_

