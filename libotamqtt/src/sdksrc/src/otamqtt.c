




#include "plat_string.h"
#include "plat_stdlib.h"
#include "plat_stdio.h"
#include "plat_time.h"

#include "plat_net.h"

#include "mqtt.h"

#include "otamqtt.h"



/****************************
local global variable
****************************/

unsigned char g_ota_req_from_ont[57];
int g_pkt_id = 1;


/****************************
utils
****************************/

char *ota_strstr(const char *s1, const char *s2, int len)
{
	int len2;
	int i;
	if ((len2 = strlen(s2)) == 0)   //
		return NULL;

	for (i = 0; i < len; ++s1)
	{
		i++;
		if (*s1 == *s2 && strncmp(s1, s2, len2) == 0)
			return (char *)s1;
	}
	return NULL;
}

static void showInfo(struct OtaMqttContext *ctx)
{
	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);  
	plat_printf("{ \
	 	\"datastreams\": [ \
	 	{ \"id\":\"info\",\"datapoints\":[{\"value\": \"IMSI:%s ,IMEI:%s,EID:%s,ICCID:%s,LAC:%s,CELLID:%s\"}] }\
	 	] \
	 	}",pdev_info->IMSI,pdev_info->IMEI,pdev_info->EID,pdev_info->ICCID,pdev_info->LAC,pdev_info->CELLID);

}


/****************************
tcp 
****************************/

static int Ota_CreateTcpConnect(const char *ip, unsigned short port)
{

	struct plat_sockaddr_storage addr;
	int sock_fd;
	int sl = 0;
	struct plat_sockaddr_in4 *addrv4 = (struct plat_sockaddr_in4 *)&(addr);
	
	memset(&addr,0,sizeof(struct plat_sockaddr_storage));

	addrv4->sin_family = PLAT_AF_INET;
	addrv4->sin_port = plat_htons(port);
	if ( -1 == plat_pton(PLAT_AF_INET,ip,&addrv4->sin_addr)) 
	{
		   return -1;
	}
	sl = sizeof(struct plat_sockaddr_in4);

	sock_fd = plat_socket(PLAT_AF_INET, PLAT_SOCK_STREAM, PLAT_IPPROTO_IP);
	if (sock_fd < 0) {
		plat_printf("Ota_CreateTcpConnect  Failed to create socket file descriptor.\n");
		return -1;
	}
	if(-1 == plat_connect(sock_fd,  &addr, sl)) {
		plat_printf("Mqtt_CreateTcpConnect  Failed to connect to the server,\n");
		plat_socket_close(sock_fd);
		return -1;
	}

	return sock_fd;

}

static int Ota_TcpSend(int sock,const char* data, int len)
{
	int bytes = 0;
	bytes = plat_send(sock, data, len, 0);
	if(bytes<0)
	{
		return -1;
	}
	return bytes;
}

static int Ota_TcpRecv(int sock,const char* data, int len)
{
	int bytes = 0;
	bytes = plat_recv(sock, data, len, 0);
	if(bytes<0)
	{
		return -1;
	}
	return bytes;
}



// Function name:
//     Mqtt_Send_Action()
// Description:
//     Send the whole buffer content in Mqtt Context
// Parameter:
//     ctx: the Mqtt Context
// Return Value:
//     -1: failure
//      0: success
static int Ota_Mqtt_Send_Action(struct OtaMqttContext *ctx)
{
	int bytes;
	if (-1 != ctx->Sended_Bytes) {
		plat_printf("Ota_Mqtt_Send_Action  There are something to be send, please wait a moment to retry.\n");
		return 0;
	}

	bytes = Mqtt_SendPkt(ctx->Mqtt_Ctx, ctx->Mqtt_Buf, 0);
	if (bytes < 0) {
		plat_printf("Ota_Mqtt_Send_Action  Failed to send the packet to the server.\n");
		return -1;
	}else if (bytes != ctx->Mqtt_Buf->buffered_bytes) {
		ctx->Sended_Bytes = bytes;
		plat_printf("Mqtt_Send_Action  There are some data not sended(%d bytes).\n",ctx->Mqtt_Buf->buffered_bytes - bytes);
	}
	else {
		MqttBuffer_Reset(ctx->Mqtt_Buf);
		return 0;
	}

	while (1) {

			bytes = Mqtt_SendPkt(ctx->Mqtt_Ctx, ctx->Mqtt_Buf, ctx->Sended_Bytes);
			if (bytes < 0) {
				plat_printf("Mqtt_Send_Action  Failed to send the packet to the server.\n");
				return -1;
			}
			else {
				ctx->Sended_Bytes += bytes;
				if (ctx->Sended_Bytes == ctx->Mqtt_Buf->buffered_bytes) {
					MqttBuffer_Reset(ctx->Mqtt_Buf);
					ctx->Sended_Bytes = -1;
					break;
				}
			}
		}
	return 0;
}

