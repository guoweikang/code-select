/** 
	* @file 		lwm2msdk.c
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


#include "liblwm2m.h"
#include "connection.h"

#include "plat_string.h"
#include "plat_stdlib.h"
#include "plat_pthread.h"

#include "j_aes.h"
#include "j_base64.h"

#include "lwm2msdk.h"
#include "lwm2mclient.h"

#ifdef LINUX_PLAT 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#endif 

#define MAX_PACKET_SIZE 1024
#define OBJ_COUNT 3


int g_reboot = 0;
static int g_quit = -1;//

PLAT_PTHREAD_T 	g_lwsdktid;
static  int	s_thrd_flag = 0;

#ifdef HAVE_PIPE
int 		g_pip_ios_fd[2] ;
#else
int  g_stop_flag = 0;
#endif 


char g_szEPI[256]="";
Options  g_opt = {"CMEI_IMEI","CMEI_IMEI2","IMSI", "v1.0","appkey123456","",4,"127.0.0.1",5683,300,56380,0};
OptFuncs g_sdk_opt_funcs = {NULL};



typedef struct
{
    lwm2m_object_t * securityObjP;
    int sock;
    connection_t * connList;
    int addressFamily;
} client_data_t;


//modify by guoweikang don't use linux and otheres 
/*
void handle_sigint(int signum)
{
    g_quit = 1;
}
*/

int isbigendian()
{
    union unpara
    {
            char sz[2];
            short si;
    };
    union unpara ua;
    ua.si = 1;
    if(ua.sz[1]==1)
       return 1;
    else
       return 0;
}


void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

    LOG_M("Connecting to %s\r\n", uri);

    // parse uri in the form "coaps://[host]:[port]"
    if (0 == strncmp(uri, "coaps://", strlen("coaps://")))
    {
        host = uri+strlen("coaps://");
    }
    else if (0 == strncmp(uri, "coap://", strlen("coap://")))
    {
        host = uri+strlen("coap://");
    }
    else
    {
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) goto exit;
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else goto exit;
    }
    // split strings
    *port = 0;
    port++;

    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        LOG_S( "Connection creation failed.\r\n");
    }
    else {
        dataP->connList = newConnP;
    }

exit:
    lwm2m_free(uri);
    return (void *)newConnP;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    connection_t * targetP;

    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
        connection_t * parentP;

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}


