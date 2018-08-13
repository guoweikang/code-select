
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



#include "plat_net.h"
#include "cfw.h"
#include "tcpip_inet.h"
#include "at.h"
#include "cfw.h"
#include "at_module.h"
#include "at_cmd_gprs.h"
#include "at_cmd_tcpip.h"

#define REC_SOCKET_MAX (10)
#define REC_BUFFER_LEN (1024)
UINT8 *rec_buff[REC_SOCKET_MAX] = {0, };
int rec_size[REC_SOCKET_MAX] = {0, };
SOCKET rec_socket[REC_SOCKET_MAX] = {INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET,INVALID_SOCKET};
CFW_TCPIP_SOCKET_ADDR rec_addr[REC_SOCKET_MAX] = {0, };
int rec_type[REC_SOCKET_MAX] = {0, };

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

int   plat_socket(int    domain, int type, int protocol)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000001);
	hal_HstSendEvent(0xffffffff);

	int ntype;
	int ndomain;
	int nprotocol;

	if (type == PLAT_SOCK_DGRAM)
	{
		ntype = CFW_TCPIP_SOCK_DGRAM;
	}
	else if (type == PLAT_SOCK_STREAM)
	{
		ntype = CFW_TCPIP_SOCK_STREAM;
	}
	else
	{
		plat_printf("plat_socket type error = %d\n", type);
		return -1;
	}
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000011);
	hal_HstSendEvent(0xffffffff);

	if (domain == PLAT_AF_INET)
	{
		ndomain = CFW_TCPIP_AF_INET;
	}
	else
	{
		plat_printf("plat_socket domain error = %d\n", domain);
		return -1;
	}
	if (protocol == PLAT_IPPROTO_UDP)
	{
		nprotocol = CFW_TCPIP_IPPROTO_UDP;
	}
	else if (protocol == PLAT_IPPROTO_IP)
	{
		nprotocol = CFW_TCPIP_IPPROTO_IP;
	}
	else if (protocol == PLAT_IPPROTO_TCP)
	{
		nprotocol = CFW_TCPIP_IPPROTO_TCP;
	}
	else
	{
		plat_printf("plat_socket protocol error = %d\n", protocol);
		return -1;
	}

	SOCKET ret = CFW_TcpipSocket(ndomain, ntype, nprotocol);

	plat_printf("creat socket = %d\n", ret);

	int i;
	for (i = 0; i < REC_SOCKET_MAX; i++)
	{
		if(rec_socket[i] == INVALID_SOCKET)
		{
			rec_socket[i] = ret;
			rec_type[i] = type;
			plat_printf(">>>>>>>>>>rec socket save i:%d, socket:%d<<<<<<<<<<\n", i, rec_socket[i]);
			break;
		}
	}

	return (int)ret;
}

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
int   plat_socket_close(int      sock)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000002);
	hal_HstSendEvent(0xffffffff);

	int i;
	for (i = 0; i < REC_SOCKET_MAX; i++)
	{
		if(rec_socket[i] == sock)
		{
			plat_printf(">>>>>>>>>>rec socket delete i:%d, socket:%d<<<<<<<<<<\n", i, rec_socket[i]);
			rec_socket[i] = INVALID_SOCKET;
			if (rec_buff[i] != NULL)
			{
				plat_free(rec_buff[i]);
				rec_buff[i] == NULL;
			}
			break;
		}
	}

	return CFW_TcpipSocketClose(sock);
}

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
int   plat_bind(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len)
{
	CFW_TCPIP_SOCKET_ADDR *bind_addr;
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000003);
	hal_HstSendEvent(0xffffffff);

	if (addr->net_family != PLAT_AF_INET)
	{
		return -1;
	}
	bind_addr = plat_malloc(sizeof(CFW_TCPIP_SOCKET_ADDR));
	memset(bind_addr, 0, sizeof(CFW_TCPIP_SOCKET_ADDR));

	struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)addr;
	bind_addr->sin_family = CFW_TCPIP_AF_INET;
	bind_addr->sin_port = addrv4->sin_port;
	bind_addr->sin_addr.s_addr = addrv4->sin_addr.s_addr;
	bind_addr->sin_len = 0;
	plat_printf("bind_addr sin_port = %d \n", bind_addr->sin_port);
	plat_printf("bind_addr sin_addr s_addr = %d \n", bind_addr->sin_addr.s_addr);

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000013);
	hal_HstSendEvent(0xffffffff);

	int ret = CFW_TcpipSocketBind(sock, bind_addr, addr_len);
	plat_free(bind_addr);

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000014);
	hal_HstSendEvent(ret);
	hal_HstSendEvent(0xffffffff);

	
	return ret;
}

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
int   plat_connect(int    sock, const struct  plat_sockaddr_storage *addr, int addr_len)
{
	CFW_TCPIP_SOCKET_ADDR *connect_addr;
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000004);
	hal_HstSendEvent(0xffffffff);

	if (addr->net_family != PLAT_AF_INET)
	{
		return -1;
	}

	connect_addr = plat_malloc(sizeof(CFW_TCPIP_SOCKET_ADDR));
	memset(connect_addr, 0, sizeof(CFW_TCPIP_SOCKET_ADDR));

	struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)addr;
	connect_addr->sin_family = CFW_TCPIP_AF_INET;
	connect_addr->sin_port = addrv4->sin_port;
	connect_addr->sin_addr.s_addr = addrv4->sin_addr.s_addr;
	connect_addr->sin_len = 4;

	int ret = CFW_TcpipSocketConnect(sock, connect_addr, addr_len);
	plat_free(connect_addr);

	return ret;
}

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
SIZE_T   plat_sendto(int    sock,const void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *destaddr, int addr_len)
{
	CFW_TCPIP_SOCKET_ADDR *send_addr;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000005);
	hal_HstSendEvent(0xffffffff);

	if (destaddr->net_family != PLAT_AF_INET)
	{
		return -1;
	}

	send_addr = plat_malloc(sizeof(CFW_TCPIP_SOCKET_ADDR));
	memset(send_addr, 0, sizeof(CFW_TCPIP_SOCKET_ADDR));

	struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)destaddr;
	send_addr->sin_family = CFW_TCPIP_AF_INET;
	send_addr->sin_port = addrv4->sin_port;
	send_addr->sin_addr.s_addr = addrv4->sin_addr.s_addr;
	send_addr->sin_len = 4;

	SIZE_T ret = CFW_TcpipSocketSendto(sock, buf, buflen, flags, send_addr, addr_len);
	plat_free(send_addr);
	
	return ret;
}

