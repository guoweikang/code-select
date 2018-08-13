
/** 
	* @file 		plat_uio.h 
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




#ifndef _PLATFORM_UIO_H_
#define _PLATFORM_UIO_H_




//��ƽ̨��Ҫʵ��time_t        �ο�linux : __extension__ typedef long int time_t


#if defined(LINUX_PLAT) 




#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif






#ifdef __cplusplus
}
#endif


#elif defined(NB_PLAT)





#else 

#error NO PLATFORM DEFINE 




#endif   


struct plat_iovec {
    void *iov_base;
    SIZE_T iov_len;
};


#endif  //_PLATFORM_UIO_H_