void print_state(lwm2m_context_t * lwm2mH)
{
    lwm2m_server_t * targetP;

    LOG_S( "State: ");
    switch(lwm2mH->state)
    {
    case STATE_INITIAL:
        plat_fprintf(PLAT_STDERR, "STATE_INITIAL");
        break;
    case STATE_BOOTSTRAP_REQUIRED:
        plat_fprintf(PLAT_STDERR, "STATE_BOOTSTRAP_REQUIRED");
        break;
    case STATE_BOOTSTRAPPING:
        plat_fprintf(PLAT_STDERR, "STATE_BOOTSTRAPPING");
        break;
    case STATE_REGISTER_REQUIRED:
        plat_fprintf(PLAT_STDERR, "STATE_REGISTER_REQUIRED");
        break;
    case STATE_REGISTERING:
        plat_fprintf(PLAT_STDERR, "STATE_REGISTERING");
        break;
    case STATE_READY:
        plat_fprintf(PLAT_STDERR, "STATE_READY");
        break;
    default:
        plat_fprintf(PLAT_STDERR, "Unknown !");
        break;
    }
    plat_fprintf(PLAT_STDERR, "\r\n");

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        LOG_S( "No Bootstrap Server.\r\n");
    }
    else
    {
        LOG_S("Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            plat_fprintf(PLAT_STDERR, " - Security Object ID %d", targetP->secObjInstID);
            plat_fprintf(PLAT_STDERR, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            plat_fprintf(PLAT_STDERR, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                plat_fprintf(PLAT_STDERR, "DEREGISTERED\r\n");
                break;
            case STATE_BS_HOLD_OFF:
                plat_fprintf(PLAT_STDERR, "CLIENT HOLD OFF\r\n");
                break;
            case STATE_BS_INITIATED:
               plat_fprintf(PLAT_STDERR, "BOOTSTRAP INITIATED\r\n");
                break;
            case STATE_BS_PENDING:
               plat_fprintf(PLAT_STDERR, "BOOTSTRAP PENDING\r\n");
                break;
            case STATE_BS_FINISHED:
                plat_fprintf(PLAT_STDERR, "BOOTSTRAP FINISHED\r\n");
                break;
            case STATE_BS_FAILED:
                plat_fprintf(PLAT_STDERR, "BOOTSTRAP FAILED\r\n");
                break;
            default:
                plat_fprintf(PLAT_STDERR, "INVALID (%d)\r\n", (int)targetP->status);
            }
            plat_fprintf(PLAT_STDERR, "\r\n");
        }
    }

    if (lwm2mH->serverList == NULL)
    {
        LOG_S( "No LWM2M Server.\r\n");
    }
    else
    {
        LOG_S( "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            plat_fprintf(PLAT_STDERR, " - Server ID %d", targetP->shortID);
            plat_fprintf(PLAT_STDERR, "\tstatus: ");
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                plat_fprintf(PLAT_STDERR, "DEREGISTERED\r\n");
                break;
            case STATE_REG_PENDING:
                plat_fprintf(PLAT_STDERR, "REGISTRATION PENDING\r\n");
                break;
            case STATE_REGISTERED:
                plat_fprintf(PLAT_STDERR, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
                break;
            case STATE_REG_UPDATE_PENDING:
                plat_fprintf(PLAT_STDERR, "REGISTRATION UPDATE PENDING\r\n");
                break;
            case STATE_REG_UPDATE_NEEDED:
                plat_fprintf(PLAT_STDERR, "REGISTRATION UPDATE REQUIRED\r\n");
                break;
            case STATE_DEREG_PENDING:
                plat_fprintf(PLAT_STDERR, "DEREGISTRATION PENDING\r\n");
                break;
            case STATE_REG_FAILED:
                plat_fprintf(PLAT_STDERR, "REGISTRATION FAILED\r\n");
                break;
            default:
                plat_fprintf(PLAT_STDERR, "INVALID (%d)\r\n", (int)targetP->status);
            }
            plat_fprintf(PLAT_STDERR, "\r\n");
        }
    }
}


void lwm2m_sdk_notify(SDK_NOTIFY_TYPE type,SDK_NOTIFY_CODE code,int ret,const char *notify_msg)
{
    if(g_sdk_opt_funcs.NotifyMsg==NULL)
    return ;
    OptNotifyParam optNotifyParam;
    optNotifyParam.notify_type = type;
    optNotifyParam.notify_code = code;
    if(code == NOTIFY_CODE_UNKNOWEN)
    {
        if(ret==0)
        optNotifyParam.notify_code=NOTIFY_CODE_OK;
        else if(ret<0)
        optNotifyParam.notify_code=NOTIFY_CODE_FAILED;
    }
    
    char szmsg[1024];
    memset(szmsg,0,sizeof(szmsg));
    plat_sprintf(szmsg,"{ret=%0x,msg=%s}",ret,notify_msg);
    optNotifyParam.notify_msg = szmsg;
    g_sdk_opt_funcs.NotifyMsg(&optNotifyParam);
}

void lwm2m_sdk_notify_sys(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,notify_msg);
}

void lwm2m_sdk_notify_reg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_reg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_FAILED,ret,notify_msg);
}