BOOL plat_check(CFW_EVENT* CfwEv)
{
	int i;
	SOCKET socket_check = INVALID_SOCKET;

	for (i = 0; i < REC_SOCKET_MAX; i++)
	{
		if(rec_socket[i] == (SOCKET)(CfwEv->nParam1))
		{
			socket_check = (SOCKET)(CfwEv->nParam1);
			break;
		}
	}

	plat_printf(">>>>>>>>>>rec socket:%d, socket list:%d,socket get:%d,id:%d<<<<<<<<<<\n", (SOCKET)(CfwEv->nParam1),i,socket_check,CfwEv->nEventId);


	if((socket_check != INVALID_SOCKET) && (EV_CFW_TCPIP_REV_DATA_IND == CfwEv->nEventId))
	{
		INT32 iResult      = 0;
		UINT8 * rec;

		rec = plat_malloc(REC_BUFFER_LEN);
		memset(rec, 0, REC_BUFFER_LEN);

		rec_addr[i].sin_len = sizeof(CFW_TCPIP_SOCKET_ADDR);

		if (rec_type[i] == PLAT_SOCK_DGRAM)
		{
			iResult = CFW_TcpipSocketRecvfrom(socket_check, rec, REC_BUFFER_LEN, 0, &rec_addr[i], (INT32 *) & (rec_addr[i].sin_len));
			plat_printf(">>>>>>find ip:%d, find port:%d, find len:%d<<<<<<\n", rec_addr[i].sin_addr.s_addr, rec_addr[i].sin_port, rec_addr[i].sin_len);
		}
		else
		{
			iResult = CFW_TcpipSocketRecv(socket_check, rec, REC_BUFFER_LEN, 0);
		}
		if (SOCKET_ERROR == iResult)
		{
			AT_TCPIP_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, CfwEv->nUTI);
		}
		else
		{
			if (rec_buff[i] == NULL)
			{
				rec_buff[i] = plat_malloc(REC_BUFFER_LEN);
			}
			memset(rec_buff[i], 0, REC_BUFFER_LEN);
			memcpy(rec_buff[i], rec, REC_BUFFER_LEN);
			rec_size[i] = iResult;
			AT_TCPIP_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, rec, iResult, CfwEv->nUTI);
		}
		plat_free(rec);
		return TRUE;
	}
	return FALSE;
}

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
SIZE_T   plat_recvfrom(int    sock, void *buf , SIZE_T buflen, int flags,const struct  plat_sockaddr_storage *srcaddr, int *addr_len)
{
#if 0
	CFW_TCPIP_SOCKET_ADDR *connect_addr;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000006);
	hal_HstSendEvent(0xffffffff);

	if (srcaddr->net_family != PLAT_AF_INET)
	{
		return -1;
	}

	connect_addr = plat_malloc(sizeof(CFW_TCPIP_SOCKET_ADDR));
	memset(connect_addr, 0, sizeof(CFW_TCPIP_SOCKET_ADDR));

	struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)srcaddr;
	connect_addr->sin_family = CFW_TCPIP_AF_INET;
	connect_addr->sin_port = addrv4->sin_port;
	connect_addr->sin_addr.s_addr = addrv4->sin_addr.s_addr;
	connect_addr->sin_len = 4;

	SIZE_T ret = CFW_TcpipSocketRecvfrom(sock, buf, buflen, flags, connect_addr, addr_len);
	plat_free(connect_addr);
	plat_printf("recv ret = %d\n", ret);
	//plat_printf("recv buf = %s\n", buf);
	plat_printf("recv socket = %d\n", sock);
	
	return ret;
