/***************************************************************************
* Copyright(C),2012-2018, CMIoT 
* File name: sm3.h
* Description : hash message digest SM3
* Author: xixi  
* Version: v1.0 
* Date:  2018-6-7
* History: 
*       Date       	Version    	Author       Notes
*    	2018-6-7				v1.0 			xixi			create files
**************************************************************************/
#ifndef __SM3_H__
#define __SM3_H__


/**
 * \brief          SM3 context structure
 */
typedef struct
{
	unsigned long total[2];     /*!< number of bytes processed  */
	unsigned long state[8];     /*!< intermediate digest state  */
	unsigned char buffer[64];   /*!< data block being processed */
}SM3_CXT;

#define SM3_CXT_SIZE			sizeof(SM3_CXT)
#define SM3_LENGTH				32

/**
 * @fn SM3_Init
 * @brief SM3 context setup
 * @param[in]		param			输入参数
 * @param[out]		param			输出数据
 * @return				0，执行成功，非0，失败，详见 @ref            RetCode.h
 * @note 
**/
void SM3_Init( SM3_CXT *ctx );

/**
 * \brief          SM3 process buffer
 *
 * \param ctx      SM3 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void SM3_Update( SM3_CXT *ctx, unsigned char *input, int ilen );

/**
 * \brief          SM3 final digest
 *
 * \param ctx      SM3 context
 */
void SM3_Final( SM3_CXT *ctx, unsigned char output[32] );

/**
 * \brief          Output = SM3( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SM3 checksum result
 */
void SM3( unsigned char *input, int ilen,
          unsigned char output[32]);


#endif /* sm3.h */

