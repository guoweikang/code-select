
/** 
	* @file 		plat_stdlib.c 
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









//��ƽ̨��Ҫʵ��

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


