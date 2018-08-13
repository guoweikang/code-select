#include "sm4.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint16_t SM4_Encrypt(const uint8_t *key, uint8_t *in, uint16_t inLen, uint8_t *out)
{
  uint8_t data[16] = {0};
  uint16_t index = 0;
  
  
  SM4_CTX ctx;
  sm4_setkey_enc(&ctx, (uint8_t *)key);
  
  while(inLen >= 16)
  {
  
    sm4_crypt_ecb(&ctx, SM4_ENCRYPT, 16, in + index, out + index);
    inLen -= 16;
    index += 16;
  }
  
  memmove(data, in + index, inLen);
  data[inLen] = 0x80;
  
  memset(data + inLen + 1, 0, 16 - inLen - 1);
  sm4_crypt_ecb(&ctx, SM4_ENCRYPT, 16, data, out + index);
  index += 16;
  
  return index;
  
}

uint16_t SM4_Decrypt(const uint8_t *key, uint8_t *in, uint16_t inLen, uint8_t *out)
{
  uint8_t data[16] = {0};
  uint16_t index = 0;
  int8_t i = 15;

	SM4_CTX ctx;
  sm4_setkey_dec(&ctx, (uint8_t *)key);
  
  if((inLen & 15) != 0)
  {
    return 0;   //error
  }
  
  for(index = 0; index < inLen - 16; index += 16)
  {
  	sm4_crypt_ecb(&ctx, SM4_DECRYPT, 16, in + index, out + index);
  }
  
  index = inLen - 16;
	sm4_crypt_ecb(&ctx, SM4_DECRYPT, 16, in + index, data);
  
  //去填充
  for(; i >= 0; i --)
  {
    if(data[i] == 0)
    {
      continue;
    }
    else if(data[i] == 0x80)
    {
      break;
    }
    else
    {
      return 0;
    }
  }
  
  if(i < 0)
  {
    return 0;
  }
  
  memmove(out + index, data, i);
  return index + i;
  
  
}

