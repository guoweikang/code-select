
/** 
	* @file 		plat_net.h 
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




#ifndef _PLATFORM_NET_H_
#define _PLATFORM_NET_H_





#include "plat_stdint.h"
#include "plat_stddef.h"
#include "plat_uio.h"

// 各个平台需要定义  

/*
SOCK_T   : sock fd 类型
FAMILLY_T： famliy 类型
函数 ： plat_socket
函数 ： plat_socket_close
函数 ： plat_bind
函数 ： plat_connect

函数 ： plat_htons
函数 ： plat_ntohs
函数 ： plat_pton
函数 ： plat_ntop
函数 ： plat_get_hostIP
函数 ： plat_get_IpByName

函数 ： plat_recvfrom
函数 ： plat_socket_read
函数 ： plat_sendmsg
函数 ： plat_send
函数 ： plat_recv
函数 ： plat_sendto

*/


/******************************************
		LINUX PLATFORM INC
******************************************/

#if defined(LINUX_PLAT) 



#ifdef __cplusplus
extern "C" {
#endif




typedef int SOCK_T;
typedef short  FAMILLY_T;


#ifdef __cplusplus
}
#endif

/******************************************
		C216B PLATFORM INC
******************************************/


#elif defined(C216B_PLAT)


#ifdef __cplusplus
extern "C" {
#endif
#include "cfw.h"


typedef uint8_t SOCK_T;

typedef uint8_t  FAMILLY_T;




#ifdef __cplusplus
}
#endif


/******************************************
		NB PLATFORM INC
******************************************/


#elif defined(NB_PLAT)

#ifdef __cplusplus
extern "C" {
#endif







#ifdef __cplusplus
}
#endif


#else 

#error NO PLATFORM DEFINE 
#endif   




/******************************************
		common
******************************************/




#define _ADDR_SIZE        128



/*!\brief 平台 IP地址
* 
*定义平台 IP 通用地址 可以包含 IPV4和 IPV6 (Structure large enough to hold any socket address) 
*/
struct plat_sockaddr_storage
{
    FAMILLY_T net_family;      /* Address family */
    char padding[_ADDR_SIZE - sizeof(FAMILLY_T)];   /*  pading */
};




typedef uint32_t in_addr_t;

/*!\brief 平台IPV4 IP地址
* 
*定义平台IPV4 IP 地址 
*/
struct plat_in4_addr  {
    in_addr_t s_addr;                    /* IPv4 address */
};



/*!\brief 平台IPV4 地址
* 
*定义平台IPV4  地址结构 
*/
struct plat_sockaddr_in4 {
     FAMILLY_T sin_family;             /* AF_INET */
     uint16_t sin_port;                 /* Port number.  */
     struct plat_in4_addr sin_addr;            /* Internet address.  */
     /* Pad to size of `struct plat_sockaddr_storage'.  */
     unsigned char sin_zero[sizeof (struct plat_sockaddr_storage) -
                            sizeof (short) -
                            sizeof (uint16_t) -
                            sizeof (struct plat_in4_addr)];

							
};




/*!\brief 平台IPV6 IP地址
* 
*定义平台IPV6 IP 地址 
*/
struct plat_in6_addr
{
	union
	{
		uint8_t __u6_addr8[16];   // 128 bit
		uint16_t __u6_addr16[8];  // 64 bit
        uint32_t __u6_addr32[4];  // 32 bit
	} __in6_u;
#define s6_addr 		__in6_u.__u6_addr8
#define s6_addr16		__in6_u.__u6_addr16
#define s6_addr32		__in6_u.__u6_addr32
};

/*!\brief 平台IPV6地址结构体
* 
*定义平台IPV6 地址类型结构体
*/

struct plat_sockaddr_in6 {
	FAMILLY_T sin6_family;     		///< Address family; must be AF_INET6.
	uint16_t sin6_port;   		///< Transport layer port
	uint32_t sin6_flowinfo;		///< IPv6 flow information
	struct plat_in6_addr sin6_addr;  ///< Ipv6 address.
	uint32_t sin6_scope_id;		///< scope id (new in RFC2553)
	unsigned char sin_zero[sizeof (struct plat_sockaddr_storage) -
						   sizeof (short) -
						   sizeof (uint16_t) -
						   sizeof (uint32_t) -
						   sizeof (struct plat_in6_addr) -
						   sizeof (uint32_t)];
};





struct plat_msghdr  { 
	/*  消息的协议地址 协议地址和套接口信息，
	在非连接的UDP中，发送者要指定对方地址端口，
	接受方用于的到数据来源，如果不需要的话可以设置为NULL
	（在TCP或者连接的UDP中，一般设置为NULL）  */
    void  * msg_name ;   
    int msg_namelen ;   /*  地址的长度   */ 
    struct plat_iovec  * msg_iov ;   /*  多io缓冲区的地址 */ 
    SIZE_T  msg_iovlen ;   /*  缓冲区的个数        */ 
    void  * msg_control ;   /*  辅助数据的地址       */ 
    SIZE_T msg_controllen ;   /*  辅助数据的长度  */ 
    int  msg_flags ;   /*  接收消息的标识  */ 
} ;



