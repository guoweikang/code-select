/** 
	* @file 		commandline.c
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

#include "plat_string.h"
#include "plat_stdlib.h"
#include "plat_stdio.h"
#include "plat_stdint.h"
#include "plat_stddef.h"

#include <plat_inttypes.h>
#include "liblwm2m.h"

#include "commandline.h"

#define HELP_COMMAND "help"
#define HELP_DESC    "Type '"HELP_COMMAND" [COMMAND]' for more details on a command."
#define UNKNOWN_CMD_MSG "Unknown command. Type '"HELP_COMMAND"' for help."


static command_desc_t * prv_find_command(command_desc_t * commandArray,
                                         char * buffer,
                                         SIZE_T length)
{
    int i;

    if (length == 0) return NULL;

    i = 0;
    while (commandArray[i].name != NULL
        && (strlen(commandArray[i].name) != length || strncmp(buffer, commandArray[i].name, length)))
    {
        i++;
    }

    if (commandArray[i].name == NULL)
    {
        return NULL;
    }
    else
    {
        return &commandArray[i];
    }
}

static void prv_displayHelp(command_desc_t * commandArray,
                            char * buffer)
{
    command_desc_t * cmdP;
    int length;

    // find end of first argument
    length = 0;
    while (buffer[length] != 0 && !plat_isspace(buffer[length]&0xff))
        length++;

    cmdP = prv_find_command(commandArray, buffer, length);

    if (cmdP == NULL)
    {
        int i;

        plat_printf( HELP_COMMAND"\t"HELP_DESC"\r\n");

        for (i = 0 ; commandArray[i].name != NULL ; i++)
        {
            plat_printf( "%s\t%s\r\n", commandArray[i].name, commandArray[i].shortDesc);
        }
    }
    else
    {
        plat_printf( "%s\r\n", cmdP->longDesc?cmdP->longDesc:cmdP->shortDesc);
    }
}


void handle_command(command_desc_t * commandArray,
                    char * buffer)
{
    command_desc_t * cmdP;
    int length;

    // find end of command name
    length = 0;
    while (buffer[length] != 0 && !plat_isspace(buffer[length]&0xFF))
        length++;

    cmdP = prv_find_command(commandArray, buffer, length);
    if (cmdP != NULL)
    {
        while (buffer[length] != 0 && plat_isspace(buffer[length]&0xFF))
            length++;
        cmdP->callback(buffer + length, cmdP->userData);
    }
    else
    {
        if (!strncmp(buffer, HELP_COMMAND, length))
        {
            while (buffer[length] != 0 && plat_isspace(buffer[length]&0xFF))
                length++;
            prv_displayHelp(commandArray, buffer + length);
        }
        else
        {
            plat_printf( UNKNOWN_CMD_MSG"\r\n");
        }
    }
}

static char* prv_end_of_space(char* buffer)
{
    while (plat_isspace(buffer[0]&0xff))
    {
        buffer++;
    }
    return buffer;
}

char* get_end_of_arg(char* buffer)
{
    while (buffer[0] != 0 && !plat_isspace(buffer[0]&0xFF))
    {
        buffer++;
    }
    return buffer;
}

char * get_next_arg(char * buffer, char** end)
{
    // skip arg
    buffer = get_end_of_arg(buffer);
    // skip space
    buffer = prv_end_of_space(buffer);
    if (NULL != end)
    {
        *end = get_end_of_arg(buffer);
    }

    return buffer;
}

int check_end_of_args(char* buffer)
{
    buffer = prv_end_of_space(buffer);

    return (0 == buffer[0]);
}

/**********************************************************
 * Display Functions
 */

static void print_indent(PLAT_FILE * stream,
                         int num)
{
    int i;

    for ( i = 0 ; i < num ; i++)
        plat_fprintf(stream, "    ");
}

void output_buffer(PLAT_FILE * stream,
                   uint8_t * buffer,
                   int length,
                   int indent)
{
    int i;

    if (length == 0) plat_fprintf(stream, "\n");

    if (buffer == NULL) return;

    i = 0;
    while (i < length)
    {
        uint8_t array[16];
        int j;

        print_indent(stream, indent);
        memcpy(array, buffer+i, 16);
        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            plat_fprintf(stream, "%02X ", array[j]);
            if (j%4 == 3) plat_fprintf(stream, " ");
        }
        if (length > 16)
        {
            while (j < 16)
            {
                plat_fprintf(stream, "   ");
                if (j%4 == 3) plat_fprintf(stream, " ");
                j++;
            }
        }
        plat_fprintf(stream, " ");
        for (j = 0 ; j < 16 && i+j < length; j++)
        {
            if (plat_isprint(array[j]))
                plat_fprintf(stream, "%c", array[j]);
            else
                plat_fprintf(stream, ".");
        }
        plat_fprintf(stream, "\n");
        i += 16;
    }
}

