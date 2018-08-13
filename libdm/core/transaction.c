/** 
	* @file 		transaction.c 
	* @brief		系统库包装层 包装 time
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	


#include "internals.h"


/*
 * Modulo mask (+1 and +0.5 for rounding) for a random number to get the tick number for the random
 * retransmission time between COAP_RESPONSE_TIMEOUT and COAP_RESPONSE_TIMEOUT*COAP_RESPONSE_RANDOM_FACTOR.
 */
#define COAP_RESPONSE_TIMEOUT_TICKS         (CLOCK_SECOND * COAP_RESPONSE_TIMEOUT)
#define COAP_RESPONSE_TIMEOUT_BACKOFF_MASK  ((CLOCK_SECOND * COAP_RESPONSE_TIMEOUT * (COAP_RESPONSE_RANDOM_FACTOR - 1)) + 1.5)

static int prv_checkFinished(lwm2m_transaction_t * transacP,
                             coap_packet_t * receivedMessage)
{
    int len;
    const uint8_t* token;
    coap_packet_t * transactionMessage = transacP->message;

    if (COAP_DELETE < transactionMessage->code)
    {
        // response
        return transacP->ack_received ? 1 : 0;
    }
    if (!IS_OPTION(transactionMessage, COAP_OPTION_TOKEN))
    {
        // request without token
        return transacP->ack_received ? 1 : 0;
    }

    len = coap_get_header_token(receivedMessage, &token);
    if (transactionMessage->token_len == len)
    {
        if (memcmp(transactionMessage->token, token, len)==0) return 1;
    }

    return 0;
}

lwm2m_transaction_t * transaction_new(void * sessionH,
                                      coap_method_t method,
                                      char * altPath,
                                      lwm2m_uri_t * uriP,
                                      uint16_t mID,
                                      uint8_t token_len,
                                      uint8_t* token)
{
    lwm2m_transaction_t * transacP;
    int result;

    LOG_ARG("method: %d, altPath: \"%s\", mID: %d, token_len: %d",
            method, altPath, mID, token_len);
    LOG_URI(uriP);

    // no transactions without peer
    if (NULL == sessionH) return NULL;

    transacP = (lwm2m_transaction_t *)lwm2m_malloc(sizeof(lwm2m_transaction_t));

    if (NULL == transacP) return NULL;
    memset(transacP, 0, sizeof(lwm2m_transaction_t));

    transacP->message = lwm2m_malloc(sizeof(coap_packet_t));
    if (NULL == transacP->message) goto error;

    coap_init_message(transacP->message, COAP_TYPE_CON, method, mID);

    transacP->peerH = sessionH;

    transacP->mID = mID;

    if (altPath != NULL)
    {
        // TODO: Support multi-segment alternative path
        coap_set_header_uri_path_segment(transacP->message, altPath + 1);
    }
    if (NULL != uriP)
    {
        char stringID[LWM2M_STRING_ID_MAX_LEN];

        result = utils_intToText(uriP->objectId, (uint8_t*)stringID, LWM2M_STRING_ID_MAX_LEN);
        if (result == 0) goto error;
        stringID[result] = 0;
        coap_set_header_uri_path_segment(transacP->message, stringID);

        if (LWM2M_URI_IS_SET_INSTANCE(uriP))
        {
            result = utils_intToText(uriP->instanceId, (uint8_t*)stringID, LWM2M_STRING_ID_MAX_LEN);
            if (result == 0) goto error;
            stringID[result] = 0;
            coap_set_header_uri_path_segment(transacP->message, stringID);
        }
        else
        {
            if (LWM2M_URI_IS_SET_RESOURCE(uriP))
            {
                coap_set_header_uri_path_segment(transacP->message, NULL);
            }
        }
        if (LWM2M_URI_IS_SET_RESOURCE(uriP))
        {
            result = utils_intToText(uriP->resourceId, (uint8_t*)stringID, LWM2M_STRING_ID_MAX_LEN);
            if (result == 0) goto error;
            stringID[result] = 0;
            coap_set_header_uri_path_segment(transacP->message, stringID);
        }
    }
    if (0 < token_len)
    {
        if (NULL != token)
        {
            coap_set_header_token(transacP->message, token, token_len);
        }
        else {
            // generate a token
            uint8_t temp_token[COAP_TOKEN_LEN];
            TIME_T tv_sec = lwm2m_gettime();

            // initialize first 6 bytes, leave the last 2 random
            temp_token[0] = mID;
            temp_token[1] = mID >> 8;
            temp_token[2] = tv_sec;
            temp_token[3] = tv_sec >> 8;
            temp_token[4] = tv_sec >> 16;
            temp_token[5] = tv_sec >> 24;
            // use just the provided amount of bytes
            coap_set_header_token(transacP->message, temp_token, token_len);
        }
    }

    LOG("Exiting on success");
    return transacP;

error:
    LOG("Exiting on failure");
    lwm2m_free(transacP);
    return NULL;
}

void transaction_free(lwm2m_transaction_t * transacP)
{
    LOG("Entering");
    if (transacP->message)
    {
       coap_free_header(transacP->message);
       lwm2m_free(transacP->message);
    }

    if (transacP->buffer) lwm2m_free(transacP->buffer);
    lwm2m_free(transacP);
}

void transaction_remove(lwm2m_context_t * contextP,
                        lwm2m_transaction_t * transacP)
{
    LOG("Entering");
    contextP->transactionList = (lwm2m_transaction_t *) LWM2M_LIST_RM(contextP->transactionList, transacP->mID, NULL);
    transaction_free(transacP);
}

