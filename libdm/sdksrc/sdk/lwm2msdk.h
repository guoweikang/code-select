/** 
	* @file 		lwm2msdk.h
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

#ifndef LWM2M_SDK_H  
#define LWM2M_SDK_H  
  
#include "plat_stdio.h"  
  
#ifdef __cplusplus
extern "C" {
#endif


//閫氱煡绫诲瀷
enum _SDK_NOTIFY_TYPE
{
      NOTIFY_TYPE_SYS           = 0x0000  //SDK杩愯
    , NOTIFY_TYPE_REGISTER      = 0x0100  //娉ㄥ唽
    , NOTIFY_TYPE_DEREGISTER              //娉ㄩ攢
    , NOTIFY_TYPE_REG_UPDATE              //鏇存柊
};
typedef enum _SDK_NOTIFY_TYPE SDK_NOTIFY_TYPE;


enum _SDK_NOTIFY_CODE
{
      NOTIFY_CODE_OK      = 0x0000 //鎴愬姛
    , NOTIFY_CODE_FAILED 
    , NOTIFY_CODE_UNKNOWEN 
};
typedef enum _SDK_NOTIFY_CODE SDK_NOTIFY_CODE;

//sdk娑堟伅閫氱煡鍑芥暟鍙傛暟
typedef struct
{

	SDK_NOTIFY_TYPE notify_type;
	SDK_NOTIFY_CODE notify_code;
	const char *    notify_msg;
	
} OptNotifyParam;


typedef struct
{
    //sdk娑堟伅閫氱煡鍑芥暟--no block
	void  (*NotifyMsg) (OptNotifyParam *optNotifyParam);
	
} OptFuncs;


typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI
	char szCMEI_IMEI2[64]; 	//CMEI/IMEI2
	char szIMSI[64];		//IMSI
	char szDMv[16]; 		//DM鐗堟湰鍙?
	char szAppKey[64]; 		//appkey
	char szPwd[64];			//pwd绉橀挜
	
	int  nAddressFamily; 	//4鎴?  ipv4锛宨pv6
	char szSrvIP[32];		//127.0.0.1
    int  nSrvPort;   		//5683
	int  nLifetime;    		//300
	int  nLocalPort;        //鏈湴绔彛缁戝畾,榛樿涓?6830
	int  nBootstrap; 		//鏄惁booststrap 0,not use; other n,use;
	
} Options;


//鍒濆鍖朞ptions:鍙傛暟,optfuncs:鍥炶皟鍑芥暟
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 
//鍚姩sdk锛宯Mode=0,闃诲鍦ㄥ綋鍓嶇嚎绋嬭繍琛岋紱nMode!=0,鍦ㄦ柊绾跨▼涓繍琛?
int LWM2M_SDK_RUN(int nMode);
//閫�鍑篠DK鐨勫惊鐜紱娉ㄩ攢lwm2m鏈嶅姟
int LWM2M_SDK_STOP();
//娓呯悊sdk,鍙傛暟绛?
int LWM2M_SDK_FINI();
  

#ifdef __cplusplus
}
#endif
  
#endif