#endif
	SIZE_T ret = 0;

	int i;
	for (i = 0; i < REC_SOCKET_MAX; i++)
	{
		if(rec_socket[i] == sock)
		{
			break;
		}
	}
	if (i >= REC_SOCKET_MAX)
	{
		return -1;
	}
	while(1)
	{
		if (-1 == CFW_TcpipGetRecvAvailable(rec_socket[i]))
		{
			return -1;
		}		
		if(rec_buff[i] == NULL)
		{
			continue;
		}
		//plat_printf("plat_recvfrom size:%d\n", rec_size[i]);
		if (rec_size[i] > 0)
		{
			struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)srcaddr;
			if (addrv4 != NULL)
			{
				addrv4->sin_family = PLAT_AF_INET;
				addrv4->sin_port = rec_addr[i].sin_port;
				addrv4->sin_addr.s_addr = rec_addr[i].sin_addr.s_addr;
			}

			if (addr_len != NULL)
			{
				*addr_len = 128;
			}

			if(rec_size[i] > buflen)
			{
				return -1;
			}
			ret = rec_size[i];
			
			memset(buf, 0, buflen);
			memcpy(buf, rec_buff[i], rec_size[i]);
			memset(rec_buff[i], 0, REC_BUFFER_LEN);
			rec_size[i] = 0;
			return ret;
		}
	}
	return -1;
}

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
uint16_t   plat_htons(uint16_t num)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000007);
	hal_HstSendEvent(0xffffffff);

	return htons(num);
}

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
uint16_t   plat_ntohs(uint16_t num)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000008);
	hal_HstSendEvent(0xffffffff);

	return ntohs(num);
}