bool transaction_handleResponse(lwm2m_context_t * contextP,
                                 void * fromSessionH,
                                 coap_packet_t * message,
                                 coap_packet_t * response)
{
    bool found = false;
    bool reset = false;
    lwm2m_transaction_t * transacP;

    LOG("Entering");
    transacP = contextP->transactionList;

    while (NULL != transacP)
    {
        if (lwm2m_session_is_equal(fromSessionH, transacP->peerH, contextP->userData) == true)
        {
            if (!transacP->ack_received)
            {
                if ((COAP_TYPE_ACK == message->type) || (COAP_TYPE_RST == message->type))
                {
                    if (transacP->mID == message->mid)
	                {
    	                found = true;
        	            transacP->ack_received = true;
            	        reset = COAP_TYPE_RST == message->type;
            	    }
                }
            }

            if (reset || prv_checkFinished(transacP, message))
            {
                // HACK: If a message is sent from the monitor callback,
                // it will arrive before the registration ACK.
                // So we resend transaction that were denied for authentication reason.
                if (!reset)
                {
                    if (COAP_TYPE_CON == message->type && NULL != response)
                    {
                        coap_init_message(response, COAP_TYPE_ACK, 0, message->mid);
                        message_send(contextP, response, fromSessionH);
                    }
                
	                if ((COAP_401_UNAUTHORIZED == message->code) && (COAP_MAX_RETRANSMIT > transacP->retrans_counter))
    	            {
        	            transacP->ack_received = false;
            	        transacP->retrans_time += COAP_RESPONSE_TIMEOUT;
                	    return true;
                	}
				}       
                if (transacP->callback != NULL)
                {
                    transacP->callback(transacP, message);
                }
                transaction_remove(contextP, transacP);
                return true;
            }
            // if we found our guy, exit
            if (found)
            {
                TIME_T tv_sec = lwm2m_gettime();
                if (0 <= tv_sec)
                {
                    transacP->retrans_time = tv_sec;
                }
                if (transacP->response_timeout)
                {
                    transacP->retrans_time += transacP->response_timeout;
                }
                else
                {
                    transacP->retrans_time += COAP_RESPONSE_TIMEOUT * transacP->retrans_counter;
                }
                return true;
            }
        }

        transacP = transacP->next;
    }
    return false;
}

int transaction_send(lwm2m_context_t * contextP,
                     lwm2m_transaction_t * transacP)
{
    bool maxRetriesReached = false;

    LOG("Entering");
    if (transacP->buffer == NULL)
    {
        transacP->buffer_len = coap_serialize_get_size(transacP->message);
        if (transacP->buffer_len == 0)
        {
           transaction_remove(contextP, transacP);
           return COAP_500_INTERNAL_SERVER_ERROR;
        }

        transacP->buffer = (uint8_t*)lwm2m_malloc(transacP->buffer_len);
        if (transacP->buffer == NULL)
        {
           transaction_remove(contextP, transacP);
           return COAP_500_INTERNAL_SERVER_ERROR;
        }

        transacP->buffer_len = coap_serialize_message(transacP->message, transacP->buffer);
        if (transacP->buffer_len == 0)
        {
            lwm2m_free(transacP->buffer);
            transacP->buffer = NULL;
            transaction_remove(contextP, transacP);
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (!transacP->ack_received)
    {
        long unsigned timeout;

        if (0 == transacP->retrans_counter)
        {
            TIME_T tv_sec = lwm2m_gettime();
            if (0 <= tv_sec)
            {
                transacP->retrans_time = tv_sec + COAP_RESPONSE_TIMEOUT;
                transacP->retrans_counter = 1;
                timeout = 0;
            }
            else
            {
                maxRetriesReached = true;
            }
        }
        else
        {
            timeout = COAP_RESPONSE_TIMEOUT << (transacP->retrans_counter - 1);
        }

        if (COAP_MAX_RETRANSMIT + 1 >= transacP->retrans_counter)
        {
            (void)lwm2m_buffer_send(transacP->peerH, transacP->buffer, transacP->buffer_len, contextP->userData);

            transacP->retrans_time += timeout;
            transacP->retrans_counter += 1;
        }
        else
        {
            maxRetriesReached = true;
        }
    }

    if (transacP->ack_received || maxRetriesReached)
    {
        if (transacP->callback)
        {
            transacP->callback(transacP, NULL);
        }
        transaction_remove(contextP, transacP);
        return -1;
    }

    return 0;
}

void transaction_step(lwm2m_context_t * contextP,
                      TIME_T currentTime,
                      TIME_T * timeoutP)
{
    lwm2m_transaction_t * transacP;

    LOG("Entering");
    transacP = contextP->transactionList;
    while (transacP != NULL)
    {
        // transaction_send() may remove transaction from the linked list
        lwm2m_transaction_t * nextP = transacP->next;
        int removed = 0;

        if (transacP->retrans_time <= currentTime)
        {
            removed = transaction_send(contextP, transacP);
        }

        if (0 == removed)
        {
            TIME_T interval;

            if (transacP->retrans_time > currentTime)
            {
                interval = transacP->retrans_time - currentTime;
            }
            else
            {
                interval = 1;
            }

            if (*timeoutP > interval)
            {
                *timeoutP = interval;
            }
        }
        else
        {
            *timeoutP = 1;
        }

        transacP = nextP;
    }
}

