
/** 
	* @file 		plat_uio.h 
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




#ifndef _PLATFORM_UIO_H_
#define _PLATFORM_UIO_H_




//各平台需要实现time_t        参考linux : __extension__ typedef long int time_t


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