// Function name:
//     ota_Mqtt_Recv_Action()
// Description:
//     Receive response into buffer in Mqtt Context
// Parameter:
//     ctx: the Mqtt Context
// Return Value:
//     -1: failure
//      0: success
int Ota_Mqtt_Recv_Action(struct OtaMqttContext *ctx)
{

	while (1) {
			int err;
			err = Mqtt_RecvPkt(ctx->Mqtt_Ctx);
			if(MQTTERR_ENDOFFILE == err) {
				plat_printf("Ota_Mqtt_Recv_Action  The connection is disconnected.\n");
				plat_socket_close(ctx->Mqtt_Fd);
				ctx->Mqtt_Fd = -1;
				return -1;
			}

			if(MQTTERR_IO == err) {
				plat_printf("Ota_Mqtt_Recv_Action  Send TCP data error\n");
				return -1;
			}

			if(MQTTERR_NOERROR != err) {
				plat_printf("Ota_Mqtt_Recv_Action   error is %d.\n", err);
				return -1;
			}else {
				plat_printf("Ota_Mqtt_Recv_Action Receive action finished.\n");
			break;
			}
	}
	return 0;
}






/****************************
apikey
****************************/

static int ota_http_register_device(struct OtaMqttContext *ctx)
{
	char requestbuf[REGISTER_BUFFER_LENTH];
	char recvbuf[REGISTER_BUFFER_LENTH];
	
	char urlbuf[128];
	char contextbuf[128];
	char *head = NULL;
	char *tail = NULL;
	int ret;
	int sock_fd;
	char *url = "http://api.heclouds.com/register_de?register_code=%s";
	char *imei = ctx->dev_info.IMEI;
	char *register_code = ctx->Register_Code;



	if (strlen(register_code)== 0)
	{
		plat_printf("ota_http_register_device register code not init ");
		return -1;
	}
	if (strlen(imei)== 0)
	{
		plat_printf("ota_http_register_device IMEI not init ");
		return -1;
	}
	
	sock_fd = Ota_CreateTcpConnect(HTTP_CREATE_DEVICE_IP,HTTP_CREATE_DEVICE_PORT);
	if (sock_fd < 0)
	{
		return -1;
	}

	
	plat_sprintf(contextbuf, HTTP_CONTEXT_SN, imei, imei);
	plat_sprintf(urlbuf, url, register_code);
	plat_sprintf(requestbuf, HTTP_POST, urlbuf, strlen(contextbuf), contextbuf);


	if(Ota_TcpSend(sock_fd,requestbuf, REGISTER_BUFFER_LENTH) == -1)
	{
		return -1;
	}
	
	ret = Ota_TcpRecv(sock_fd,recvbuf, REGISTER_BUFFER_LENTH);

	if (ret>0 && ret<=sizeof(recvbuf))
	{	

		plat_printf(recvbuf);

		head = ota_strstr(recvbuf, "key", ret);
		if (head == NULL)
			return -1;

		head += 6;
		tail = ota_strstr(head, "\"", sizeof(recvbuf));
		if (tail == NULL || tail - head <= 0)
			return -1;
		
		memmove(ctx->Api_Key, head, tail - head);
		

		head = ota_strstr(recvbuf, "device_id", sizeof(recvbuf));
		head += 12;
		tail = ota_strstr(head, "\"", sizeof(recvbuf));
		if (tail == NULL || tail - head <= 0)
			return -1;

		memmove(ctx->Device_ID, head, tail - head);

		memcpy(ctx->Device_Auth,imei,strlen(imei)+1);
		
		plat_printf("\nDeviceid:%s  \tApikey:%s  deviceAuth = %s \n", ctx->Device_ID, ctx->Api_Key,ctx->Device_Auth);
		
		plat_socket_close(sock_fd);
		
	
	}
	else
	{
		return -1;
	}
	return 0;
}






// Function name:
//     mqtt_recv_pkt()
// Description:
//     Lowest level read function
// Parameter:
//     arg: file descriptor
//     buf: receive buffer
//     count: the number of receive content
// Return Value:
//     The number of bytes actually read
static int mqtt_recv_pkt(void *arg, void *buf, uint32_t count)
{
	int bytes = plat_socket_read(*((int *)arg), buf, count);
	return bytes;
}

// Function name:
//     mqtt_send_pkt()
// Description:
//     Lowest level write function
// Parameter:
//     arg: file descriptor
//     iov: msg_iov
//     iovcnt: msg_iovlen
// Return Value:
//     The number of bytes actually write
static int mqtt_send_pkt(void *arg, const struct plat_iovec *iov, int iovcnt)
{
	int bytes;
	struct plat_msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = (struct plat_iovec*)iov;
	msg.msg_iovlen = (SIZE_T)iovcnt;

	int i=0,j=0;
	
	for(i=0; i<iovcnt; ++i){
		char *pkg = (char*)iov[i].iov_base;
		if(iov[i].iov_len > 1024){
			plat_printf("mqtt_send_pkt length:%d\n", (int)iov[i].iov_len);
		}else{
			plat_printf("mqtt_send_pkt iov %d :\n",i);
			for(j=0; j<iov[i].iov_len; ++j)
			plat_printf(" %02X ", pkg[j]&0xFF);
			plat_printf("\n");
		}
	}
	bytes = plat_sendmsg(*((int *)arg), &msg, 0);
	return bytes;
}



