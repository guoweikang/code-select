#ifndef ONENET_MQTT_BUFFER_H
#define ONENET_MQTT_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "plat_stdint.h"
//#include "mqtt_config.h"

struct MqttExtent {
    uint32_t len;
    char *payload;

    struct MqttExtent *next;
};

struct MqttBuffer {
    struct MqttExtent *first_ext;
    struct MqttExtent *last_ext;
    uint32_t available_bytes;

    char **allocations;
    char *first_available;
    uint32_t alloc_count;
    uint32_t alloc_max_count;
    uint32_t buffered_bytes;
};

/**
 * 初始化缓冲区，缓冲区对象在使用完后，必须�?@see MqttBuffer_Destroy销�? * @param buf 被初始化的缓冲区对象
 */
void MqttBuffer_Init(struct MqttBuffer *buf);
/**
 * 销毁缓冲区对象
 * @param buf 被销毁的缓冲区对�? */
void MqttBuffer_Destroy(struct MqttBuffer *buf);
/**
 * 重置(清空)缓冲区对�? * @param buf 被重�?清空)的缓冲区对象
 */
void MqttBuffer_Reset(struct MqttBuffer *buf);
/**
 * 分配一块连续的内存
 * @param buf 用于分配连续缓冲区的缓冲区对�? * @param size 将要分配的缓冲区大小（字节数�? */
struct MqttExtent *MqttBuffer_AllocExtent(struct MqttBuffer *buf, uint32_t size);
/**
 * 将一个数据块添加到缓冲区的末�? * @param buf 存储数据块的缓冲区对�? * @param payload 数据块的首地址
 * @param size 数据块的大小（字节数�? * @param own �?时，数据块将被拷贝到缓冲区，�?时，缓冲区仅保存其引�? * @return 成功则返�?MQTTERR_NOERROR
 * @remark 当own�?时，必须保证payload在buf未被销毁前一直有�? */
int MqttBuffer_Append(struct MqttBuffer *buf, char *payload, uint32_t size, int own);
/**
 * 将一块连续的内存添加到缓冲区的末�? * @param buf 存储数据块的缓冲区对�? * @param ext 将要加入缓冲区的数据�? * @remark 若ext不是buf分配的，则需保证ext在buf被销毁前一直有�? */
void MqttBuffer_AppendExtent(struct MqttBuffer *buf, struct MqttExtent *ext);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ONENET_MQTT_BUFFER_H
