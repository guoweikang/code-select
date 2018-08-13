/** 
	* @file 		j_base64.h 
	* @brief		
	* @details		
	* @author		郭伟康
	* @date 	2018/7/12
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/12 , 第一次创建\n 
*/	


#ifndef __J_BASE_64_H__
#define __J_BASE_64_H__

#include "plat_stdio.h"
#include "plat_string.h"


#ifdef __cplusplus
extern "C" {
#endif

int j_base64_encode(unsigned char *data, unsigned int dataLen, 
		unsigned char **encData, unsigned int *encDataLen);
int j_base64_decode(unsigned char *data, unsigned int dataLen, 
		unsigned char **decData, unsigned int *decDataLen);

void j_base64_free(void *data, SIZE_T size);

#ifdef __cplusplus
}
#endif


#endif //__J_BASE_64_H__