static int OtaMqtt_HandleConnAck(void *arg, char flags, char ret_code)
{
	plat_printf("OtaMqtt_HandleConnAck  Success to connect to the server, flags(%0x), code(%d).\n", flags, ret_code);
	return 0;
}




static int OtaMqtt_HandlePingResp(void *arg)
{
	plat_printf("OtaMqtt_HandlePingResp  Recv the ping response.\n");
	return 0;
}

static int OtaMqtt_HandlePublish(void *arg, 
		uint16_t pkt_id, 
		const char *topic, 
		const char *payload, 
		uint32_t payloadsize, 
		int dup, 
		enum MqttQosLevel qos)
{
	struct OtaMqttContext *ctx = (struct OtaMqttContext*)arg;
	ctx->pkt_to_ack = pkt_id;
	ctx->dup = dup;
	ctx->qos = qos;
	plat_printf("OtaMqtt_HandlePublish dup=%d, qos=%d, id=%d\ntopic: %s\npayloadsize=%d\n", dup, qos, pkt_id, topic, payloadsize);

	//get cmdid
	//$creq/topic_name/cmdid
	memset(ctx->cmdid, MAX_CMID_LENGTH, 0);
	if ('$' == topic[0] && 
			'c' == topic[1] && 
			'r' == topic[2] && 
			'e' == topic[3] && 
			'q' == topic[4] && 
			'/' == topic[5]) {
		int i=6;
		while(topic[i]!='/' && i<strlen(topic)) {
			++i;
		}
		if (i<strlen(topic)) {
			memcpy(ctx->cmdid, topic+i+1, strlen(topic+i+1));
		}
	}
	return 0;
}

static int OtaMqtt_HandlePubAck(void *arg, uint16_t pkt_id)
{
	plat_printf("OtaMqtt_HandlePubAck Recv the publish ack, packet id is %d.\n", pkt_id);
	return 0;
}

static int OtaMqtt_HandlePubRec(void *arg, uint16_t pkt_id)
{
	struct OtaMqttContext *ctx = (struct OtaMqttContext*)arg;
	ctx->pkt_to_ack = pkt_id;
	plat_printf("OtaMqtt_HandlePubRec Recv the publish rec, packet id is %d.\n", pkt_id);
	return 0;
}

static int OtaMqtt_HandlePubRel(void *arg, uint16_t pkt_id)
{
	struct OtaMqttContext *ctx = (struct OtaMqttContext*)arg;
	ctx->pkt_to_ack = pkt_id;
	plat_printf("OtaMqtt_HandlePubRel Recv the publish rel, packet id is %d.\n", pkt_id);
	return 0;
}

static int OtaMqtt_HandlePubComp(void *arg, uint16_t pkt_id)
{
	plat_printf("OtaMqtt_HandlePubComp  Recv the publish comp, packet id is %d.\n", pkt_id);
	return 0;
}

static int OtaMqtt_HandleSubAck(void *arg, uint16_t pkt_id, const char *codes, uint32_t count)
{
	uint32_t i;
	plat_printf("OtaMqtt_HandleSubAck  Recv the subscribe ack, packet id is %d, return code count is %d:.\n", pkt_id, count);
	for(i = 0; i < count; ++i) {
		unsigned int code = ((unsigned char*)codes)[i];
		plat_printf("OtaMqtt_HandleSubAck   code%d=%02x\n", i, code);
	}

	return 0;
}

static int OtaMqtt_HandleUnsubAck(void *arg, uint16_t pkt_id)
{
	plat_printf("OtaMqtt_HandleUnsubAck  Recv the unsubscribe ack, packet id is %d.\n", pkt_id);
	return 0;
}

static int OtaMqtt_HandleCmd(void *arg, 
		uint16_t pkt_id, 
		const char *cmdid, 
		int64_t timestamp, 
		const char *desc, 
		const char *cmdarg, 
		uint32_t cmdarg_len, 
		int dup, 
		enum MqttQosLevel qos)
{
	uint32_t i;
	int index = 0;
	int new_index = 0;
	struct OtaMqttContext *ctx = (struct OtaMqttContext*)arg;
	ctx->pkt_to_ack = pkt_id;
	memset(ctx->cmdid, MAX_CMID_LENGTH, 0);
	strcpy(ctx->cmdid, cmdid);
	plat_printf("OtaMqtt_HandleCmd Recv the command, packet id is %d, cmduuid is %s, qos=%d, dup=%d.\n", pkt_id, cmdid, qos, dup);

	if(0 != timestamp) {
		TIME_T seconds = timestamp / 1000;
		struct plat_tm *st = plat_localtime(&seconds);

		plat_printf("OtaMqtt_HandleCmd    The timestampe is %04d-%02d-%02dT%02d:%02d:%02d.%03d.\n",
				st->tm_year + 1900, st->tm_mon + 1, st->tm_mday,
				st->tm_hour, st->tm_min, st->tm_sec, (int)(timestamp % 1000));
	}
	else {
		plat_printf("OtaMqtt_HandleCmd    There is no timestamp.\n");
	}