/*!\brief 平台 接口 转换IP地址为网络序
	* 
	* 平台统一标准输出接口，各个平台内部各自实现
	* @param[in]   af: 地址簇类型 V4 or V6
	* @param[in]   src: 原IP 字节序
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
int    plat_pton(FAMILLY_T af,const char* src,void *dst)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000009);
	hal_HstSendEvent(0xffffffff);

    if(af == PLAT_AF_INET) {
        in_addr in_val;
		plat_printf("plat_pton ip: %s\n", src);
        if (inet_aton(src, &in_val)) {
			hal_HstSendEvent(0xffffffff);
			hal_HstSendEvent(0x10000019);
			hal_HstSendEvent(0xffffffff);
			struct plat_in4_addr *addrv4 = (struct plat_in4_addr *)dst;
			addrv4->s_addr = in_val.s_addr;

            return (0);
        }
    }
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x10000029);
	hal_HstSendEvent(0xffffffff);

    return (-1);
}

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
const char  *plat_ntop(FAMILLY_T af,const char* src,void *dst,int addrlen)
{
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x1000000A);
	hal_HstSendEvent(0xffffffff);

    const UINT8 *p = (const UINT8 *) src;
    if(af == PLAT_AF_INET) {
        char temp[PLAT_INET_ADDRSTRLEN];
        snprintf(temp, sizeof(temp), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
        if (strlen(temp) >= addrlen){
            return (NULL);
        }
        strcpy(dst, temp);
        return (dst);
    }
    return (NULL);
}

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
int    plat_get_hostIP(char *ip)
{
	UINT8 uiLen 					  = 0;
	UINT8 acIPAddr[18]				  = { 0, };

	UINT8 cid = 1;
	bool cid_flag = FALSE;
	UINT32 iResult = 0;
	UINT8 actState = 0;
	
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x1000000B);
	hal_HstSendEvent(0xffffffff);

	for(cid = 1; cid <= 7; cid++)
	{
		iResult = CFW_GetGprsActState(cid, &actState, 0);
		if (CFW_GPRS_ACTIVED == actState && ERR_SUCCESS == iResult)
		{
			cid_flag = TRUE;
			break;
		}
	}

	if (cid_flag == TRUE) 
	{
		hal_HstSendEvent(0xffffffff);
		hal_HstSendEvent(0x11111111);
		hal_HstSendEvent(cid);
		hal_HstSendEvent(0xffffffff);
		UINT32 ret = CFW_GprsGetPdpAddr(cid, &uiLen, acIPAddr, 0);

		if (ret == 0)
		{
			char ip_temp[18] = {0,};
			plat_ntop(PLAT_AF_INET, acIPAddr, ip_temp, 18);

			memcpy(ip, ip_temp, strlen(ip_temp));
			AT_WriteUart("host ip:\n", strlen("host ip:\n"));
			AT_WriteUart(ip, strlen(ip));
			AT_WriteUart("\n", strlen("\n"));
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

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
int    plat_get_IpByName(const char *host,char *ip,const char* port)
{
	UINT32 ulRet = 0;
	UINT32 ulIpAddr = 0;
	//UINT8 nSim = 0;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x1000000C);
	hal_HstSendEvent(0xffffffff);

	struct ip_addr nIpAddr;
	nIpAddr.addr = ulIpAddr;

	UINT8 cid = 1;
	UINT32 iResult = 0;
	UINT8 actState = 0;
	bool cid_flag = FALSE;

	for(cid = 1; cid <= 7; cid++)
	{
		iResult = CFW_GetGprsActState(cid, &actState, 0);
		if (CFW_GPRS_ACTIVED == actState && ERR_SUCCESS == iResult)
		{
			cid_flag = TRUE;
			break;
		}
	}

	//nSim = AT_SIM_ID(pParam->nDLCI);

	//AT_Gprs_CidInfo  *nStaAtGprsCidInfo = g_staAtGprsCidInfo_e[nSim];

	plat_printf("host: %s,cid:%d\n", host, cid);
	if (cid_flag == TRUE)
	{
		int time = 0;
		do{
			if (time > 10) 
			{
				hal_HstSendEvent(0xffffffff);
				hal_HstSendEvent(0x1000001C);
				hal_HstSendEvent(0xffffffff);
				return (-1);
			}
			plat_sleep(3);
			time++;
			ulRet = CFW_Gethostbyname(host, &nIpAddr, cid, 0);//to do
		} while(ulRet == DNS_QUERY_QUEUED);

		if(ulRet == DNS_QUERY_INVALID)
		{
			plat_printf("plat_get_IpByName error\n");
			return (-1);
		}

		if(ulRet == DNS_QUERY_COMPLETE)
		{
			in_addr nInAddr;
			nInAddr.s_addr = nIpAddr.addr;
			char *temp = CFW_TcpipInet_ntoa(nInAddr);
			memcpy(ip, temp, strlen(temp));
			plat_printf("change host to ip:%s\n", ip);
			
			//char ip_temp[18] = {0,};
			//plat_ntop(PLAT_AF_INET, nIpAddr, ip_temp, 18);
			//plat_printf("change host to ip2:%s\n", ip_temp);
			
			hal_HstSendEvent(0xffffffff);
			hal_HstSendEvent(0x1000002C);
			hal_HstSendEvent(0xffffffff);
			return (0);
		}
	}
	
	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x1000003C);
	hal_HstSendEvent(0xffffffff);

	return (-1);
}

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
int    plat_socket_read(int sock, void *buf, SIZE_T nbyte)
{
	return plat_recvfrom(sock, buf, nbyte, 0, NULL, NULL);
}

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

int plat_sendmsg(int sockfd, struct plat_msghdr *msg, int flags)
{
	int i;
	int send_count = 0;

	for (i = 0; i < msg->msg_iovlen; i++)
	{
		plat_printf("plat_sendmsg need send length:%d\n", msg->msg_iov[i].iov_len);
		send_count += CFW_TcpipSocketSend(sockfd, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len, flags);
		plat_printf("plat_sendmsg send length:%d\n", send_count);
	}
	plat_printf("plat_sendmsg send total length:%d\n", send_count);

	return send_count;
}

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

int plat_send(int sockfd, const void *buf, SIZE_T len, int flags)
{
	int ret;

	ret = CFW_TcpipSocketSend(sockfd, buf, len, flags);

	return ret;
}

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

int plat_recv(int sockfd, const void *buf, SIZE_T len, int flags)
{
	return plat_recvfrom(sockfd, buf, len, flags, NULL, NULL);
}