
/** 
	* @file 		plat_net.c 
	* @brief		系统库包装层 包装 网络通信相关接口
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	

#if defined(LINUX_PLAT) 


#include "plat_net.h"
#include "plat_stdio.h"
#include "plat_stddef.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <net/if.h>




int   plat_socket(int    domain, int type, int protocol)
{
	int Ldomain,Ltype,Lprotocol;
	switch(domain)
	{
		case PLAT_AF_INET: 
			Ldomain = AF_INET;
			break;
		case PLAT_AF_INET6: 
			Ldomain = AF_INET6;
			break;
		default:
			plat_printf("plat_socket error not process domain :%d ",domain);
			return -1;

	}
	switch(type)
	{
		case PLAT_SOCK_STREAM: 
			Ltype = SOCK_STREAM;
			break;
		case PLAT_SOCK_DGRAM: 
			Ltype = SOCK_DGRAM;
			break;
		default:
			plat_printf("plat_socket error not process type :%d ",type);
			return -1;

	}
	
	switch(protocol)
	{
		case PLAT_IPPROTO_IP: 
			Lprotocol = IPPROTO_IP;
			break;
		case PLAT_IPPROTO_TCP: 
			Lprotocol = IPPROTO_TCP;
			break;
		case PLAT_IPPROTO_UDP: 
			Lprotocol = IPPROTO_UDP;
			break;
		case 0: 
			Lprotocol = 0;
			break;
		default:
			plat_printf("plat_socket error not process protocol :%d ",type);
			return -1;

	}
	plat_printf("plat_socket domain =%d   type =%d  protocol = %d \n ",Ldomain,Ltype,Lprotocol);
	return socket(Ldomain,Ltype,Lprotocol);
	

}




int   plat_socket_close(int      sock)
{
	return close(sock);
}


static void plat2LinuxIp4addr(struct sockaddr_in  *dest,struct plat_sockaddr_in4 *src)
{
	dest->sin_family = AF_INET;
	memcpy(&dest->sin_port,&src->sin_port, sizeof(uint16_t));
	memcpy(&dest->sin_addr,&src->sin_addr,sizeof(struct plat_in4_addr));
}


static void plat2LinuxIp6addr(struct sockaddr_in6  *dest,struct plat_sockaddr_in6 *src)
{
	dest->sin6_family = AF_INET6;
	memcpy(&dest->sin6_port , &src->sin6_port , sizeof(uint16_t));
	memcpy(&dest->sin6_addr , &src->sin6_addr , sizeof(struct plat_in6_addr));
	memcpy(&dest->sin6_flowinfo , &src->sin6_flowinfo , sizeof(uint32_t));
	memcpy(&dest->sin6_scope_id , &src->sin6_scope_id , sizeof(uint32_t));
}


int   plat_bind(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len)
{
	int family =  addr->net_family;
	struct sockaddr_storage Laddr;
	memset(&Laddr,0,sizeof(struct sockaddr_storage));
	switch(family)
	{
		case PLAT_AF_INET:
			plat2LinuxIp4addr((struct sockaddr_in *)&Laddr, (struct plat_sockaddr_in4 *)addr);
			return  bind(sock,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in));
			break;
		case PLAT_AF_INET6:
			
			plat2LinuxIp6addr((struct sockaddr_in6 *)&Laddr,(struct plat_sockaddr_in6 *)addr);
			return bind(sock,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in6));
			break;
		default: 
			plat_printf("addr is error family ",family);
			return -1;
	}
	
}




int   plat_connect(int     sock, const struct  plat_sockaddr_storage *addr, int addr_len)
{
	int family =  addr->net_family;
	struct sockaddr_storage Laddr;
	memset(&Laddr,0,sizeof(struct sockaddr_storage));
	switch(family)
	{
		case PLAT_AF_INET:
		
			plat2LinuxIp4addr((struct sockaddr_in *)&Laddr, (struct plat_sockaddr_in4 *)addr);
			return connect(sock,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in));
			break;
		case PLAT_AF_INET6:
			
			plat2LinuxIp6addr((struct sockaddr_in6 *)&Laddr,(struct plat_sockaddr_in6 *)addr);
			return connect(sock,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in6));
			break;
		default: 
			plat_printf("addr is error family ",family);
			return -1;
	}
}


int plat_send(int sockfd, const void *buf, SIZE_T len, int flagss)
{
	return send(sockfd,buf,len,flagss);
}



int plat_sendmsg(int sockfd, struct plat_msghdr *msg, int flags)
{
	struct msghdr msgh;
	int ret;
	if(sizeof(struct msghdr) != sizeof(struct plat_msghdr))
	{
		return -1;
		printf("plat_sendmsg msg struct not equal \n ");
	}

	ret =  sendmsg(sockfd,(struct msghdr*)msg,flags);
	if (ret < 0 )
	{
		printf("plat_sendmsg error error = %d  %s  \n ",errno,strerror(errno));
	}
	return ret;

}


SIZE_T   plat_sendto(int    sock,const void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *destaddr, int addr_len)
{
	
	int family =  destaddr->net_family;
	struct sockaddr_storage Laddr;
	memset(&Laddr,0,sizeof(struct sockaddr_storage));
	switch(family)
	{
		case PLAT_AF_INET:
			plat2LinuxIp4addr((struct sockaddr_in *)&Laddr, (struct plat_sockaddr_in4 *)destaddr);
			return sendto(sock,buf,buflen,flags,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in));
			break;
		case PLAT_AF_INET6:
			plat2LinuxIp6addr((struct sockaddr_in6 *)&Laddr,(struct plat_sockaddr_in6 *)destaddr);
			return sendto(sock,buf,buflen,flags,(struct sockaddr *)&Laddr,sizeof(struct sockaddr_in6));
			break;
		default: 
			plat_printf("addr is error family ",family);
			return -1;
	}

}



static void linux2platIp4addr(struct plat_sockaddr_in4  *dest,struct sockaddr_in *src)
{
	dest->sin_family = PLAT_AF_INET;
	memcpy(&dest->sin_port,&src->sin_port, sizeof(uint16_t));
	memcpy(&dest->sin_addr,&src->sin_addr,sizeof(struct plat_in4_addr));
}


static void linux2platIp6addr(struct plat_sockaddr_in6  *dest,struct sockaddr_in6 *src)
{
	dest->sin6_family = PLAT_AF_INET6;
	memcpy(&dest->sin6_port , &src->sin6_port , sizeof(uint16_t));
	memcpy(&dest->sin6_addr , &src->sin6_addr , sizeof(struct plat_in6_addr));
	memcpy(&dest->sin6_flowinfo , &src->sin6_flowinfo , sizeof(uint32_t));
	memcpy(&dest->sin6_scope_id , &src->sin6_scope_id , sizeof(uint32_t));
}



SIZE_T   
plat_recvfrom(int     sock, void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *srcaddr, int *addr_len)
{
	
	int ret = 0;
	struct sockaddr_storage Lsrcaddr;
    socklen_t Llen;
	Llen = sizeof(Lsrcaddr);
	
	ret = recvfrom(sock,buf,buflen,flags,(struct sockaddr *)&Lsrcaddr,&Llen);

	if(ret == -1 ) 
	{
		return ret;
	}

	switch(Lsrcaddr.ss_family)
	{
		case AF_INET:
			*addr_len = sizeof(struct plat_sockaddr_in4);
			linux2platIp4addr((struct plat_sockaddr_in4 *)srcaddr,(struct sockaddr_in *)&Lsrcaddr);
			
			break;
		case AF_INET6:
			*addr_len = sizeof(struct plat_sockaddr_in6) ;
			linux2platIp6addr((struct plat_sockaddr_in6 *)srcaddr,(struct sockaddr_in6 *)&Lsrcaddr);
			break;
		default: 
			plat_printf("addr is error family ",Lsrcaddr.ss_family);
			return -1;
	}
	return ret;
}


uint16_t   plat_htons(uint16_t num)
{
	return htons(num);
}


uint16_t   plat_ntohs(uint16_t num)
{
	return ntohs(num);
}




int    plat_pton(FAMILLY_T af,const char* src,void *dst)
{
	int ret = 0;
	switch(af)
	{
		case PLAT_AF_INET:
			ret = inet_pton(AF_INET,src,dst);
			return ret;
			break;
		case PLAT_AF_INET6:
			return inet_pton(AF_INET6,src,dst);
			break;
		default: 
			plat_printf("addr is error family \n",af);
			return -1;
	}
	
}




const char *plat_ntop(FAMILLY_T af,const char* src,void *dst,int addrlen)
{
	switch(af)
	{
		case PLAT_AF_INET:
			return inet_ntop(AF_INET,src,dst,INET6_ADDRSTRLEN);
			break;
		case PLAT_AF_INET6:
			return inet_ntop(AF_INET6,src,dst,INET6_ADDRSTRLEN);
			break;
		default: 
			plat_printf("addr is error family ",af);
			return NULL;
	}	
}


/* 传统 linux 获取eth0等设备地址 */
static int get_local_ip(char * ifname, char * ip)
{
    char *temp = NULL;
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0); 

    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    memcpy(ifr.ifr_name, ifname, strlen(ifname));

    if(0 != ioctl(inet_sock, SIOCGIFADDR, &ifr)) 
    {   
        perror("ioctl error");
        return -1;
    }

    temp = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);     
    memcpy(ip, temp, strlen(temp));

    close(inet_sock);

    return 0;
}