	if(NULL != desc) {
		plat_printf("OtaMqtt_HandleCmd    The description is: %s.\n", desc);
	}
	else {
		plat_printf("OtaMqtt_HandleCmd    There is no description.\n");
	}

	plat_printf("OtaMqtt_HandleCmd    The length of the command argument is %d, the argument is:%s \n", cmdarg_len,cmdarg);

	unsigned char temp[113];
	for(i = 0; i < cmdarg_len; ++i) {
		temp[i] = cmdarg[i];
		const char c = cmdarg[i];
		if(0 == i % 16) {
			plat_printf("OtaMqtt_HandleCmd \n        ");
		}
		plat_printf("OtaMqtt_HandleCmd %02X'%c' ", c, c);
	}
	memset(g_ota_req_from_ont, 0 , sizeof(g_ota_req_from_ont));
	memcpy(g_ota_req_from_ont, temp, cmdarg_len);
	
	plat_printf("OtaMqtt_HandleCmd ############%s\n", temp);


	plat_printf("OtaMqtt_HandleCmd send the cmd resp with Qos=1\n");
	int err = Mqtt_PackCmdRetPkt(ctx->Mqtt_Buf, 1, ctx->cmdid, "hello MQTT", 11, MQTT_QOS_LEVEL1,1);
	if(MQTTERR_NOERROR != err) {
		plat_printf("OtaMqtt_HandleCmd Critical bug: failed to pack the cmd ret packet.\n");
		return -1;
	}

	return 0;
}



int OtaMqttContext_init(struct OtaMqttContext *ctx)
{
	
	int err = 0;

	ctx->Sended_Bytes = -1;
	ctx->Mqtt_Fd = -1;

	err = Mqtt_InitContext(ctx->Mqtt_Ctx, 1 << 13);

	if(MQTTERR_NOERROR != err) {
		return -1;
	}

	ctx->Mqtt_Ctx->read_func = mqtt_recv_pkt;
	ctx->Mqtt_Ctx->read_func_arg = (void*)(&(ctx->Mqtt_Fd));
	ctx->Mqtt_Ctx->writev_func = mqtt_send_pkt;
	ctx->Mqtt_Ctx->writev_func_arg = (void*) (void*)(&(ctx->Mqtt_Fd));
	ctx->Mqtt_Ctx->handle_conn_ack = OtaMqtt_HandleConnAck;
	ctx->Mqtt_Ctx->handle_conn_ack_arg = ctx;
	ctx->Mqtt_Ctx->handle_ping_resp = OtaMqtt_HandlePingResp;
	ctx->Mqtt_Ctx->handle_ping_resp_arg = ctx;
	ctx->Mqtt_Ctx->handle_publish = OtaMqtt_HandlePublish;
	ctx->Mqtt_Ctx->handle_publish_arg = ctx;
	ctx->Mqtt_Ctx->handle_pub_ack = OtaMqtt_HandlePubAck;
	ctx->Mqtt_Ctx->handle_pub_ack_arg = ctx;
	ctx->Mqtt_Ctx->handle_pub_rec = OtaMqtt_HandlePubRec;
	ctx->Mqtt_Ctx->handle_pub_rec_arg = ctx;
	ctx->Mqtt_Ctx->handle_pub_rel = OtaMqtt_HandlePubRel;
	ctx->Mqtt_Ctx->handle_pub_rel_arg = ctx;
	ctx->Mqtt_Ctx->handle_pub_comp = OtaMqtt_HandlePubComp;
	ctx->Mqtt_Ctx->handle_pub_comp_arg = ctx;
	ctx->Mqtt_Ctx->handle_sub_ack = OtaMqtt_HandleSubAck;
	ctx->Mqtt_Ctx->handle_sub_ack_arg = ctx;
	ctx->Mqtt_Ctx->handle_unsub_ack = OtaMqtt_HandleUnsubAck;
	ctx->Mqtt_Ctx->handle_unsub_ack_arg = ctx;
	ctx->Mqtt_Ctx->handle_cmd = OtaMqtt_HandleCmd;
	ctx->Mqtt_Ctx->handle_cmd_arg = ctx;

	ctx->cmdid[0] = '\0';			//TODO delete
	MqttBuffer_Init(ctx->Mqtt_Buf);
	ctx->encrypt_enable = DISABLE_ENCRYPT;
	ctx->write_ota_success = OTA_WRITE_FAIL;
	return 0;
}