//domain 
#define PLAT_AF_INET   0 ///< IPV4 
#define PLAT_AF_INET6  1 ///< IPV6

//type 
#define PLAT_SOCK_STREAM  0 ///< TCP/IP 
#define PLAT_SOCK_DGRAM  1 ///< UDP

//protocol 

#define PLAT_IPPROTO_IP 1 ///< IP

#define PLAT_IPPROTO_TCP 2 ///< TCP


#define PLAT_IPPROTO_UDP 3 ///< UDP


#define PLAT_INET6_ADDRSTRLEN 64  ///< IPV6 最长地址
#define PLAT_INET_ADDRSTRLEN 16 ///< IPV4 最长地址


/*!\brief 平台 创建套接字接口
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   domain: 地址簇类型
	* @param[in]   type:   连接类型
	* @param[in]   protocol: 协议类型
	* @return  sock 套接字 失败 -1 
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

int   plat_socket(int    domain, int type, int protocol);

/*!\brief 平台 关闭套接字
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   sock: 通信套接字
	* @return  sock 套接字
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int   plat_socket_close(int      sock);





/*!\brief 平台 绑定套接字
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   sock: 套接字
	* @param[in]   addr:  地址
	* @param[in]   addr_len: 地址长度
	* @return  成功(0) 失败(-1)
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int   plat_bind(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len);




/*!\brief 平台 建立连接
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   sock: 套接字
	* @param[in]   addr:  地址
	* @param[in]   addr_len: 地址长度
	* @return  成功(0) 失败(-1)
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int   plat_connect(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len);




/*!\brief 平台 发送数据
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]   sock: 套接字
	* @param[in]   buf:  待发送缓冲区
	* @param[in]   buflen:  缓冲区大小
	* @param[in]   flags:  发送标志
	* @param[in]   destaddr:  发送地址
	* @param[in]   addr_len: 地址长度
	* @return  发送长度 失败 -1 
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
SIZE_T   plat_sendto(int    sock,const void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *destaddr, int addr_len);


/*!\brief 平台统一接口 接收数据
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[in]   sock: 套接字
	* @param[in]   buf:  待发送缓冲区
	* @param[in]   buflen:  缓冲区大小
	* @param[in]   flags:  发送标志
	* @param[out]   destaddr:  接收地址
	* @param[out]   addr_len: 接收地址长度
	* @return  发送长度 失败 -1 
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
SIZE_T   plat_recvfrom(int    sock, void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *srcaddr, int *addr_len);




/*!\brief 平台 接口 主机序转网络序号
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   num: 待转序字节
	* @return  网络序两字节
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
uint16_t   plat_htons(uint16_t num);

/*!\brief 平台 接口 网络序转主机序
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   num: 待转序字节
	* @return  主机序两字节
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
uint16_t   plat_ntohs(uint16_t num);



/*!\brief 平台 接口 转换IP地址为网络序
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   af: 地址簇类型 V4 or V6
	* @param[in]   src: 原IP 字节序
	* @param[out]   dst: 转换后字节序存放
	* @return  returns 1 on success (network address was successfully converted).  0 is returned if src does not  contain  a  character
       string  representing  a valid network address in the specified address family.  If af does not contain a valid address family, -1 is
       returned and errno is set to EAFNOSUPPORT.
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int    plat_pton(FAMILLY_T af,const char* src,void *dst);



/*!\brief 平台 接口 转换IP地址为主机序
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   af: 地址簇类型 V4 or V6
	* @param[in]   src: 原IP 字节序
    * @param[in]   addrlen: 缓冲区dst长度
	* @param[out]   dst: 转换后字节序存放
	* @return  成功(0) 失败(-1)
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
const char  *plat_ntop(FAMILLY_T af,const char* src,void *dst,int addrlen);



/*!\brief 平台 接口 获取本主机对外IP 
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[out]   ip: 本机ip存放地址
	* @return  成功(0) 失败(-1)
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int    plat_get_hostIP(char *ip);



/*!\brief 根据主机名获取IP 
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   host: 主机名字
	* @param[out]   ip: ip 地址
	* @return  成功(0) 失败(-1)
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int    plat_get_IpByName(const char *host,char *ip,const char* port);



/*!\brief socket 数据读取 阻塞接口
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  sock：
	* @param[out] buf:
	* @param[in] nbyte:
	* @return  读取字节数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int    plat_socket_read(int sock, void *buf, SIZE_T nbyte);


/*!\brief socket 发送数据
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  sockfd：
	* @param[in] msg:
	* @param[in] flags:
	* @return  发送字节数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

int plat_sendmsg(int sockfd, struct plat_msghdr *msg, int flags);


/*!\brief socket 发送数据
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  sockfd：
	* @param[in] buf:
	* @param[in] len:
	* @return  发送字节数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

int plat_send(int sockfd, const void *buf, SIZE_T len, int flags);


/*!\brief socket 接收数据
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]  sockfd：
	* @param[in] buf:
	* @param[in] len:
	* @return  发送字节数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/

int plat_recv(int sockfd,  void *buf, SIZE_T len, int flagss);

#endif  // _PLATFORM_INTTYPES_H_



