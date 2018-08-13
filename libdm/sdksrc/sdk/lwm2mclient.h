/** 
	* @file 		lwm2mclient.h
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

#ifndef LWM2MCLIENT_H_
#define LWM2MCLIENT_H_


#include "liblwm2m.h"
#include "internals.h"

#define LOG_M(FMT, ...) lwm2m_printf("[%s:%d] " FMT "\r\n", __func__ , __LINE__ , __VA_ARGS__)


#define LOG_S(STR) 		LOG_M("%s",STR)
#define TEST_OBJECT_ID 31024

lwm2m_object_t * get_object_device(void);
void free_object_device(lwm2m_object_t * objectP);
lwm2m_object_t * get_server_object(int serverId,int lifetime);
void free_server_object(lwm2m_object_t * object);
lwm2m_object_t * get_security_object(int serverId,const char* serverUri);
void free_security_object(lwm2m_object_t * objectP);
char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);

#endif /* LWM2MCLIENT_H_ */