int    plat_get_hostIP(char *ip)
{

	return get_local_ip("eth0",ip);
}



int    plat_get_IpByName(const char *host,char *ip,const char *port)
{
	struct addrinfo hints;
    struct addrinfo *servinfo = NULL;
    struct addrinfo *p;
    int s;
	int ret =-1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
	struct sockaddr_in	*hostaddr = NULL; 

    if (0 != getaddrinfo(host, port, &hints, &servinfo) || servinfo == NULL) return -1;
	s = -1;
    for(p = servinfo ; p != NULL && s == -1 ; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s >= 0)
        {
            if (-1 == connect(s, p->ai_addr, p->ai_addrlen))
            {
                close(s);
                s = -1;
            }else {
				hostaddr = (struct sockaddr_in	*)p->ai_addr;
				ret = 0;
            }
        }
    }
	
	if (s >= 0)
    {
     	
        inet_ntop(AF_INET, &hostaddr->sin_addr, ip, INET6_ADDRSTRLEN);
		plat_printf("ip = %s \n",ip);
        close(s);
    }
	if (NULL != servinfo) {
		plat_free(servinfo);
	}

	return ret;

}



int    plat_socket_read(int sock, void *buf, SIZE_T nbyte)
{
	return read(sock,buf,nbyte);
}



int plat_recv(int sockfd,  void *buf, SIZE_T len, int flagss)
{
	return recv(sockfd,buf,len,flagss);
}





#endif  




