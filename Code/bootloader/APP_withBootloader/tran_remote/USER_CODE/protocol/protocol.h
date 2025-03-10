/*
 * protocol.h
 *
 *  Created on: 2022年7月29日
 *      Author: hasee
 */

#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_


/*

该通信协议仅支持多方通信，携带发送者和目的接收者，ipv1(单字节地址)
所有api不会对缓冲区长度进行判断,需要使用者自行分配好足够的缓冲区避免内存溢出\错误

                生物电子创新基地
作者: 黄涛(865355983@qq.com),杨象添(1546902365@qq.com)
版本: V2.7
日期: 2022/10/13
更新:
    修复decode过程中,解析缓冲区长度 - head长度,无符号整形负数导致下溢,得到错误的head检索长度从而引发内存错误
    修复错误的函数名
    decode_buf 函数 新增参数  decode_all_target,调用时推荐正确设置好地址并设置该参数为0,设置参数为1可以无需对比taget_addr解析出协议,
    多机共用端口通信时需要为0否则可能会解析到不属于本地址的包
    2022/10/13更新日志：
    convert 转变为 create_packet
    respond_convert 转变为 respond_packet
    decode_buf  转变为 unpack_buf
    decode_uint8 转变为 unpack_buf_from_back
    updata_buf 转变为 send_to_processes_buffer
*/

#include <stdio.h>
#include <string.h>
#include "val_data.h"


#ifdef __linux__
    #ifndef HAVE_PRINTF
    #define HAVE_PRINTF
    #endif
    #include <malloc.h>

#endif
// typedef char int8_t;
// typedef short int16_t;
// typedef int int32_t;

#ifndef UINT_TYPE
#define UINT_TYPE
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#endif
// #define DEBUG
// #define BUF_SIZE 1024
#define BUF_SIZE 1024
//建议 BUF_SIZE >= 128,推荐 1024 , 根据接收数据大小以及频率的情况来设定


typedef struct Head HeadStr;
typedef struct DataTail DataTailStr;
typedef struct Protocol ProtocolStr;
typedef struct Receive ReceiveStr;
typedef struct ReceiveProtocol ReceiveProtocolStr;

/*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |          head_flags           |  source_addr  |  target_addr  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           pack_idx            |      data_size      | pack_cls|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           val_sum             |           rear_flags          |
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    |                             ....                              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |        data_val_sum           |       data_rear_flags         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
struct Head
{
    uint16_t head_flags;//头字节
    uint8_t source_addr; //源地址 0～255    // 255 为广播地址
    uint8_t target_addr; //目的地址 0～255   // 255 为广播地址

    uint16_t pack_idx; //包序号，发送者按发送顺序递增,接受者可以计算出丢包情况
    uint16_t pack_cls:5; //数据包类型 0～31,用于给用户提供区分包类型
    uint16_t data_size:11; //数据包长度 0～2047

    uint16_t val_sum; //首部校验和，不含头尾
    uint16_t rear_flags; //
};
struct DataTail
{
    uint16_t data_val_sum; //数据校验和，不含尾
    uint16_t data_rear_flags; //数据的尾部
};
struct Protocol{
    struct Head head; //协议的头结构
    uint16_t head_size; //头结构长度
    struct DataTail data_tail; //协议的尾结构
    uint16_t data_tail_size; //协议的尾结构长度
};
struct ReceiveProtocol{
    struct Head *head;  //接受到的head
    struct DataTail *data_tail; //接收到的data_tail
};
struct Receive{
    //本机协议
    ProtocolStr *myprotocol; //接受者协议
    //解析出来的协议
    ReceiveProtocolStr decode_protocol;
    uint16_t pack_idx; //记录当前的包id
    //解析信息
    uint16_t loss; //记录当前帧和上一帧之间的丢包数
    //解析缓冲区
    uint8_t buf[BUF_SIZE];  //将待解析的数据重首地址开始放置
    uint16_t buf_size; //输入的数据长度(待解码的数据长度)
    uint16_t handle_index; //记录buf已被解码的长度
    uint16_t decode_data_index; //解码出有效数据的偏移值
    uint16_t decode_data_size; //解码出来的数据长度
};

/*
校验从head的下一个到 val_sum的前一个
*/
uint16_t get_head_val(HeadStr* head);
/*
绑定接收结构体
*/
int bind_receive(ReceiveStr *receive,ProtocolStr* protocol);
/*
初始化协议结构体
*/
int init_protocol(ProtocolStr* protocol,uint16_t head_flags,uint16_t rear_flags,uint16_t data_rear_flags,uint8_t source_addr);
/*
使用printf打印string 和 head协议信息
*/
void printf_head(const char *string,HeadStr* head);
/*
封装协议到发送缓冲区
pack_buf:数据包缓冲区,将要发送的数据,使用head和tail将数据包裹拷贝到该缓冲区(从首地址开始)并返回数据长度
send_data：要发送数据
send_data：数据长度
protocol： 协议信息
target_addr：目标地址
pack_cls：包类型(0～31，用户扩展使用，用于为用户提供辨别不同的包)
*/
// int convert(uint8_t send_buf[BUF_SIZE],uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t target_addr,uint8_t pack_cls);
// convert 转变为 create_packet
int create_packet(uint8_t *pack_buf,uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t target_addr,uint8_t pack_cls);
/*
convert的扩展,将ReceiveStr传入将自动设定taget_addr为发送者地址,
用于协议收到请求的应答转换
*/
// respond_convert 转变为 respond_packet
// int respond_convert(uint8_t send_buf[BUF_SIZE],uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,ReceiveStr *receive,uint8_t pack_cls);
int respond_packet(uint8_t *pack_buf,uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,ReceiveStr *receive,uint8_t pack_cls);
/*
将接收到的数据复制到解析缓冲区，以调用decode进行解析
data：接收缓冲区
data_size：接收的数据长度
*/
//updata_buf 转变为 send_to_processes_buffer
int send_to_processes_buffer(ReceiveStr *receive_protocol,uint8_t *data,int data_size);