static int ota_mqtt_register_connect(struct OtaMqttContext *ctx, int keepalive)
{
	int err ;

	if (ctx->Mqtt_Fd >= 0) {
		plat_socket_close(ctx->Mqtt_Fd);
	}

	MqttBuffer_Init(ctx->Mqtt_Buf);
	err = Ota_CreateTcpConnect(ctx->ServerIp,ctx->ServerPort);
	if(err >0 )
	{
		ctx->Mqtt_Fd = err;
	}else {
		return -1;	
	}
	plat_printf("mqtt_connect fd = %d \n",ctx->Mqtt_Fd);

	err = ota_http_register_device(ctx);
	if (MQTTERR_NOERROR != err) {
		
		plat_printf("mqtt_connect http_register_device() failed.\n");
		return -1;
	}

	err = Mqtt_PackConnectPkt(ctx->Mqtt_Buf, 
			keepalive, 
			ctx->Device_ID, 
			1, 
			NULL, 
			NULL,
			0,
			MQTT_QOS_LEVEL0, 
			0,
			ctx->Product_ID,
			ctx->Device_Auth,
			strlen(ctx->Device_Auth));
	if (MQTTERR_NOERROR != err) {
		
		plat_printf("Mqtt_PackConnectPkt() failed. Failed to pack the MQTT CONNECT PACKET, errcode is %d.\n", err);
		return -1;
	}

	err = Ota_Mqtt_Send_Action(ctx);
	if (MQTTERR_NOERROR != err) {
		plat_printf("ota_Mqtt_Send_Action() failed.\n", err);
		return -1;
	}

	err =  Ota_Mqtt_Recv_Action(ctx);			//TODO
	if (MQTTERR_NOERROR != err) {
		plat_printf("Mqtt_Recv_Action() failed.\n", err);
		return -1;
	}


	return 0;
}

void cal_sm4_encrypt_key(struct OtaMqttContext *ctx)
{
	int sm3_in_len = 0;
	int dev_id_len = 0;
	int i = 0;
	 char sm3_in[TIME_LEN+DEV_ID_LEN+SALT_LEN] = {0};
	 char sm3_out[SM3_OUT_LEN] = {0};

	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);   
	 char *sm4_key = ctx->sm4_key; 

	strncpy(sm3_in,pdev_info->time, TIME_LEN);

	dev_id_len = strlen(ctx->Device_ID);
    strncpy(sm3_in+TIME_LEN, ctx->Device_ID, dev_id_len ); 
    strncpy(sm3_in+TIME_LEN+dev_id_len, SALT, strlen(SALT)); 
    
	sm3_in_len = TIME_LEN + strlen(SALT) + dev_id_len;
	
	SM3(sm3_in, sm3_in_len, sm3_out);

	for (i = 0; i < 16; i++) {
		*(sm4_key + i) = *(sm3_out + 16 + i);
	}

}

void cal_sm4_decrypt_key(char *time,struct OtaMqttContext *ctx)
{
    int sm3_in_len = 0;
	int dev_id_len = 0;
	int i = 0;
	 char sm3_in[TIME_LEN+DEV_ID_LEN+SALT_LEN] = {0};
	 char sm3_out[SM3_OUT_LEN] = {0};
	 char *sm4_key = ctx->sm4_key; 

	
	strncpy(sm3_in, time, TIME_LEN);
	dev_id_len = strlen(ctx->Device_ID);
    strncpy(sm3_in+TIME_LEN, ctx->Device_ID, dev_id_len ); 
    strncpy(sm3_in+TIME_LEN+dev_id_len, SALT, strlen(SALT)); 
    
	sm3_in_len = TIME_LEN + strlen(SALT) + dev_id_len;
	SM3(sm3_in, sm3_in_len, sm3_out);

	for (i = 0; i < 16; i++) {
		*(sm4_key + i) = *(sm3_out + 16 + i);
	}

}

// Function name:
//     mqtt_publish_bin()
// Description:
//     publish a bin
// Parameter:
//     ctx: Mqtt Context
//     qos: quality level
//     str: publish content, like "dsid,desc,data"
//     len: content length
// Return Value:
//    -1: Failure
//     0: Success
static int ota_mqtt_publish_bin(struct OtaMqttContext *ctx, int qos, char *str, int len, char *transparent_str, int transparent_len){
	int retain = 0;
	int own = 1;
	int err = MQTTERR_NOERROR;

	char *dsid = (char *)plat_malloc(MAX_DATA_BIN_DSID_UNITCHAR);
	memset(dsid, 0, MAX_DATA_BIN_DSID_UNITCHAR);
	char *desc = (char *)plat_malloc(MAX_DATA_BIN_DESC_UNITCHAR);
	memset(desc, 0, MAX_DATA_BIN_DESC_UNITCHAR);

	char *tempptr = NULL;
	char *tempstr = str;


	if (-1 != ctx->Mqtt_Fd) {
		/*****************
		 * step 1: decode dsid
		 ****************/
		tempptr = strchr(tempstr, ',');
		if (NULL != tempptr) {
			memcpy(dsid, tempstr, (int)(tempptr - tempstr));
			tempstr = tempptr + 1;
		}
		else {
			return - 1;
		}


		/*****************
		 * step 2: decode desc
		 ****************/
		memcpy(desc, tempstr, strlen(tempstr));
		plat_printf("strlen(tempstr) = %d\n", strlen(tempstr));

		MqttBuffer_Init(ctx->Mqtt_Buf);
		err = Mqtt_PackDataPointByBinary(ctx->Mqtt_Buf, g_pkt_id++, dsid, desc, 0, transparent_str, transparent_len, qos, retain, own);
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin Mqtt_PackDataPointByString fail.\n");
			return -1;
		}


		err = Ota_Mqtt_Send_Action(ctx);
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin  Ota_Mqtt_Send_Action() failed.\n", err);
			return -1;
		}

		err = Ota_Mqtt_Recv_Action(ctx);			//TODO
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin  Ota_Mqtt_Recv_Action() failed.\n", err);
			return -1;
		}
	}
	else {
		plat_printf("ota_mqtt_publish_bin  ctx->Mqtt_Fd has been closed.\n", err);
		return -1;
	}

	plat_free(dsid);
	plat_free(desc);
	return 0;
}

