/** 
	* @file 		connection.c
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

#include "plat_stdlib.h"
#include "plat_string.h"
#include "plat_net.h"

#include "connection.h"

// from commandline.c
void output_buffer(PLAT_FILE * stream, uint8_t * buffer, int length, int indent);

int create_socket(const char * portStr, FAMILLY_T addressFamily)
{

    int  s = -1;
	struct	plat_sockaddr_storage addr;
	memset(&addr,0,sizeof(struct plat_sockaddr_storage));
	
	short port = plat_atoi(portStr);
	char hostIp[100] = {0};

	if( 0 != plat_get_hostIP(hostIp)) return -1;

	char *hostIp2 = "0.0.0.0";
	plat_printf("host ip = %s \n",hostIp);
   /*modify by guoweikang by 2018.07.12 */
	if (addressFamily == PLAT_AF_INET)
	{
		struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)&(addr);
		addrv4->sin_family = PLAT_AF_INET;
		addrv4->sin_port = plat_htons(port);

		if ( -1 == plat_pton(PLAT_AF_INET,hostIp2,&addrv4->sin_addr)) return -1;
		
	}else if(addressFamily == PLAT_AF_INET6)
	{
		struct plat_sockaddr_in6 *addrv6 = (struct plat_sockaddr_in6 *)&(addr);
		addrv6->sin6_family = PLAT_AF_INET6;
		addrv6->sin6_port = plat_htons(port);
		if ( -1 == plat_pton(PLAT_AF_INET6,hostIp,&addrv6->sin6_addr)) return -1;
	}
	
	plat_printf("create socket \n");
    s = plat_socket(addressFamily, PLAT_SOCK_DGRAM,PLAT_IPPROTO_UDP);
	plat_printf("create socket success sock = %d \n",s);
	 if (s >= 0)
    {
       
       if (-1 == plat_bind(s, &addr, sizeof(struct plat_sockaddr_storage)))
       {
          plat_socket_close(s);
          s = -1;
       }
    }

    return s;
}

connection_t * connection_find(connection_t * connList,
                               struct plat_sockaddr_storage * addr,
                               SIZE_T addrLen)
{
    connection_t * connP;

    connP = connList;
    while (connP != NULL)
    {
    	char ip[100]= {};
    	plat_printf("****************conlist addrlen =%d  findlen = %d \n",connP->addrLen,addrLen);
		plat_printf("*************  CONNP  af = %d   port =  %d ip = %s len = %d  \n",
			connP->addr.sin6_family,((struct plat_sockaddr_in4 *)&(connP->addr))->sin_port,
			plat_ntop(connP->addr.sin6_family,&(((struct plat_sockaddr_in4 *)&(connP->addr))->sin_addr),ip,16),
			connP->addrLen );
		plat_printf("*************  find:  af = %d   port =  %d ip = %s len = %d  \n",
			addr->net_family,((struct plat_sockaddr_in4 *)addr)->sin_port,
			plat_ntop(addr->net_family,&(((struct plat_sockaddr_in4 *)addr)->sin_addr),ip,16),
			addrLen);
		if ((connP->addrLen == addrLen)
         && (memcmp(&(connP->addr), addr, addrLen) == 0))
        {
            return connP;
        }
        connP = connP->next;
    }

    return connP;
}

connection_t * connection_new_incoming(connection_t * connList,
                                       int sock,
                                       struct plat_sockaddr_storage * addr,
                                       SIZE_T addrLen)
{
    connection_t * connP;

    connP = (connection_t *)plat_malloc(sizeof(connection_t));
    if (connP != NULL)
    {
        connP->sock = sock;
        memcpy(&(connP->addr), addr, addrLen);
        connP->addrLen = addrLen;
        connP->next = connList;
    }

    return connP;
}

connection_t * connection_create(connection_t * connList,
                                 int sock,
                                 char * host,
                                 char * port,
                                 FAMILLY_T addressFamily)
{

    int s;
    struct plat_sockaddr_storage addr;
	memset(&addr,0,sizeof(struct plat_sockaddr_storage));
    int sl = 0;
    connection_t * connP = NULL;
	short sport = plat_atoi(port);

	char ip[100];
	if(0 != plat_get_IpByName(host,ip,port)) return NULL;
	/*modify by guoweikang by 2018.07.12 */
	 if (addressFamily == PLAT_AF_INET)
	 {
		 struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)&(addr);
		 addrv4->sin_family = PLAT_AF_INET;
		 addrv4->sin_port = plat_htons(sport);
		 if ( -1 == plat_pton(PLAT_AF_INET,ip,&addrv4->sin_addr)) 
		 {
		 	return NULL;
		 }

		 sl = sizeof(struct plat_sockaddr_in4);
		 
	 }else if(addressFamily == PLAT_AF_INET6)
	 {
		 struct plat_sockaddr_in6 *addrv6 = (struct plat_sockaddr_in6 *)&(addr);
		 addrv6->sin6_family = PLAT_AF_INET6;
		 addrv6->sin6_port = plat_htons(sport);
		 if ( -1 == plat_pton(PLAT_AF_INET6,ip,&addrv6->sin6_addr)) return NULL;

		 sl = sizeof(struct plat_sockaddr_in6);
	 }
    s = -1;
    s = plat_socket(addressFamily,PLAT_SOCK_DGRAM, PLAT_IPPROTO_UDP);
    if (s >= 0)
    {
      
        if (-1 == plat_connect(s, &addr, sl))
        {
            plat_socket_close(s);
            s = -1;
        }
    }
    if (s >= 0)
    {
        connP = connection_new_incoming(connList, sock, &addr, sl);
        plat_socket_close(s);
    }

    return connP;
}

void connection_free(connection_t * connList)
{
    while (connList != NULL)
    {
        connection_t * nextP;

        nextP = connList->next;
        plat_free(connList);

        connList = nextP;
    }
}

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    SIZE_T length)
{
    int nbSent;
    SIZE_T offset;

#ifdef WITH_LOGS
    char s[PLAT_INET6_ADDRSTRLEN];
    uint16_t port;

    s[0] = 0;

    if (PLAT_AF_INET == connP->addr.sin6_family)
    {
        struct plat_sockaddr_in4 *saddr = (struct plat_sockaddr_in4 *)&connP->addr;
        plat_ntop(saddr->sin_family, &saddr->sin_addr, s, PLAT_INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (PLAT_AF_INET6 == connP->addr.sin6_family)
    {
        struct plat_sockaddr_in6 *saddr = (struct plat_sockaddr_in4 *)&connP->addr;
        plat_ntop(saddr->sin6_family, &saddr->sin6_addr, s, PLAT_INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    }

    plat_printf(PLAT_STDERR, "Sending %d bytes to [%s]:%hu\r\n", length, s, plat_ntohs(port));

    output_buffer(PLAT_STDERR, buffer, length, 0);
#endif

    offset = 0;
    while (offset != length)
    {
        nbSent = plat_sendto(connP->sock, buffer + offset, length - offset, 0, (struct plat_sockaddr_storage *)&(connP->addr), connP->addrLen);
        if (nbSent == -1) return -1;
        offset += nbSent;
    }
    return 0;
}

uint8_t lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          SIZE_T length,
                          void * userdata)
{
    connection_t * connP = (connection_t*) sessionH;

    if (connP == NULL)
    {
        plat_fprintf(PLAT_STDERR, "#> failed sending %lu bytes, missing connection\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length))
    {
        plat_fprintf(PLAT_STDERR, "#> failed sending %lu bytes\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    return (session1 == session2);
}

