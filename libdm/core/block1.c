/** 
	* @file 		block1.c 
	* @brief		
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/4/16 , 第一次创建\n 
*/	


#include "internals.h"

#include "plat_stdlib.h"
#include "plat_string.h"
#include "plat_stdio.h"

// the maximum payload transferred by block1 we accumulate per server
#define MAX_BLOCK1_SIZE 4096

uint8_t coap_block1_handler(lwm2m_block1_data_t ** pBlock1Data,
                            uint16_t mid,
                            uint8_t * buffer,
                            SIZE_T length,
                            uint16_t blockSize,
                            uint32_t blockNum,
                            bool blockMore,
                            uint8_t ** outputBuffer,
                            SIZE_T * outputLength)
{
    lwm2m_block1_data_t * block1Data = *pBlock1Data;;

    // manage new block1 transfer
    if (blockNum == 0)
    {
       // we already have block1 data for this server, clear it
       if (block1Data != NULL)
       {
           lwm2m_free(block1Data->block1buffer);
       }
       else
       {
           block1Data = lwm2m_malloc(sizeof(lwm2m_block1_data_t));
           *pBlock1Data = block1Data;
           if (NULL == block1Data) return COAP_500_INTERNAL_SERVER_ERROR;
       }

       block1Data->block1buffer = lwm2m_malloc(length);
       block1Data->block1bufferSize = length;

       // write new block in buffer
       memcpy(block1Data->block1buffer, buffer, length);
       block1Data->lastmid = mid;
    }
    // manage already started block1 transfer
    else
    {
       if (block1Data == NULL)
       {
           // we never receive the first block
           // TODO should we clean block1 data for this server ?
           return COAP_408_REQ_ENTITY_INCOMPLETE;
       }

       // If this is a retransmission, we already did that.
       if (block1Data->lastmid != mid)
       {
          uint8_t * oldBuffer = block1Data->block1buffer;
          SIZE_T oldSize = block1Data->block1bufferSize;

          if (block1Data->block1bufferSize != blockSize * blockNum)
          {
              // we don't receive block in right order
              // TODO should we clean block1 data for this server ?
              return COAP_408_REQ_ENTITY_INCOMPLETE;
          }

          // is it too large?
          if (block1Data->block1bufferSize + length >= MAX_BLOCK1_SIZE) {
              return COAP_413_ENTITY_TOO_LARGE;
          }
          // re-alloc new buffer
          block1Data->block1bufferSize = oldSize+length;
          block1Data->block1buffer = lwm2m_malloc(block1Data->block1bufferSize);
          if (NULL == block1Data->block1buffer) return COAP_500_INTERNAL_SERVER_ERROR;
          memcpy(block1Data->block1buffer, oldBuffer, oldSize);
          lwm2m_free(oldBuffer);

          // write new block in buffer
          memcpy(block1Data->block1buffer + oldSize, buffer, length);
          block1Data->lastmid = mid;
       }
    }

    if (blockMore)
    {
        *outputLength = -1;
        return COAP_231_CONTINUE;
    }
    else
    {
        // buffer is full, set output parameter
        // we don't free it to be able to send retransmission
        *outputLength = block1Data->block1bufferSize;
        *outputBuffer = block1Data->block1buffer;

        return NO_ERROR;
    }
}

void free_block1_buffer(lwm2m_block1_data_t * block1Data)
{
    if (block1Data != NULL)
    {
        // free block1 buffer
        lwm2m_free(block1Data->block1buffer);
        block1Data->block1bufferSize = 0 ;

        // free current element
        lwm2m_free(block1Data);
    }
}