//构造info信息
static int ota_CreateInfo(struct OtaMqttContext *ctx){
	 
	// struct timeval tpstart;
	// gettimeofday(&tpstart,NULL);
	// srand(tpstart.tv_usec);
	 
	// time_t curtime;
	// time(&curtime);
	// struct tm *p = localtime(&curtime);
	 //char timeStr[512];
	 //sprintf(timeStr,"%d-%d-%d %d:%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);  
	plat_printf("\n");
	showInfo(ctx);
	plat_printf("\n");
	plat_sprintf(pdev_info->send_buffer,"{ \
	 	\"datastreams\": [ \
	 	{ \"id\":\"info\",\"datapoints\":[{\"value\": \"IMSI:%s ,IMEI:%s,EID:%s,ICCID:%s,LAC:%s,CELLID:%s\"}] }\
	 	] \
	 	}",pdev_info->IMSI,pdev_info->IMEI,pdev_info->EID,pdev_info->ICCID,pdev_info->LAC,pdev_info->CELLID);
	 pdev_info->send_len = strlen(pdev_info->send_buffer);
	plat_printf("create info : \n%s \n len = %d \n",pdev_info->send_buffer,pdev_info->send_len);
	 return 0;
}

//构造准备信息
static int ota_createReady(struct OtaMqttContext *ctx){
	 
	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);  

	plat_sprintf(pdev_info->send_buffer,"{ \
	 	\"datastreams\": [ \
	 	{ \"id\":\"otaReady\",\"datapoints\":[{\"value\": \"OTAREADY:%s\"}] }\
	 	] \
	 	}",ctx->ReqID);
	 pdev_info->send_len = strlen(pdev_info->send_buffer);
	plat_printf("create ready : \n%s \n len = %d \n",pdev_info->send_buffer,pdev_info->send_len);
	return 0;
}


static int ota_mqtt_publish_string(struct OtaMqttContext *ctx, int qos,  char *transparent_str, int transparent_len){
	int retain = 0;
	int own = 1;
	int err = MQTTERR_NOERROR;

	if (-1 != ctx->Mqtt_Fd) {

		MqttBuffer_Init(ctx->Mqtt_Buf);
		
		err = Mqtt_PackDataPointByString(ctx->Mqtt_Buf, g_pkt_id++, 0, kTypeFullJson,  transparent_str, transparent_len, qos, retain, own);
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin Mqtt_PackDataPointByString fail.\n");
			return -1;
		}

		err = Ota_Mqtt_Send_Action(ctx);
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin  Ota_Mqtt_Send_Action() failed.\n", err);
			return -1;
		}

		err = Ota_Mqtt_Recv_Action(ctx);			//TODO
		if (MQTTERR_NOERROR != err) {
			plat_printf("ota_mqtt_publish_bin  Ota_Mqtt_Recv_Action() failed.\n", err);
			return -1;
		}
	}
	else {
		plat_printf("ota_mqtt_publish_bin  ctx->Mqtt_Fd has been closed.\n", err);
		return -1;
	}

	return 0;
}

int ota_write_time(struct OtaDeviceInfo *ctx)
{
	TIME_T timep;
	struct plat_tm *p;

	plat_time(&timep);
	p = plat_localtime(&timep);
	plat_strftime(ctx->time, TIME_LEN, "%d%H%M%S",  p);
	return 0;
}



static int ota_mqtt_connect(struct OtaMqttContext *ctx )
{
	int ret = -1;

	ret = ota_mqtt_register_connect(ctx, MQTT_KEEPALIVE_TIME);


	return ret;
}


