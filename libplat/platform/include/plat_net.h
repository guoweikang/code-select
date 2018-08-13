
/** 
	* @file 		plat_net.h 
	* @brief		ϵͳ���װ�� ��װ ����ͨ����ؽӿ�
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	




#ifndef _PLATFORM_NET_H_
#define _PLATFORM_NET_H_





#include "plat_stdint.h"
#include "plat_stddef.h"
#include "plat_uio.h"

// ����ƽ̨��Ҫ����  

/*
SOCK_T   : sock fd ����
FAMILLY_T�� famliy ����
���� �� plat_socket
���� �� plat_socket_close
���� �� plat_bind
���� �� plat_connect

���� �� plat_htons
���� �� plat_ntohs
���� �� plat_pton
���� �� plat_ntop
���� �� plat_get_hostIP
���� �� plat_get_IpByName

���� �� plat_recvfrom
���� �� plat_socket_read
���� �� plat_sendmsg
���� �� plat_send
���� �� plat_recv
���� �� plat_sendto

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



/*!\brief ƽ̨ IP��ַ
* 
*����ƽ̨ IP ͨ�õ�ַ ���԰��� IPV4�� IPV6 (Structure large enough to hold any socket address) 
*/
struct plat_sockaddr_storage
{
    FAMILLY_T net_family;      /* Address family */
    char padding[_ADDR_SIZE - sizeof(FAMILLY_T)];   /*  pading */
};




typedef uint32_t in_addr_t;

/*!\brief ƽ̨IPV4 IP��ַ
* 
*����ƽ̨IPV4 IP ��ַ 
*/
struct plat_in4_addr  {
    in_addr_t s_addr;                    /* IPv4 address */
};



/*!\brief ƽ̨IPV4 ��ַ
* 
*����ƽ̨IPV4  ��ַ�ṹ 
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




/*!\brief ƽ̨IPV6 IP��ַ
* 
*����ƽ̨IPV6 IP ��ַ 
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

/*!\brief ƽ̨IPV6��ַ�ṹ��
* 
*����ƽ̨IPV6 ��ַ���ͽṹ��
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
	/*  ��Ϣ��Э���ַ Э���ַ���׽ӿ���Ϣ��
	�ڷ����ӵ�UDP�У�������Ҫָ���Է���ַ�˿ڣ�
	���ܷ����ڵĵ�������Դ���������Ҫ�Ļ���������ΪNULL
	����TCP�������ӵ�UDP�У�һ������ΪNULL��  */
    void  * msg_name ;   
    int msg_namelen ;   /*  ��ַ�ĳ���   */ 
    struct plat_iovec  * msg_iov ;   /*  ��io�������ĵ�ַ */ 
    SIZE_T  msg_iovlen ;   /*  �������ĸ���        */ 
    void  * msg_control ;   /*  �������ݵĵ�ַ       */ 
    SIZE_T msg_controllen ;   /*  �������ݵĳ���  */ 
    int  msg_flags ;   /*  ������Ϣ�ı�ʶ  */ 
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


#define PLAT_INET6_ADDRSTRLEN 64  ///< IPV6 ���ַ
#define PLAT_INET_ADDRSTRLEN 16 ///< IPV4 ���ַ


