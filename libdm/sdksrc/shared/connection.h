/** 
	* @file 		connection.h
	* @brief		
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/11 , 第一次创建\n 
*/	

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "plat_stdio.h"

#include "plat_net.h"

#include "liblwm2m.h"

#define LWM2M_STANDARD_PORT_STR "5683"
#define LWM2M_STANDARD_PORT      5683
#define LWM2M_DTLS_PORT_STR     "5684"
#define LWM2M_DTLS_PORT          5684
#define LWM2M_BSSERVER_PORT_STR "5685"
#define LWM2M_BSSERVER_PORT      5685

typedef struct _connection_t
{
    struct _connection_t *  next;
    int                     sock;
    struct  plat_sockaddr_in6    addr;
    SIZE_T                  addrLen;
} connection_t;

int create_socket(const char * portStr, FAMILLY_T ai_family);

connection_t * connection_find(connection_t * connList, struct plat_sockaddr_storage * addr, SIZE_T addrLen);
connection_t * connection_new_incoming(connection_t * connList, int sock, struct plat_sockaddr_storage * addr, SIZE_T addrLen);
connection_t * connection_create(connection_t * connList, int sock, char * host, char * port, FAMILLY_T addressFamily);

void connection_free(connection_t * connList);

int connection_send(connection_t *connP, uint8_t * buffer, SIZE_T length);

#endif

