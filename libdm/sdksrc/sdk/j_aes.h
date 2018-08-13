
/** 
	* @file 		j_aes.h 
	* @brief		
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/12
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/12 , ��һ�δ���\n 
*/	

#ifndef __J_AES_H__
#define __J_AES_H__

#include "plat_stdio.h"
#include "plat_string.h"


#ifdef __cplusplus
extern "C" {
#endif

void  hex_print(const char *name, const unsigned char *buf, SIZE_T len);
void  HexStrToByte(const char* source, int sourceLen, unsigned char* dest);
int   EncryptionAES( char *szin, int inlen,unsigned char *key,unsigned char *ciphertext);
char* StrSHA256(const char* str, int length, char* sha256);

#ifdef __cplusplus
}
#endif


#endif //__J_BASE_64_H__


