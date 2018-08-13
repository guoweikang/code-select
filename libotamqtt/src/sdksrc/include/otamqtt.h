




#ifndef OTA_MQTT_H
#define OTA_MQTT_H



#include "plat_stdint.h"
#include "mqtt.h"


#define IMSI_LEN       15
#define IMEI_LEN       15 
#define EID_LEN        20 
#define ICCID_LEN      20 
#define LAC_LEN        4
#define CELLID_LEN     8 



#define TIME_LEN     8 
#define OTA_REQ_LEN  16
#define DEV_ID_LEN     8 



/************************************
 * global micros definitions
 ***********************************/
#define MAX_TEMPSTR_LENGTH 128
#define MQTT_KEEPALIVE_TIME 120
#define MAX_DATA_LENGTH (1024 * 1024)

#define MAX_DATA_FLOAT_UINTCHAR (500 * 4 + 12)

#define MAX_DATA_BIN_DSID_UNITCHAR 60
#define MAX_DATA_BIN_DESC_UNITCHAR 160

#define MAX_CARD_LAC_LENGTH 18
#define MAX_CARD_CELLID_LENGTH 18
#define SERVER_IP_LENGTH 16


#define MAX_CMID_LENGTH 1024

#define REGISTER_BUFFER_LENTH 1024

/************************************
 *sm micros definitions
 ***********************************/
#define ENABLE_ENCRYPT    1
#define DISABLE_ENCRYPT    0
#define SM4_KEY_LEN    16
#define SM3_OUT_LEN    32
#define SM4_BUF_LEN 200
#define SM3_BUF_LEN 200

#define SALT        "IOT"
#define SALT_LEN     5 

#define OTA_WRITE_SUCCESS    1
#define OTA_WRITE_FAIL       0


/************************************
 *http micros definitions
 ***********************************/

#define HTTP_POST	"POST %s HTTP/1.1\r\nHOST:api.heclouds.com\r\n"\
	"Content-Length:%d\r\n\r\n%s\r\n"

#define HTTP_CONTEXT_SN		"{\"sn\":\"%s\",\"title\":\"%s\"}"
#define HTTP_CONTEXT_MAC	"{\"mac\":\"%s\",\"title\":\"%s\"}"
#define HTTP_CREATE_DEVICE_IP "183.230.40.33"
#define HTTP_CREATE_DEVICE_PORT 80

/************************************
ota repeat time 
 ***********************************/

#define OTA_START_PERIOD 60



struct OtaDeviceInfo 
{
	char time[TIME_LEN+1];
	char IMEI[IMEI_LEN+1];    //user init
	char IMSI[IMSI_LEN+1];    //user init
	char OLD_IMSI[IMSI_LEN+1];
	char EID[EID_LEN+1];      //user init
	char ICCID[ICCID_LEN+1];  //user init
	char LAC[LAC_LEN+1];      //user init
	char CELLID[CELLID_LEN+1];//user init
	unsigned char send_buffer[1024];
	int send_len;

};


struct OtaMqttContext
{
	
	int Mqtt_Fd;
	uint32_t Sended_Bytes;
	struct MqttContext Mqtt_Ctx[1];
	struct MqttBuffer Mqtt_Buf[1];

/* OneNet mqtt 平台连接参数*/
	char ServerIp[16];               //user init 
	unsigned short ServerPort;       //user init 
	char Product_ID[MAX_TEMPSTR_LENGTH / 2];  // user init  
	char Register_Code[MAX_TEMPSTR_LENGTH];   // user init 


	/*各个厂家提供检查空写结果函数实现
	  成功返回 0 失败返回-1 
	  注意：写卡成功，需要将新的IMSI 写入 new_imsi
	*/
	void *check_arg;     // user init 
	int (*check_ota_write_result)(void *arg,char *new_imsi);  // user init 


	char Device_ID[MAX_TEMPSTR_LENGTH / 2];   // user don't  init 
	char Device_Auth[MAX_TEMPSTR_LENGTH / 2]; // user don't  init 
	char Api_Key[MAX_TEMPSTR_LENGTH / 2];   //   user don't  init  

	/*设备卡信息*/
	int  write_ota_success;    //写卡成功标志位
	char ReqID[MAX_TEMPSTR_LENGTH];
	struct OtaDeviceInfo  dev_info;      //user init 


/*sm4 key*/
	int encrypt_enable;  
	char sm4_key[SM4_KEY_LEN];

	int dup;
	enum MqttQosLevel qos;
	int retain;
	
	uint16_t pkt_to_ack;
	char cmdid[MAX_CMID_LENGTH];			//TODO Delete
	

};



int OtaMqttContext_init(struct OtaMqttContext *ctx);

int OtaMqtt_Start_Main_loop(struct OtaMqttContext *ctx);








#endif 









