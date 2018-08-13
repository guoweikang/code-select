

#include "lwm2msdk.h"
#ifdef HAVE_SIGNAL
#include <signal.h>
#endif 
#include "plat_stdio.h"
#include "plat_string.h"
#include "plat_stdlib.h"

#define CFG_PATH "/opt/config_file.txt"

#define BUFFER_LEN 100

static int s_quit = 0;
static int s_state = 0;

#ifdef HAVE_SIGNAL
void handle_sig(int signum)
{
    s_quit = 1;
    LWM2M_SDK_STOP();
}
#endif 

void  myNotifyMsg(OptNotifyParam *optNotifyParam)
{
   if(optNotifyParam==NULL)
    return ;

    plat_printf("myNotifyMsg:type=%0x,code=%d,msg=%s\n",optNotifyParam->notify_type,optNotifyParam->notify_code,optNotifyParam->notify_msg);
    switch (optNotifyParam->notify_type)
    {
    case NOTIFY_TYPE_SYS:
    break;
    case NOTIFY_TYPE_REGISTER:
        if(optNotifyParam->notify_code == NOTIFY_CODE_OK )
        {
            s_state = 0;
        }
        else
        {
            s_state = 1;
        }
    break;
    case NOTIFY_TYPE_DEREGISTER:
         s_state = 3;
    break;
    case NOTIFY_TYPE_REG_UPDATE:
        if(optNotifyParam->notify_code == NOTIFY_CODE_OK )
        {
            s_state = 0;
        }
        else
        {
            s_state = 2;
        }
    break;
    default:
    break;  
    }
    
}



int main(int argc, char *argv[])
{
	char *buffer_data = plat_malloc(BUFFER_LEN);
	char imsi[20];		//IMSI 15 char length
	char iccid[30];		//ICCID 20 char length
	char imei[20];		//IMEI 15 char length
	int i = 0;
	char *tempstr1 = NULL;
	char *tempstr2 = NULL;

	plat_printf("sizeof(int) is %d\n", sizeof(int));

	memset(buffer_data, 0, BUFFER_LEN);
	memset(imsi, 0, 20);
	memset(iccid, 0, 30);
	memset(imei, 0, 20);

	PLAT_FILE *config_file = plat_fopen(CFG_PATH, "r");
	if (!config_file) {
		plat_printf("not found the config file or no access.\n");
		plat_exit(0);
	}
	while(!plat_feof(config_file)) {
		plat_fread(buffer_data + i, 1, 1, config_file);
		i ++;
	}
	plat_fclose(config_file);
	
	tempstr1 = strchr(buffer_data, ',');
	if (NULL != tempstr1) {
		memcpy(imsi, buffer_data, (int)(tempstr1 - buffer_data));
		tempstr1 = tempstr1 + 1;
	}
	else{
		plat_printf("Wrong format, comma is missing.\n");
		plat_exit(0);
	}

	tempstr2 = strchr(tempstr1, ',');
	if (NULL != tempstr2) {
		memcpy(iccid, tempstr1, (int)(tempstr2 - tempstr1));
		tempstr2 = tempstr2 + 1;
	}
	else{
		plat_printf("Wrong format, comma is missing.\n");
		plat_exit(0);
	}

	memcpy(imei, tempstr2, 15);

	

	/************************************
	 * Main process
	 ************************************/
	plat_printf("imsi: %s\n", imsi);
	plat_printf("iccid: %s\n", iccid);
	plat_printf("imei: %s\n", imei);

	Options opt = {"", "", "", "C417M_S.S01", "M100000052", "n525A97z0M7Vyh91b0508l7j0U5g2g9Y", 4, "shipei.fxltsbl.com", 5683, 60, 56831, 0};
	//////Options opt = {"861111003929109", "", "460041850403690", "v1.0", "M100000052", "n525A97z0M7Vyh91b0508l7j0U5g2g9Y", 4, "shipei.fxltsbl.com", 5683, 300, 56831, 0};
	memset(opt.szCMEI_IMEI, 0, sizeof(opt.szCMEI_IMEI));
	memset(opt.szIMSI, 0, sizeof(opt.szIMSI));
	memcpy(opt.szCMEI_IMEI, imei, sizeof(imei));
	memcpy(opt.szIMSI, imsi, sizeof(imsi));
	OptFuncs optfuncs={NULL};
    optfuncs.NotifyMsg = myNotifyMsg;

	int nrunmode = 1; // thread
	if(argc>1)
		nrunmode = plat_atoi(argv[1]);
	
	if(argc>2)
	{
		strcpy(opt.szSrvIP,argv[2]);
	}
	
	if(argc>3)
	{
		strcpy(opt.szPwd,argv[3]);
	}
#ifdef HAVE_SIGNAL
	signal(SIGINT, handle_sig);
#endif 
	//init config
	int ret = LWM2M_SDK_INIT(&opt,&optfuncs);

	if(nrunmode==0)
	{
		while(s_quit==0)
		{
		    plat_printf("[%s:%d]to run in main thread\n",__func__,__LINE__);
		    ret = LWM2M_SDK_STOP();
		    ret = LWM2M_SDK_RUN(0);
		    if(s_quit==0 && s_state != 0)
		    {
		        plat_sleep(30);
		        continue;
		    }	    	
		    
		    
		}
	}
	else
	{
		plat_printf("to run in created thread\n");
		ret = LWM2M_SDK_RUN(1);
		
		
		int ntimes = 3600 * 24;
		while(s_quit==0 && ntimes-- > 0)
		{
		    if(ntimes%60==0)
		        plat_printf("main thread sleep[%d]\n",ntimes);
		        
			plat_sleep(1);
		}
	}

	plat_printf("main thread to stop workthread\n");
	LWM2M_SDK_STOP();
	
	LWM2M_SDK_FINI();
	plat_printf("main quit \n");

	plat_free(buffer_data);
	return 0;
}