void output_tlv(PLAT_FILE * stream,
                uint8_t * buffer,
                SIZE_T buffer_len,
                int indent)
{
    lwm2m_data_type_t type;
    uint16_t id;
    SIZE_T dataIndex;
    SIZE_T dataLen;
    int length = 0;
    int result;

    while (0 != (result = lwm2m_decode_TLV((uint8_t*)buffer + length, buffer_len - length, &type, &id, &dataIndex, &dataLen)))
    {
        print_indent(stream, indent);
        plat_fprintf(stream, "{\r\n");
        print_indent(stream, indent+1);
        plat_fprintf(stream, "ID: %d", id);

        plat_fprintf(stream, " type: ");
        switch (type)
        {
        case LWM2M_TYPE_OBJECT_INSTANCE:
            plat_fprintf(stream, "Object Instance");
            break;
        case LWM2M_TYPE_MULTIPLE_RESOURCE:
            plat_fprintf(stream, "Multiple Instances");
            break;
        case LWM2M_TYPE_OPAQUE:
            plat_fprintf(stream, "Resource Value");
            break;
        default:
            plat_printf("unknown (%d)", (int)type);
            break;
        }
        plat_fprintf(stream, "\n");

        print_indent(stream, indent+1);
        plat_fprintf(stream, "{\n");
        if (type == LWM2M_TYPE_OBJECT_INSTANCE || type == LWM2M_TYPE_MULTIPLE_RESOURCE)
        {
            output_tlv(stream, buffer + length + dataIndex, dataLen, indent+1);
        }
        else
        {
            int64_t intValue;
            double floatValue;
            uint8_t tmp;

            print_indent(stream, indent+2);
            plat_fprintf(stream, "data (%ld bytes):\r\n", dataLen);
            output_buffer(stream, (uint8_t*)buffer + length + dataIndex, dataLen, indent+2);

            tmp = buffer[length + dataIndex + dataLen];
            buffer[length + dataIndex + dataLen] = 0;
            if (0 < plat_sscanf((const char *)buffer + length + dataIndex, "%"PRId64, &intValue))
            {
                print_indent(stream, indent+2);
                plat_fprintf(stream, "data as Integer: %" PRId64 "\r\n", intValue);
            }
            if (0 < plat_sscanf((const char*)buffer + length + dataIndex, "%lg", &floatValue))
            {
                print_indent(stream, indent+2);
                plat_fprintf(stream, "data as Float: %.16g\r\n", floatValue);
            }
            buffer[length + dataIndex + dataLen] = tmp;
        }
        print_indent(stream, indent+1);
        plat_fprintf(stream, "}\r\n");
        length += result;
        print_indent(stream, indent);
        plat_fprintf(stream, "}\r\n");
    }
}

void output_data(PLAT_FILE * stream,
                 lwm2m_media_type_t format,
                 uint8_t * data,
                 int dataLength,
                 int indent)
{
    int i;

    print_indent(stream, indent);
    plat_fprintf(stream, "%d bytes received of type ", dataLength);

    switch (format)
    {
    case LWM2M_CONTENT_TEXT:
        plat_fprintf(stream, "text/plain:\r\n");
        output_buffer(stream, data, dataLength, indent);
        break;

    case LWM2M_CONTENT_OPAQUE:
        plat_fprintf(stream, "application/octet-stream:\r\n");
        output_buffer(stream, data, dataLength, indent);
        break;

    case LWM2M_CONTENT_TLV:
        plat_fprintf(stream, "application/vnd.oma.lwm2m+tlv:\r\n");
        output_tlv(stream, data, dataLength, indent);
        break;

    case LWM2M_CONTENT_JSON:
        plat_fprintf(stream, "application/vnd.oma.lwm2m+json:\r\n");
        print_indent(stream, indent);
        for (i = 0 ; i < dataLength ; i++)
        {
            plat_fprintf(stream, "%c", data[i]);
        }
        plat_fprintf(stream, "\n");
        break;

    case LWM2M_CONTENT_LINK:
        plat_fprintf(stream, "application/link-format:\r\n");
        print_indent(stream, indent);
        for (i = 0 ; i < dataLength ; i++)
        {
            plat_fprintf(stream, "%c", data[i]);
        }
        plat_fprintf(stream, "\n");
        break;

    default:
        plat_fprintf(stream, "Unknown (%d):\r\n", format);
        output_buffer(stream, data, dataLength, indent);
        break;
    }
}