/*
由buf由前正向解析协议
解析协议信息
decode_all_target 1: 捕获所有目标地址的协议
返回值： 解析的有效数据长度
0 未解析出协议头或数据长度为0
*/
//decode_buf  转变为 unpack_buf
int unpack_buf(ReceiveStr *receive_protocol,int decode_all_target);
/*
    由buf从后向前解析协议
    适用于数据接包不完整，破损包（不完整包）在缓冲区内复用解析的情况，
    目前的缺点：每次需要重新打入全部缓冲数据，存在历史数据重复拷贝的情况
    send_to_processes_buffer
*/
//decode_uint8 转变为 unpack_buf_from_back
int unpack_buf_from_back(ReceiveStr *receive_protocol,uint8_t *buf,int buf_size,int decode_all_target);
/*
拷贝已经解析数据
buf         ：预先申请好的数据缓冲区（需要保证buf长度 > 解析出的数据长度）
返回值：解析出的数据长度
*/
int copy_receive_data(ReceiveStr *receive,uint8_t *buf);
/*
拷贝已经解析好的协议以及数据
new_protocol：预先申请好的协议头地址
buf         ：预先申请好的数据缓冲区（需要保证buf长度 > 解析出的数据长度）
返回值：解析出的数据长度
*/
int copy_receive_protocol(ReceiveStr *receive,ProtocolStr *new_protocol,uint8_t *buf);
/*
更新协议的源地址(发送地址)
*/
void set_source_addr(ProtocolStr* protocol,uint8_t source_addr);
/*
更新协议的目的地址(接收地址)
*/
void set_target_addr(ProtocolStr* protocol,uint8_t target_addr);
/*
获取解析的数据包的数据首地址(不包含头)
*/
uint8_t *get_data(ReceiveStr *receive);
/*
获取解析的数据包的数据首地址在buf的偏移地址(不包含头)
*/
uint16_t get_data_index(ReceiveStr *receive);
/*
获取解析的数据包内的数据长度(不包含头尾)
*/
uint16_t get_data_size(ReceiveStr *receive);
/*
获取解析的数据包类型
*/
uint8_t get_pack_cls(ReceiveStr *receive);
/*
获取解析的数据包id
*/
uint16_t get_pack_idx(ReceiveStr *receive);
/*
获取解析的数据包与上一帧数据包之间的丢包数量
*/
uint16_t get_loss(ReceiveStr *receive);
/*
获取解析缓冲区的数据长度
*/
uint16_t get_buf_size(ReceiveStr *receive);
/*
获取解析缓冲区地址
*/
uint8_t *get_buf(ReceiveStr *receive);
/*
获取协议的源地址
*/
uint8_t get_protocol_source_addr(ProtocolStr *protocol);
/*
获取协议的目的地址
*/
uint8_t get_protocol_target_addr(ProtocolStr *protocol);
/*
获取接收到的协议包源地址
*/
uint8_t get_decode_source_addr(ReceiveStr *receive);
/*
获取接收到的协议包目的地址
*/
uint8_t get_decode_target_addr(ReceiveStr *receive);

#ifdef __linux__
/*
复用动态内存申请,buf不能为野指针,只能为NULL or 已分配的空间
根据buf_size判断  若buf_size  < target_buf_size,会释放原来的空间重新申请 element_size*target_buf_size的字节.
否则 将会复用buf
*/
void *repeat_malloc(void *buf,uint32_t element_size,uint32_t buf_size,uint32_t target_buf_size);
#endif


#endif /* PROTOCOL_H_ */