int to_encode(char *szin,char *szout)
{
    char *name = szin;
    //AES CBC
	/* A 256 bit key */  
	char *passwd = "00000000000000000000000000000000";
	if(strlen(g_opt.szPwd)>0)
	{
		passwd = g_opt.szPwd;
	}
	else
	{
		LOG_M("pwd is null,use default pwd is:%s~\n", passwd);
	}
	//
	unsigned char key[64]={0};
	memset(key,0,sizeof(key));
	char shaout[256];
	memset(shaout,0,sizeof(shaout));
	StrSHA256(passwd,strlen(passwd), shaout);
	//printf("pwd=%s\n",passwd);
	//printf("shaout=%s\n",shaout);
	HexStrToByte(shaout,strlen(shaout),key);
	//hex_print("16sha:",key,32);
	
	/* A 128 bit IV */  
	unsigned char iv[16]={0};
	memset(iv,0,16);
	int i=0;
	for (i=0; i<16; ++i) {  
        iv[i] = 0;  
    } 
	unsigned char ciphertext[264]={0};   
	/* Buffer for the decrypted text */  
	//unsigned char decryptedtext[264]={0}; 

	//int decryptedtext_len=0,
	int ciphertext_len=0;
	char *plaintext =  "The quick brown fox jumps over the lazy dog1234";
	plaintext = name;
	//printf("Plaintext is:\n%s~\n", plaintext);

	ciphertext_len=EncryptionAES( plaintext, strlen(plaintext), key,ciphertext);	
 
	//printf("Ciphertext is %d bytes long:\n", ciphertext_len);
	//hex_print("Ciphertext:",ciphertext,ciphertext_len);
	
	//name = ciphertext;
	
	//base64
	unsigned char *testbase64=(unsigned char *)"123456789";//MTIzNDU2Nzg5
	testbase64=ciphertext;
	
	unsigned char *encData=0;
	//unsigned char *decData=0;
	unsigned int encDataLen=0;
	//unsigned int decDataLen=0;
	//printf("base64 inputlen=%d,strlen=%d\n",ciphertext_len,strlen(testbase64) );
	int ret= j_base64_encode(testbase64, ciphertext_len, &encData, &encDataLen);
	if(ret<0)
	{
	    LOG_M("ret=%d~\n", ret);
	}
	//printf("base64 encData=%s~\n", encData);
	memcpy(szout,encData,encDataLen);
	
    j_base64_free(encData, encDataLen);
	
    return encDataLen;
	

}


