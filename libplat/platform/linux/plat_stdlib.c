
/** 
	* @file 		plat_stdlib.c 
	* @brief		系统库包装层 包装 stdlib
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	









//各平台需要实现

#if defined(LINUX_PLAT) 

#include "plat_stdlib.h"
#include "unistd.h"





void*  plat_malloc(SIZE_T size)
{
	return malloc(size);
}





void  plat_free(void* ptr)
{
	free(ptr);
}


int  plat_atoi(const char *ptr)
{
	return atoi(ptr);
}


void  plat_srand(unsigned int seed)
{
	 srand(seed);
}

int  plat_rand(void)
{
	return rand();
}



int  plat_sleep(int seconds)
{
	return sleep(seconds);
}


void  plat_exit(int num)
{
	 exit(num);
}


#endif  