static int ota_mqtt_send_info(struct OtaMqttContext *ctx )
{
	int ret = -1;
	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);  

	if(ctx->encrypt_enable == ENABLE_ENCRYPT)
	{
		int sm4_in_len = 0;
		int sm4_out_len = 0;
		int sm3_in_len = 0;
		int sm3_out_len = 0;

	 	char sm4_src_buf[SM4_BUF_LEN] = {0};
	 	char sm3_src_buf[SM3_BUF_LEN] = {0};
	 	char sm4_dst_buf[SM4_BUF_LEN] = {0};
	 	char sm3_dst_buf[SM3_BUF_LEN] = {0};

	// iccid eid imsi imei lac cellid
		strncat(sm4_src_buf, pdev_info->ICCID, ICCID_LEN);
		strncat(sm4_src_buf, pdev_info->EID, EID_LEN);
		strncat(sm4_src_buf, pdev_info->IMSI, IMSI_LEN);
		strncat(sm4_src_buf, pdev_info->IMEI, IMEI_LEN);
		strncat(sm4_src_buf, pdev_info->LAC, LAC_LEN);
		strncat(sm4_src_buf, pdev_info->CELLID, CELLID_LEN);

		ota_write_time(pdev_info);

		sm4_in_len = IMEI_LEN + IMSI_LEN + EID_LEN + ICCID_LEN + LAC_LEN + CELLID_LEN;
		sm4_out_len = sm4_in_len < 16 ? 16 : (sm4_in_len + 16 - sm4_in_len % 16);
		cal_sm4_encrypt_key(ctx);


		ret = SM4_Encrypt(ctx->sm4_key, sm4_src_buf, sm4_in_len, sm4_dst_buf);

		memcpy(sm3_src_buf, pdev_info->time, TIME_LEN); 
		memcpy(sm3_src_buf+TIME_LEN, sm4_dst_buf, sm4_out_len); 
		sm3_in_len = TIME_LEN + sm4_out_len;
		sm3_out_len = SM3_OUT_LEN;
		SM3(sm3_src_buf, sm3_in_len, sm3_dst_buf);
		memcpy(pdev_info->send_buffer, pdev_info->time, TIME_LEN);
		memcpy(pdev_info->send_buffer+TIME_LEN, sm4_dst_buf, sm4_out_len);
		memcpy(pdev_info->send_buffer+TIME_LEN+sm4_out_len, sm3_dst_buf, sm3_out_len);
		pdev_info->send_len =  TIME_LEN + sm4_out_len + sm3_out_len;
		char *json_content = "ds_1,ota";
		int json_content_len = strlen(json_content) + 1;
		ret = ota_mqtt_publish_bin(ctx, 1, json_content, json_content_len, pdev_info->send_buffer, pdev_info->send_len);

	}else {
		ota_CreateInfo(ctx);
		ret = ota_mqtt_publish_string(ctx, 1,  pdev_info->send_buffer, pdev_info->send_len);
	
	}

	return ret;
}


static int ota_mqtt_wait_req(struct OtaMqttContext *ctx )
{
	int ret = -1;
	int cnt = 0;
		
	unsigned char time[TIME_LEN + 1] = {0};
	unsigned char otareq[OTA_REQ_LEN + 1] = {0};
	unsigned char check[SM3_OUT_LEN + 1] = {0};
	unsigned char sm4_out[SM4_BUF_LEN] = {0};
	char *head = NULL;
	while(1) {
		
			if (-1 == Ota_Mqtt_Recv_Action(ctx)) 
			{
				ret =-1;
				break;
			}
			if (cnt++ > 60) {
				ret = -1;
				break;
			}
			if(ctx->encrypt_enable == ENABLE_ENCRYPT)
			{
				memcpy(time, g_ota_req_from_ont, TIME_LEN);
				memcpy(otareq, g_ota_req_from_ont+TIME_LEN, OTA_REQ_LEN);
				memcpy(check, g_ota_req_from_ont+OTA_REQ_LEN+TIME_LEN, SM3_OUT_LEN);
				memset(ctx->sm4_key, 0, SM4_KEY_LEN);
				cal_sm4_decrypt_key(time, ctx);
				SM4_Decrypt(ctx->sm4_key, otareq, 16, sm4_out);
			}else
			{
				memcpy(sm4_out, g_ota_req_from_ont, strlen(g_ota_req_from_ont)+1);

			}
			
			if (strncmp(sm4_out, "OTAREQ", strlen("OTAREQ")) != 0) {
				plat_printf("ota_mqtt_wait_req Recv cmd is not OTAREQ\n");
				continue;
			} else {
				//OTAREQ:xxxxxxxx
				plat_printf("ota_write_wait_req Recv the OTAREQ command success\n");
				head = ota_strstr(sm4_out, "OTAREQ:", strlen(sm4_out));
				if (head == NULL)
					return -1;
				head += 7;
				memcpy(ctx->ReqID,head,strlen(sm4_out)-6);// bufferlen - otareq:len +1 
				plat_printf("ota_write_wait_req Recv the OTAREQ command success %s  reqid = %s\n",sm4_out,ctx->ReqID);
				ret = 0;
				break;
			}
		}
		return ret;

}