int LWM2M_SDK_RUN_PARAM(int argc, char *argv[])
{
    client_data_t data;
    lwm2m_context_t * lwm2mH = NULL;
    lwm2m_object_t * objArray[OBJ_COUNT];
	char * server = "127.0.0.1";
	char * serverPort = "5683";
    char * localPort = "56830";
	//char * initname = "testlwm2mclient";
	char * name = "";
	int lifetime=300;
    int result;
    //int opt;

    memset(&data, 0, sizeof(client_data_t));

    data.addressFamily = PLAT_AF_INET;
	//localPort = argv[opt];
	//param set by optional	
	if (strlen(g_opt.szSrvIP)>=0) server=g_opt.szSrvIP;
	
	char szsrvport[10]={0};
	if (g_opt.nSrvPort > 80 ) 
	{
		plat_sprintf(szsrvport,"%d",g_opt.nSrvPort);
		serverPort = szsrvport;		
	}
	
	if(g_opt.nAddressFamily==4) data.addressFamily =PLAT_AF_INET;
	
	if ( g_opt.nLifetime > 10 ) lifetime=g_opt.nLifetime ;

    char szlocalport[10]={0};
	if (g_opt.nLocalPort > 49152 ) 
	{
		plat_sprintf(szlocalport,"%d",g_opt.nLocalPort);
		localPort = szlocalport;		
	}
	
	char *szStars="****";
	char szEpname[256]={0};
	plat_sprintf(szEpname,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,	
	strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars
	);

    char szEPIsrc[256]={0};
    plat_sprintf(szEPIsrc,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,
	szStars
	);

    char szEpnameCode[256]={0};    
    memset(szEpnameCode,0,sizeof(szEpnameCode));
    memset(g_szEPI,0,sizeof(g_szEPI));
    name=szEpname;


    int nEpnamecodeLen = 0;
    nEpnamecodeLen = to_encode(szEpname,szEpnameCode);	
	char epnametmp[256];
	memset(epnametmp,0,sizeof(epnametmp));
	plat_sprintf(epnametmp,"L@#@%s@#@%s",szEpnameCode,g_opt.szAppKey);			//TODO

    //strcpy(epnametmp,"L@#@M+D2vyO9kk/IPVyzehd6Biypy8R69yTlHdaIQiTp6klbXqJmESZJ6QXoDlQkqOC8@#@M100000006");
	LOG_M("epname=%s~\n", epnametmp);
	name = epnametmp;

		
    char epitmp[256];
	memset(epitmp,0,sizeof(epitmp));
    to_encode(szEPIsrc,epitmp); 
	plat_sprintf(g_szEPI,"L@#@%s@#@%s",epitmp,g_opt.szAppKey);			//TODO
   
	
    LOG_M("szEPI=%s~\n", g_szEPI);  

///////////////

    /*
     *This call an internal function that create an IPv6 socket on the port 5683.
     */
    LOG_M( "Trying to bind LWM2M Client to port %s\r\n", localPort);
	
    data.sock = create_socket(localPort, data.addressFamily);
    if (data.sock < 0)
    {
        LOG_S("Failed to open socket ");
        return -1;
    }
	
	char serverUri[50];
    int serverId = 123;
	plat_sprintf (serverUri, "coap://%s:%s", server, serverPort);

	LOG_M( "server uri = %s ", serverUri);

    
	objArray[0] = get_security_object(serverId, serverUri);
    if (NULL == objArray[0])
    {
        LOG_S( "Failed to create security object\r\n");
        return -1;
    }
    data.securityObjP = objArray[0];

    objArray[1] = get_server_object(serverId, lifetime);
    if (NULL == objArray[1])
    {
        LOG_S( "Failed to create server object\r\n");
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        LOG_S("Failed to create Device object\r\n");
        return -1;
    }


	LOG_M( "lwm2m_init %s\n ","start" );

	
    lwm2mH = lwm2m_init(&data);
    if (NULL == lwm2mH)
    {
        LOG_S( "lwm2m_init() failed\r\n");
        return -1;
    }

   
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    if (result != 0)
    {
        LOG_M( "lwm2m_configure() failed: 0x%X\r\n", result);
        return -1;
    }


    //signal(SIGINT, handle_sigint);
    LOG_M("LWM2M Client \"%s\" started on port %s.\r\nUse Ctrl-C to exit.\r\n\n", name, localPort);
    g_quit = 0;
    
    /*
     * We now enter in a while loop that will handle the communications from the server
     */
    while (0 == g_quit)
    {
#if     defined(HAVE_SELECT) && defined(HAVE_PIPE) 
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
	    FD_SET(g_pip_ios_fd[0],&readfds);
#endif 
		struct plat_time_value tv;
		tv.sec = 60;

        print_state(lwm2mH);
        
        result = lwm2m_step(lwm2mH, &(tv.sec));
        if (result != 0)
        {
            LOG_M( "lwm2m_step() failed: 0x%X\r\n", result);
            g_quit = 1;
            break;
            //continue;
            return -1;
        }

        /*
         * This part wait for an event on the socket until "tv" timed out (set
         * with the precedent function)
         */
#if     defined(HAVE_SELECT) && defined(HAVE_PIPE) 

        result = select(FD_SETSIZE, &readfds, NULL, NULL, (struct timeval*)&tv);
        if (result < 0)
        {
            if (errno != EINTR)
            {
               LOG_M("Error in select(): %d %s\r\n", errno, strerror(errno));
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
            if (FD_ISSET(data.sock, &readfds))
            {
                struct plat_sockaddr_storage  addr;
				memset(&addr,0,sizeof(struct plat_sockaddr_storage));
                int addrLen;
                addrLen = sizeof(addr);

                /*
                 * We retrieve the data received
                 */
                numBytes = plat_recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct plat_sockaddr_storage *)&addr, &addrLen);

                if (0 > numBytes)
                {
                    LOG_M("Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
                }
                else if (0 < numBytes)
                {
                    connection_t * connP;

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                    }
                    else
                    {
                        /*
                         * This packet comes from an unknown peer
                         */
                        LOG_S( "received bytes ignored!\r\n");
                    }
                }
            }
			else if(FD_ISSET(g_pip_ios_fd[0], &readfds))
			{
				char c[2];  
				read(g_pip_ios_fd[0], c, 1);
				g_quit = 1;
			}
        }
#else
		if  ( 1 == g_stop_flag  )
		{
			g_quit =1;
			
		}
		else
		{
			uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;
			struct plat_sockaddr_storage  addr;
			memset(&addr,0,sizeof(struct plat_sockaddr_storage));
            int addrLen;
			numBytes = plat_recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct plat_sockaddr_storage *)&addr, &addrLen);
			if (0 > numBytes)
			{
				LOG_S("Error in recvfrom()\r\n");
			}
			else if (0 < numBytes)
			{
					 connection_t * connP;
					 connP = connection_find(data.connList, &addr, addrLen);
					 if (connP != NULL)
					 {
					 /*
					 * Let liblwm2m respond to the query depending on the context
					*/
						lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
					 } 
					 else
					{
					/*
					* This packet comes from an unknown peer
					*/
					LOG_S( "received bytes ignored!\r\n");	
					}
			}
		}
		plat_sleep(1);
