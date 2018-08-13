/** 
	* @file 		plat_stdlib.c 
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
#include "cos.h"
#include "ctype.h"
#include "stdlib.h"
#include "plat_stdlib.h"

/*!\brief 平台 申请内存函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   size: 申请内存大小
	* @return  指针地址
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台 释放内存函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   ptr: 待释放指针地址
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台 申请内存并复制数据函数
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   str: 待复制指针地址
	* @return  成功:指针地址 失败:NULL
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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


/*!\brief 平台 字符串转整型
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   ptr: 带转换字符串
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台 设置rand seed 
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]   seed: 随机数种子
	* @return  无
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台 获得随机数 
	* 
	* 平台统一标准接口，各个平台内部各自实现

	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台睡眠函数
	* 
	* 平台统一标准接口，各个平台内部各自实现

	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

/*!\brief 平台程序退出函数
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]:num 退出码
	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
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