void dump_tlv(PLAT_FILE * stream,
              int size,
              lwm2m_data_t * dataP,
              int indent)
{
    int i;

    for(i= 0 ; i < size ; i++)
    {
        print_indent(stream, indent);
        plat_fprintf(stream, "{\r\n");
        print_indent(stream, indent+1);
        plat_fprintf(stream, "id: %d\r\n", dataP[i].id);

        print_indent(stream, indent+1);
        plat_fprintf(stream, "type: ");
        switch (dataP[i].type)
        {
        case LWM2M_TYPE_OBJECT:
            plat_fprintf(stream, "LWM2M_TYPE_OBJECT\r\n");
            dump_tlv(stream, dataP[i].value.asChildren.count, dataP[i].value.asChildren.array, indent + 1);
            break;
        case LWM2M_TYPE_OBJECT_INSTANCE:
            plat_fprintf(stream, "LWM2M_TYPE_OBJECT_INSTANCE\r\n");
            dump_tlv(stream, dataP[i].value.asChildren.count, dataP[i].value.asChildren.array, indent + 1);
            break;
        case LWM2M_TYPE_MULTIPLE_RESOURCE:
            plat_fprintf(stream, "LWM2M_TYPE_MULTIPLE_RESOURCE\r\n");
            dump_tlv(stream, dataP[i].value.asChildren.count, dataP[i].value.asChildren.array, indent + 1);
            break;
        case LWM2M_TYPE_UNDEFINED:
            plat_fprintf(stream, "LWM2M_TYPE_UNDEFINED\r\n");
            break;
        case LWM2M_TYPE_STRING:
            plat_fprintf(stream, "LWM2M_TYPE_STRING\r\n");
            print_indent(stream, indent + 1);
            plat_fprintf(stream, "\"%.*s\"\r\n", (int)dataP[i].value.asBuffer.length, dataP[i].value.asBuffer.buffer);
            break;
        case LWM2M_TYPE_OPAQUE:
            plat_fprintf(stream, "LWM2M_TYPE_OPAQUE\r\n");
            output_buffer(stream, dataP[i].value.asBuffer.buffer, dataP[i].value.asBuffer.length, indent + 1);
            break;
        case LWM2M_TYPE_INTEGER:
            plat_fprintf(stream, "LWM2M_TYPE_INTEGER: ");
            print_indent(stream, indent + 1);
            plat_fprintf(stream, "%" PRId64, dataP[i].value.asInteger);
            plat_fprintf(stream, "\r\n");
            break;
        case LWM2M_TYPE_FLOAT:
            plat_fprintf(stream, "LWM2M_TYPE_FLOAT: ");
            print_indent(stream, indent + 1);
            plat_fprintf(stream, "%" PRId64, dataP[i].value.asInteger);
            plat_fprintf(stream, "\r\n");
            break;
        case LWM2M_TYPE_BOOLEAN:
            plat_fprintf(stream, "LWM2M_TYPE_BOOLEAN: ");
            plat_fprintf(stream, "%s", dataP[i].value.asBoolean ? "true" : "false");
            plat_fprintf(stream, "\r\n");
            break;
        case LWM2M_TYPE_OBJECT_LINK:
            plat_fprintf(stream, "LWM2M_TYPE_OBJECT_LINK\r\n");
            break;
        default:
            plat_fprintf(stream, "unknown (%d)\r\n", (int)dataP[i].type);
            break;
        }
        print_indent(stream, indent);
        plat_fprintf(stream, "}\r\n");
    }
}

#define CODE_TO_STRING(X)   case X : return #X