#endif 
    }

    /*
     * Finally when the loop is left, we unregister our client from it
     */
    lwm2m_close(lwm2mH);
    lwm2m_sdk_notify_dereg_success(0,"dereg");
    plat_socket_close(data.sock);
    connection_free(data.connList);

    free_security_object(objArray[0]);
    free_server_object(objArray[1]);
    free_object_device(objArray[2]);

    LOG_S( "loop func quit\r\n\n");

    return 0;
}

void * runthreadfunc(void * arg)
{

	int ret = LWM2M_SDK_RUN_PARAM(0,NULL);	
	lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
	LOG_S("runthreadfunc end");
	s_thrd_flag = 0;
	return 0;
}

int LWM2M_SDK_RUN(int nMode)
{
	//LWM2M_SDK_RUN(0,NULL);
	int ret = 0;
	if(g_quit==0)
	{
	    LOG_S("has runned");
	    return 0;
	}
	
	if(nMode==0)
	{
		ret = LWM2M_SDK_RUN_PARAM(0,NULL);
		lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
		s_thrd_flag = 0;
	}
	else
	{
		if(s_thrd_flag==0)
		{
			if (0 != plat_pthread_create(&g_lwsdktid, NULL, runthreadfunc,NULL))
			{
				LOG_S("msg=failed to start up thread.|");
				s_thrd_flag = 0;
				
				ret = -1;
				return -1;
			}
			s_thrd_flag = 1;
			
		}
		else
		{
			LOG_S("msg=hss started up thread.|");
		}
			
		
		
	}
	
	//pthread_detach(g_lwsdktid); 	
	return ret;
	
}

int  LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs)
{	
	int ret = 0;
	strcpy(g_opt.szCMEI_IMEI,options->szCMEI_IMEI);
	strcpy(g_opt.szCMEI_IMEI2,options->szCMEI_IMEI2);
	strcpy(g_opt.szIMSI,options->szIMSI);	
	strcpy(g_opt.szDMv,options->szDMv);
	strcpy(g_opt.szAppKey,options->szAppKey);
	strcpy(g_opt.szPwd,options->szPwd);
	
	strcpy(g_opt.szSrvIP,options->szSrvIP);
	g_opt.nAddressFamily = options->nAddressFamily;
	g_opt.nSrvPort = options->nSrvPort;
	g_opt.nLifetime = options->nLifetime;
	g_opt.nBootstrap = options->nBootstrap;	
	g_opt.nLocalPort = options->nLocalPort;

    g_sdk_opt_funcs = *optfuncs;


#ifdef HAVE_PIPE 
	ret = pipe(g_pip_ios_fd);
#else
	g_stop_flag =0;
#endif 

#ifdef HAVE_SIGNAL 
	signal(SIGPIPE,SIG_IGN);
#endif 


	
	
	g_quit = -1;
	s_thrd_flag = 0;

	int nisbig = isbigendian();
	LOG_M("isbigendian=%d",nisbig);
	
	
	LOG_S("init sucess");
	return ret;
	
}



int LWM2M_SDK_STOP()
{	
    int ret = 0;

	if(g_quit != 0)
	{
		LOG_S("is stopped");
		return 0;
	}

	
#ifdef HAVE_PIPE 
	ret = write(g_pip_ios_fd[1], "x", 1);
#else
	g_stop_flag  =1 ;
#endif 

	//wait for thread quit	
	if(s_thrd_flag==1)
	{	
		plat_pthread_join(g_lwsdktid,NULL);
	}
	s_thrd_flag = 0;
	
	return ret;
	
}

int LWM2M_SDK_FINI()
{	
	
	g_quit = -1;
	s_thrd_flag = 0;

#ifdef HAVE_PIPE
	close(g_pip_ios_fd[0]);
	close(g_pip_ios_fd[1]);
#else
	g_stop_flag = 0;
#endif 


	LOG_S("end");
	return 0;
	
}