static int ota_mqtt_send_ready(struct OtaMqttContext *ctx )
{

	int ret = 0;

	int execute_status = 0; 


	struct OtaDeviceInfo  *pdev_info = &(ctx->dev_info);   
	memset(pdev_info->send_buffer, 0, sizeof(pdev_info->send_buffer));
	memset(pdev_info->OLD_IMSI, 0, IMSI_LEN);
	memcpy(pdev_info->OLD_IMSI, pdev_info->IMSI, IMSI_LEN);

	if(ctx->encrypt_enable == ENABLE_ENCRYPT)
	{
			
		int sm4_in_len = 0;
		int sm4_out_len = 0;
		int sm3_in_len = 0;
		int sm3_out_len = 0;

		unsigned char sm4_src_buf[SM4_BUF_LEN] = {0};
		unsigned char sm3_src_buf[SM3_BUF_LEN] = {0};
		unsigned char sm4_dst_buf[SM4_BUF_LEN] = {0};
		unsigned char sm3_dst_buf[SM3_BUF_LEN] = {0};
    
	
    	ota_write_time(pdev_info);

		sm4_in_len = strlen("OTAREADY");
		strncat(sm4_src_buf, "OTAREADY", sm4_in_len);
		sm4_out_len = strlen("OTAREADY") < 16 ? 16 : (strlen("OTAREADY") + 16 - strlen("OTAREADY") % 16) ;
		cal_sm4_encrypt_key(ctx);
		ret = SM4_Encrypt(ctx->sm4_key, sm4_src_buf, sm4_in_len, sm4_dst_buf);
		plat_printf("main SM4 in is  %s\n", sm4_src_buf);
		plat_printf("main  SM4 encrypt result is  %d\n", ret);
		plat_printf("main SM4 in len  is  %d\n", sm4_in_len);

   	 	memcpy(sm3_src_buf, pdev_info->time, TIME_LEN); 
   	 	memcpy(sm3_src_buf+TIME_LEN, sm4_dst_buf, sm4_out_len); 
		sm3_in_len = TIME_LEN + sm4_out_len;
		sm3_out_len = SM3_OUT_LEN;
		SM3(sm3_src_buf, sm3_in_len, sm3_dst_buf);
		memcpy(pdev_info->send_buffer, pdev_info->time, TIME_LEN);
		memcpy(pdev_info->send_buffer+TIME_LEN, sm4_dst_buf, sm4_out_len);
		memcpy(pdev_info->send_buffer+TIME_LEN+sm4_out_len, sm3_dst_buf, sm3_out_len);
		pdev_info->send_len = TIME_LEN + sm4_out_len + sm3_out_len;
		plat_printf("main ota send ready : send_len  is  %d\n", pdev_info->send_len);

		char *json_content = "ds_1,des";
		int json_content_len = strlen(json_content) + 1;
		execute_status = ota_mqtt_publish_bin(ctx, 1, json_content, json_content_len, pdev_info->send_buffer, pdev_info->send_len);
		plat_printf("write_card_process execute_status is %d \n", execute_status);
   		ret = execute_status;
		}else 
		{
			ota_createReady(ctx);
			ret = ota_mqtt_publish_string(ctx, 1,  pdev_info->send_buffer, pdev_info->send_len);
		}
	return ret;
}

static void ota_mqtt_close_connect(struct OtaMqttContext *ctx)
{
	plat_socket_close(ctx->Mqtt_Fd);
	ctx->Mqtt_Fd = -1;
}





int OtaMqtt_Start_Main_loop(struct OtaMqttContext *ctx)
{

	int ret = 0;
	plat_printf("start\n");
	showInfo(ctx);
	char new_imsi[MAX_TEMPSTR_LENGTH] = {0};
	while(1) {
		
		//ota  mqtt connect 
		ret = ota_mqtt_connect(ctx);
		if (-1 == ret) {
			plat_printf("ota_mqtt_connect  failed\n");
			plat_sleep(OTA_START_PERIOD);
			continue;
		}
		plat_printf("[ota_mqtt_connect]  success\n");
		
		//ota  mqtt sendinfo 
		ret = ota_mqtt_send_info(ctx);
		if (-1 == ret) {
		    plat_printf("OtaMqtt_Start ota_mqtt_send_info failed\n");
			plat_sleep(OTA_START_PERIOD);
			continue;
		}
		plat_printf("[ota_mqtt_send_info]  success\n");

		//if write success we should exit Main_loop
		if(ctx->write_ota_success == OTA_WRITE_SUCCESS)
		{
			break;
		}
		//ota  mqtt wait_req 
		ret = ota_mqtt_wait_req(ctx);
		if (-1 == ret) {
		    plat_printf("ota_mqtt_wait_req  failed\n");	
			plat_sleep(OTA_START_PERIOD);
			continue;
		}
		plat_printf("[ota_mqtt_wait_req]  success\n");
		ret = ota_mqtt_send_ready(ctx);
		if (-1 == ret) {
		    plat_printf("ota_mqtt_send_ready  failed\n");	
			plat_sleep(OTA_START_PERIOD);
			continue;
		}
		ota_mqtt_close_connect(ctx);
		ret = ctx->check_ota_write_result((void *)ctx->check_arg,new_imsi);
		
		if (-1 == ret) {
			plat_printf("check_ota_write_result  failed\n");  
			continue;
		}else{
			if(strncmp(ctx->dev_info.OLD_IMSI, new_imsi, IMSI_LEN) == 0 || strlen(new_imsi) != IMSI_LEN )
			{
				continue;
			}
			ctx->write_ota_success = OTA_WRITE_SUCCESS;
			memcpy(ctx->dev_info.IMSI,new_imsi,strlen(new_imsi));
			plat_sleep(OTA_START_PERIOD);
		}				

	}
	return 0;
}







