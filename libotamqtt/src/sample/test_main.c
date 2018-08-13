#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "otamqtt.h"





#define MQTTIP	"183.230.40.39"
#define MQTTPORT	6002


#define OneNETRegisterCode2G "Kr4rubLBKlCv8KlS"
#define ProductID2G	"149958"


#define ProductID4G  "150819"
#define OneNETRegisterCode4G  "G5ydjvQWLIXWdwCV"                                                                                                         




 
static int card_get_lac_cellid(char *lac, char *cellid)
{
	memcpy(lac,"102A",5);
	memcpy(cellid,"079D5983",9);
	return 0;
	
	
	
}


int ota_write_time_zte(struct OtaDeviceInfo *ctx)
{
	time_t timep;
	struct tm *p;

	time(&timep);
	p = localtime(&timep);
	strftime(ctx->time, TIME_LEN, "%d%H%M%S",  p);
	return 0;
}


int ota_write_imei_zte(struct OtaDeviceInfo *ctx)
{

	strncpy(ctx->IMEI, "123456789012345", IMEI_LEN);
	return 0;
}

int ota_write_imsi_zte(struct OtaDeviceInfo *ctx)
{

	strncpy(ctx->IMSI, "123456789012345", IMSI_LEN);
	return 0;
}

int ota_write_eid_zte(struct OtaDeviceInfo *ctx)
{
	//strncpy(ctx->EID, "12345678901234567890", EID_LEN);
	//test write because zte can't get eid , user should modify use card eid info 
	strncpy(ctx->EID,   "151804114A1040000027", EID_LEN);
	return 0;
}

int ota_write_iccid_zte(struct OtaDeviceInfo *ctx)
{

	memcpy(ctx->ICCID,"898602B4151840000026" , 21);
	return 0;
}

int ota_write_lac_cellid_zte(struct OtaDeviceInfo *ctx)
{

	char *lac_str = (char *)malloc(50);
	char *cellid_str = (char *)malloc(50);
	memset(lac_str, 0, 50);
	memset(cellid_str, 0, 50);
	card_get_lac_cellid(lac_str, cellid_str);

	memcpy(ctx->LAC, lac_str, strlen(lac_str) + 1);
	memcpy(ctx->CELLID, cellid_str, strlen(cellid_str) + 1);

	free(lac_str);
	free(cellid_str);
	return 0;
}



void ont_mqtt_sleep(int milliseconds)
{
	usleep( milliseconds*1000 );
}


int ota_write_get_new_imsi(char *imsi)
{

	return 0;

}


int check_ota_write_zte(void *arg,char *new_imsi)
{


	return -1;
}




int user_init_context(struct OtaMqttContext *ctx)
{
	//onenet init  serverip port productid registercode 
	memcpy(ctx->ServerIp,MQTTIP,strlen(MQTTIP)+1);
	ctx->ServerPort = MQTTPORT;

	memcpy(ctx->Product_ID,ProductID4G,strlen(ProductID4G)+1);
	memcpy(ctx->Register_Code,OneNETRegisterCode4G,strlen(OneNETRegisterCode4G)+1);

	//device info init 
	struct OtaDeviceInfo  *pdev_info =&(ctx->dev_info) ;   
   // ota_write_time_zte(pdev_info);
    ota_write_imei_zte(pdev_info);
    ota_write_imsi_zte(pdev_info);
    ota_write_eid_zte(pdev_info);
    ota_write_iccid_zte(pdev_info);
    ota_write_lac_cellid_zte(pdev_info);
	//check_ota_result  init 
	ctx->check_arg = (void *)ctx;
	ctx->check_ota_write_result = check_ota_write_zte;

	return 0;
	

}


/************************************
 * Main function
 ***********************************/
int main(int argc, char** argv)
{

	// Log system initialization
	
	
	/****************************
	 * Enviroment initialization
	 ***************************/

	//  context initialization use sdk interface 
	struct OtaMqttContext ota_mqtt_ctx[1];
	memset(ota_mqtt_ctx, 0, sizeof(struct OtaMqttContext));
	if( OtaMqttContext_init(ota_mqtt_ctx)< 0) {
		printf("main  OtaMqttContext_init() critical error, quit the application!\n");
       return   -1;
    }

	// context need user init 
	

	if( user_init_context(ota_mqtt_ctx)< 0) {
		printf("main main OtaMqttContext_init() critical error, quit the application!\n");
		return  -1;
	}

	OtaMqtt_Start_Main_loop(ota_mqtt_ctx);
	
	return 0;
}

