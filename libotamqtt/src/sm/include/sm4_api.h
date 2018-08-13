/***************************************************************************
* Copyright(C),2012-2018, CMIoT
* File name: des.h
* Description : DES Crypto Implement
* Author: xixi
* Version: v1.0
* Date:  2018-6-7
* History:
*       Date        Version     Author       Notes
*     2018-6-7        v1.0      xixi        create files
**************************************************************************/
#ifndef __SM4_API_H__
#define __SM4_API_H__

#include <stdint.h>
/**
 * @fn SM4_Encrypt
 * @brief SM4 ECB加密，ISO9797-1 method2 填充，不满16字节整数倍
 * @param[in]   key     加密密钥 长度为16字节
 * @param[in]   in      原文缓存
 * @param[in]   inLen   待加密的原文长度
 * @param[out]  out     输出密文缓存，缓存空间长度为 inLen % 16 ? inLen : inLen+16
 * @return      密文长度
 * @note 不满16字节整数倍填充0x80, 0x00 ... 满16字节依然填充 0x80 00 00 00 00 00 00 00
**/
uint16_t SM4_Encrypt(const uint8_t *key, uint8_t *in, uint16_t inLen, uint8_t *out);
/**
 * @fn SM4_Decrypt
 * @brief SM4 ECB解密，ISO9797-1 method2 填充，
 * @param[in]   key     加密密钥 长度为16字节
 * @param[in]   in      密文缓存
 * @param[in]   inLen   待解密的密文长度 应该为16字节整数倍
 * @param[out]  out     输出原文缓存
 * @return      输出原文长度，若为0，则表明解密失败，密文非法
 * @note
**/
uint16_t SM4_Decrypt(const uint8_t *key, uint8_t *in, uint16_t inLen, uint8_t *out);

#endif