static const char* prv_status_to_string(int status)
{
    switch(status)
    {
    CODE_TO_STRING(COAP_NO_ERROR);
    CODE_TO_STRING(COAP_IGNORE);
    CODE_TO_STRING(COAP_201_CREATED);
    CODE_TO_STRING(COAP_202_DELETED);
    CODE_TO_STRING(COAP_204_CHANGED);
    CODE_TO_STRING(COAP_205_CONTENT);
    CODE_TO_STRING(COAP_400_BAD_REQUEST);
    CODE_TO_STRING(COAP_401_UNAUTHORIZED);
    CODE_TO_STRING(COAP_404_NOT_FOUND);
    CODE_TO_STRING(COAP_405_METHOD_NOT_ALLOWED);
    CODE_TO_STRING(COAP_406_NOT_ACCEPTABLE);
    CODE_TO_STRING(COAP_500_INTERNAL_SERVER_ERROR);
    CODE_TO_STRING(COAP_501_NOT_IMPLEMENTED);
    CODE_TO_STRING(COAP_503_SERVICE_UNAVAILABLE);
    default: return "";
    }
}

void print_status(PLAT_FILE * stream,
                  uint8_t status)
{
    plat_fprintf(stream, "%d.%02d (%s)", (status&0xE0)>>5, status&0x1F, prv_status_to_string(status));
}

/**********************************************************
* Base64 decoding function
*
* WARNING: Bugged for input strings with length < 4
*
*/

#define PRV_B64_PADDING '='

static uint8_t prv_b64Revert(uint8_t value)
{
    if (value >= 'A' && value <= 'Z')
    {
        return (value - 'A');
    }
    if (value >= 'a' && value <= 'z')
    {
        return (26 + value - 'a');
    }
    if (value >= '0' && value <= '9')
    {
        return (52 + value - '0');
    }
    switch (value)
    {
    case '+':
        return 62;
    case '/':
        return 63;
    default:
        return 0;
    }
}

static void prv_decodeBlock(uint8_t input[4],
                            uint8_t output[3])
{
    uint8_t tmp[4];
    int i;

    memset(output, 0, 3);

    for (i = 0; i < 4; i++)
    {
        tmp[i] = prv_b64Revert(input[i]);
    }

    output[0] = (tmp[0] << 2) | (tmp[1] >> 4);
    output[1] = (tmp[1] << 4) | (tmp[2] >> 2);
    output[2] = (tmp[2] << 6) | tmp[3];
}

SIZE_T base64_decode(uint8_t * dataP,
                     SIZE_T dataLen,
                     uint8_t ** bufferP)
{
    SIZE_T data_index;
    SIZE_T result_index;
    SIZE_T result_len;
    
    if (dataLen % 4) return 0;
    
    result_len = (dataLen >> 2) * 3;
    *bufferP = (uint8_t *)lwm2m_malloc(result_len);
    if (NULL == *bufferP) return 0;
    memset(*bufferP, 0, result_len);
    
    // remove padding
    while (dataP[dataLen - 1] == PRV_B64_PADDING)
    {
        dataLen--;
    }
    
    data_index = 0;
    result_index = 0;
    while (data_index < dataLen)
    {
        prv_decodeBlock(dataP + data_index, *bufferP + result_index);
        data_index += 4;
        result_index += 3;
    }
    switch (data_index - dataLen)
    {
    case 0:
        break;
    case 2:
    {
        uint8_t tmp[2];

        tmp[0] = prv_b64Revert(dataP[dataLen - 2]);
        tmp[1] = prv_b64Revert(dataP[dataLen - 1]);

        *bufferP[result_index - 3] = (tmp[0] << 2) | (tmp[1] >> 4);
        *bufferP[result_index - 2] = (tmp[1] << 4);
        result_len -= 2;
    }
    break;
    case 3:
    {
        uint8_t tmp[3];

        tmp[0] = prv_b64Revert(dataP[dataLen - 3]);
        tmp[1] = prv_b64Revert(dataP[dataLen - 2]);
        tmp[2] = prv_b64Revert(dataP[dataLen - 1]);

        *bufferP[result_index - 3] = (tmp[0] << 2) | (tmp[1] >> 4);
        *bufferP[result_index - 2] = (tmp[1] << 4) | (tmp[2] >> 2);
        *bufferP[result_index - 1] = (tmp[2] << 6);
        result_len -= 1;
    }
    break;
    default:
        // error
        lwm2m_free(*bufferP);
        *bufferP = NULL;
        result_len = 0;
        break;
    }

    return result_len;
}

