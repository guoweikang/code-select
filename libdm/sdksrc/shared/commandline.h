/** 
	* @file 		commandline.h
	* @brief		
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/11 , 第一次创建\n 
*/	

#include "plat_stdio.h"

#include "plat_stddef.h"
#include "plat_stdint.h"
#include "liblwm2m.h"

#define COMMAND_END_LIST {NULL, NULL, NULL, NULL, NULL}

typedef void (*command_handler_t) (char * args, void * user_data);

typedef struct
{
    char *            name;
    char *            shortDesc;
    char *            longDesc;
    command_handler_t callback;
    void *            userData;
} command_desc_t;


void handle_command(command_desc_t * commandArray, char * buffer);
char* get_end_of_arg(char* buffer);
char * get_next_arg(char * buffer, char **end);
int check_end_of_args(char* buffer);

void output_buffer(PLAT_FILE * stream, uint8_t * buffer, int length, int indent);
void output_tlv(PLAT_FILE * stream, uint8_t * buffer, SIZE_T buffer_len, int indent);
void dump_tlv(PLAT_FILE * stream, int size, lwm2m_data_t * dataP, int indent);
void output_data(PLAT_FILE * stream, lwm2m_media_type_t format, uint8_t * buffer, int length, int indent);
void print_status(PLAT_FILE * stream, uint8_t status);

SIZE_T base64_decode(uint8_t * dataP, SIZE_T dataLen, uint8_t ** bufferP);