/*!\brief ƽ̨ �����׽��ֽӿ�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   domain: ��ַ������
	* @param[in]   type:   ��������
	* @param[in]   protocol: Э������
	* @return  sock �׽��� ʧ�� -1 
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

int   plat_socket(int    domain, int type, int protocol);

/*!\brief ƽ̨ �ر��׽���
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   sock: ͨ���׽���
	* @return  sock �׽���
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int   plat_socket_close(int      sock);





/*!\brief ƽ̨ ���׽���
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   sock: �׽���
	* @param[in]   addr:  ��ַ
	* @param[in]   addr_len: ��ַ����
	* @return  �ɹ�(0) ʧ��(-1)
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int   plat_bind(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len);




/*!\brief ƽ̨ ��������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   sock: �׽���
	* @param[in]   addr:  ��ַ
	* @param[in]   addr_len: ��ַ����
	* @return  �ɹ�(0) ʧ��(-1)
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int   plat_connect(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len);




/*!\brief ƽ̨ ��������
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   sock: �׽���
	* @param[in]   buf:  �����ͻ�����
	* @param[in]   buflen:  ��������С
	* @param[in]   flags:  ���ͱ�־
	* @param[in]   destaddr:  ���͵�ַ
	* @param[in]   addr_len: ��ַ����
	* @return  ���ͳ��� ʧ�� -1 
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
SIZE_T   plat_sendto(int    sock,const void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *destaddr, int addr_len);


/*!\brief ƽ̨ͳһ�ӿ� ��������
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   sock: �׽���
	* @param[in]   buf:  �����ͻ�����
	* @param[in]   buflen:  ��������С
	* @param[in]   flags:  ���ͱ�־
	* @param[out]   destaddr:  ���յ�ַ
	* @param[out]   addr_len: ���յ�ַ����
	* @return  ���ͳ��� ʧ�� -1 
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
SIZE_T   plat_recvfrom(int    sock, void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *srcaddr, int *addr_len);




/*!\brief ƽ̨ �ӿ� ������ת�������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   num: ��ת���ֽ�
	* @return  ���������ֽ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
uint16_t   plat_htons(uint16_t num);

/*!\brief ƽ̨ �ӿ� ������ת������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   num: ��ת���ֽ�
	* @return  ���������ֽ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
uint16_t   plat_ntohs(uint16_t num);



/*!\brief ƽ̨ �ӿ� ת��IP��ַΪ������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   af: ��ַ������ V4 or V6
	* @param[in]   src: ԭIP �ֽ���
	* @param[out]   dst: ת�����ֽ�����
	* @return  returns 1 on success (network address was successfully converted).  0 is returned if src does not  contain  a  character
       string  representing  a valid network address in the specified address family.  If af does not contain a valid address family, -1 is
       returned and errno is set to EAFNOSUPPORT.
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int    plat_pton(FAMILLY_T af,const char* src,void *dst);



/*!\brief ƽ̨ �ӿ� ת��IP��ַΪ������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   af: ��ַ������ V4 or V6
	* @param[in]   src: ԭIP �ֽ���
    * @param[in]   addrlen: ������dst����
	* @param[out]   dst: ת�����ֽ�����
	* @return  �ɹ�(0) ʧ��(-1)
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
const char  *plat_ntop(FAMILLY_T af,const char* src,void *dst,int addrlen);



/*!\brief ƽ̨ �ӿ� ��ȡ����������IP 
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[out]   ip: ����ip��ŵ�ַ
	* @return  �ɹ�(0) ʧ��(-1)
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int    plat_get_hostIP(char *ip);



/*!\brief ������������ȡIP 
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]   host: ��������
	* @param[out]   ip: ip ��ַ
	* @return  �ɹ�(0) ʧ��(-1)
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int    plat_get_IpByName(const char *host,char *ip,const char* port);



/*!\brief socket ���ݶ�ȡ �����ӿ�
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  sock��
	* @param[out] buf:
	* @param[in] nbyte:
	* @return  ��ȡ�ֽ���
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
int    plat_socket_read(int sock, void *buf, SIZE_T nbyte);


/*!\brief socket ��������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  sockfd��
	* @param[in] msg:
	* @param[in] flags:
	* @return  �����ֽ���
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

int plat_sendmsg(int sockfd, struct plat_msghdr *msg, int flags);


/*!\brief socket ��������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  sockfd��
	* @param[in] buf:
	* @param[in] len:
	* @return  �����ֽ���
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

int plat_send(int sockfd, const void *buf, SIZE_T len, int flags);


/*!\brief socket ��������
	* 
	* ƽ̨ͳһ��׼����ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[in]  sockfd��
	* @param[in] buf:
	* @param[in] len:
	* @return  �����ֽ���
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/

int plat_recv(int sockfd,  void *buf, SIZE_T len, int flagss);

#endif  // _PLATFORM_INTTYPES_H_



