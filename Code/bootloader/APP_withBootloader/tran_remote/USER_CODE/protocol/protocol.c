#include "protocol.h"

//宏定义校验函数
#define VAL_FUNCTION        get_val_xor_sum
// #define VAL_FUNCTION     get_val_crc16
// #define VAL_FUNCTION      get_val_sum


uint16_t get_head_val(HeadStr* head){
    uint8_t *p=(uint8_t *)(&(head->head_flags)+1);
    uint8_t *p_end=(uint8_t *)(&(head->val_sum));
    return VAL_FUNCTION(0,p,p_end-p);
}

int bind_receive(ReceiveStr *receive,ProtocolStr* protocol){
    receive->myprotocol=protocol;
    receive->handle_index=0;
    receive->buf_size=0;
    receive->decode_data_index=0;
    receive->loss=0;
    return 0;
}
int init_protocol(ProtocolStr* protocol,uint16_t head_flags,uint16_t rear_flags,uint16_t data_rear_flags,uint8_t source_addr){
    protocol->head.head_flags=head_flags;
    // protocol->head.source_addr=source_addr;
    set_source_addr(protocol,source_addr);
    set_target_addr(protocol,0);
    protocol->head.pack_idx=0;
    protocol->head.pack_cls=0;
    protocol->head.rear_flags=rear_flags;
    protocol->data_tail.data_rear_flags=data_rear_flags;
    protocol->head_size=sizeof(protocol->head);
    protocol->data_tail_size=sizeof(protocol->data_tail);
    // printf("head_size %d  data_tail_size %d \n",protocol->head_size,protocol->data_tail_size);
    return 0;
}
void printf_head(const char *string,HeadStr* head){

#ifdef HAVE_PRINTF
    printf("\n%s",string);
    printf("\
head_flags  %u        \n\
source_addr %u        \n\
target_addr %u        \n\
pack_idx    %u        \n\
pack_cls    %u        \n\
data_size   %u        \n\
val_sum     %u        \n\
rear_flags  %u        \n\
    ",
    head->head_flags,
    head->source_addr,
    head->target_addr,
    head->pack_idx-1,
    head->pack_cls,
    head->data_size,
    head->val_sum,
    head->rear_flags
    );
#endif
}
// int create_packet(uint8_t pack_buf[BUF_SIZE],uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t pack_cls){
// int create_packet(uint8_t *pack_buf,uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t target_addr,uint8_t pack_cls){
//需要保证pack_buf 可容纳整个协议,一般需要预留 发送数据长度+头协议长度12+尾协议长度4
int create_packet(uint8_t *pack_buf,uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t target_addr,uint8_t pack_cls){
    set_target_addr(protocol,target_addr);
    uint8_t *data_buf_start=pack_buf+protocol->head_size;
    // protocol->head.target_addr=target_addr;
    protocol->head.data_size=send_size;
    protocol->head.pack_cls=pack_cls;
    protocol->head.val_sum=get_head_val(&(protocol->head));

    protocol->data_tail.data_val_sum=VAL_FUNCTION(protocol->head.val_sum,send_data,send_size);

    memcpy(pack_buf,&(protocol->head),protocol->head_size); //拷贝head数据到buf
    // if(pack_cls==2){
    //     getchar();
    // }
    memcpy(data_buf_start,send_data,send_size); //拷贝数据包到buf
    memcpy(data_buf_start+send_size,&(protocol->data_tail),protocol->data_tail_size); //拷贝数据尾包到buf
// #ifdef DEBUG
// #endif
    ++(protocol->head.pack_idx);
    return protocol->head_size+send_size+protocol->data_tail_size;
}
// int convert_to_target_addr(uint8_t pack_buf[BUF_SIZE],uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,uint8_t target_addr,uint8_t pack_cls){
//     // uint8_t *data_buf_start=pack_buf+protocol->head_size;
//     // protocol->head.target_addr=target_addr;
//     update_target_addr(protocol,target_addr);
//     return create_packet(pack_buf,send_data,send_size,protocol,pack_cls);
// }
// int respond_packet(uint8_t pack_buf[BUF_SIZE],uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,ReceiveStr *receive,uint8_t pack_cls){
int respond_packet(uint8_t *pack_buf,uint8_t *send_data,uint16_t send_size,ProtocolStr* protocol,ReceiveStr *receive,uint8_t pack_cls){
    // update_target_addr(protocol,get_decode_source_addr(receive));
    return create_packet(pack_buf,send_data,send_size,protocol,get_decode_source_addr(receive),pack_cls);
}
int send_to_processes_buffer(ReceiveStr *receive_protocol,uint8_t *data,int data_size){
    memcpy(receive_protocol->buf,data,data_size);
    receive_protocol->buf_size=data_size;
    receive_protocol->handle_index=0;
    return 0;
}
int unpack_buf_from_back(ReceiveStr *receive_protocol,uint8_t *buf,int buf_size,int decode_all_target){
    ProtocolStr* protocol=receive_protocol->myprotocol;
    int get_size=(int)buf_size - (int)protocol->head_size;
    int i=0;
    HeadStr* temp_head;
    uint16_t head_sum;
    uint8_t *data;
    DataTailStr *temp_tail;
    uint16_t sum;
    int loss;
    for(i=get_size-1;i>=0;--i){
        temp_head=(HeadStr*)(buf+i);
        if(temp_head->head_flags==protocol->head.head_flags){ //对比头
            if(temp_head->rear_flags==protocol->head.rear_flags){ //对比尾
                if(decode_all_target || temp_head->target_addr==protocol->head.source_addr){ //对比目标地址
                    head_sum = get_head_val(temp_head);
                    if(temp_head->val_sum == head_sum){ // 头校验
                        if (get_size-i >= temp_head->data_size + protocol->data_tail_size){ //检查后续数据是否满足一个包大小
                            data=(buf+i+protocol->head_size);
                            temp_tail=(DataTailStr*)(data+temp_head->data_size);
                            if(temp_tail->data_rear_flags==protocol->data_tail.data_rear_flags){ //对比数据包尾
                                sum=VAL_FUNCTION(head_sum,data,temp_head->data_size);
                                if(sum == temp_tail->data_val_sum){ //数据校验对比
                                    loss= (int)(temp_head->pack_idx) - (int)(receive_protocol->pack_idx) - 1;
                                    if(loss>0){
#ifdef HAVE_PRINTF
                                        printf("丢包 %d ",loss);
                                        printf("  get  %d   last %d \n",temp_head->pack_idx,receive_protocol->pack_idx);
#endif
                                        // printf("  get  %d   last %d \n",temp_head->pack_idx,protocol->head.pack_idx);
                                    }else{ //若为负数,则为之前的包,-1则为同一个包,没太大意义
                                        loss=0;
                                    }
                                    receive_protocol->decode_protocol.head=temp_head;
                                    receive_protocol->decode_protocol.data_tail=temp_tail;
                                    receive_protocol->decode_data_index= i + protocol->head_size;
                                    receive_protocol->decode_data_size = temp_head->data_size;
                                    receive_protocol->loss=loss;
                                    // receive_protocol->handle_index = i+protocol->head_size+temp_head->data_size+protocol->data_tail_size;
                                    receive_protocol->pack_idx=temp_head->pack_idx;
                                    return temp_head->data_size;
                                }
                            }

                        }
                    }
                }

            }
        }
    }
    // receive_protocol->handle_index=receive_protocol->buf_size;
    return 0;
}
int unpack_buf(ReceiveStr *receive_protocol,int decode_all_target){
    ProtocolStr* protocol=receive_protocol->myprotocol;
    int get_size=(int)receive_protocol->buf_size - (int)protocol->head_size;
    int i=0;
    HeadStr* temp_head;
    uint16_t head_sum;
    uint8_t *data;
    DataTailStr *temp_tail;
    uint16_t sum;
    int loss;
    uint8_t *buf=receive_protocol->buf;
    //decode head
    for(i=receive_protocol->handle_index;i<get_size;++i){
        temp_head=(HeadStr*)(buf+i);
        if(temp_head->head_flags==protocol->head.head_flags){ //对比头
            if(temp_head->rear_flags==protocol->head.rear_flags){ //对比尾
                if(decode_all_target || temp_head->target_addr==protocol->head.source_addr){ //对比目标地址
                // if(temp_head->target_addr==protocol->head.source_addr){ //对比目标地址
                    head_sum = get_head_val(temp_head);
                    if(temp_head->val_sum == head_sum){ // 头校验
                        if (get_size-i >= temp_head->data_size + protocol->data_tail_size){ //检查后续数据是否满足一个包大小
                            data=(buf+i+protocol->head_size);
                            temp_tail=(DataTailStr*)(data+temp_head->data_size);
                            if(temp_tail->data_rear_flags==protocol->data_tail.data_rear_flags){ //对比数据包尾

                                // uint16_t sum=get_val_crc16(head_sum,data,temp_head->data_size); //数据校验，在head的校验基础上再校验
                                sum=VAL_FUNCTION(head_sum,data,temp_head->data_size);
                                if(sum == temp_tail->data_val_sum){ //数据校验对比
                                    loss= (int)(temp_head->pack_idx) - (int)(receive_protocol->pack_idx) - 1;
                                    // int loss=temp_head->pack_idx - protocol->head.pack_idx -1;
                                    if(loss>0){
#ifdef HAVE_PRINTF
                                        printf("丢包 %d ",loss);
                                        printf("  get  %d   last %d \n",temp_head->pack_idx,receive_protocol->pack_idx);
#endif
                                        // printf("  get  %d   last %d \n",temp_head->pack_idx,protocol->head.pack_idx);
                                    }else{ //若为负数,则为之前的包,-1则为同一个包,没太大意义
                                        loss=0;
                                    }
                                    receive_protocol->decode_protocol.head=temp_head;
                                    receive_protocol->decode_protocol.data_tail=temp_tail;
                                    receive_protocol->decode_data_index= i + protocol->head_size;
                                    receive_protocol->decode_data_size = temp_head->data_size;
                                    receive_protocol->loss=loss;
                                    receive_protocol->handle_index= i+protocol->head_size+temp_head->data_size+protocol->data_tail_size;

                                    receive_protocol->pack_idx=temp_head->pack_idx;
                                    return temp_head->data_size;
                                }
                            }

                        }
                    }
                }

            }
        }
    }
    receive_protocol->handle_index=receive_protocol->buf_size;
    return 0;
}
/*
//拷贝接收数据
*/
int copy_receive_data(ReceiveStr *receive,uint8_t *buf){
    memcpy(buf,receive->buf+receive->decode_data_index,receive->decode_data_size);
    return receive->decode_data_size;
}
/*
//拷贝接收到的协议以及数据
Protocol temp_protocol;   //拷贝保存的变量
uint8_t data_buf[BUF_SIZE];  //拷贝保存的缓冲区(要保证缓冲区协议大,否则会导致溢出错误)
int data_size=copy_receive_protocol(&receive_protocol,&temp_protocol,data_buf);
*/

int copy_receive_protocol(ReceiveStr *receive,ProtocolStr *new_protocol,uint8_t *buf){
    new_protocol->head= *(receive->decode_protocol.head);
    new_protocol->data_tail= *(receive->decode_protocol.data_tail);
    new_protocol->data_tail_size=receive->myprotocol->data_tail_size;
    new_protocol->head_size=receive->myprotocol->head_size;
    // new_protocol->data=buf;
    // new_protocol->data_size=receive->buf_size;
    return copy_receive_data(receive,buf);
}

void set_source_addr(ProtocolStr* protocol,uint8_t source_addr){
    protocol->head.source_addr=source_addr;
}
void set_target_addr(ProtocolStr* protocol,uint8_t target_addr){
    protocol->head.target_addr=target_addr;
}
uint8_t *get_data(ReceiveStr *receive){
    return receive->buf+receive->decode_data_index;
}
uint16_t get_data_index(ReceiveStr *receive){
    return receive->decode_data_index;
}
uint16_t get_data_size(ReceiveStr *receive){
    return receive->decode_data_size;
}
uint8_t get_pack_cls(ReceiveStr *receive){
    return receive->decode_protocol.head->pack_cls;
}
uint16_t get_pack_idx(ReceiveStr *receive){
    return receive->pack_idx;
}
uint16_t get_loss(ReceiveStr *receive){
    return receive->loss;
}
uint16_t get_buf_size(ReceiveStr *receive){
    return receive->buf_size;
}
uint8_t *get_buf(ReceiveStr *receive){
    return receive->buf;
}
uint8_t get_protocol_source_addr(ProtocolStr *protocol){
    return protocol->head.source_addr;
}
uint8_t get_protocol_target_addr(ProtocolStr *protocol){
    return protocol->head.target_addr;
}
uint8_t get_decode_source_addr(ReceiveStr *receive){
    return receive->decode_protocol.head->source_addr;
}
uint8_t get_decode_target_addr(ReceiveStr *receive){
    return receive->decode_protocol.head->target_addr;
}

#ifdef __linux__
void *repeat_malloc(void *buf,uint32_t element_size,uint32_t buf_size,uint32_t target_buf_size){
    /*
      buf必须为NULL或已分配的内存
     */
    if(buf == NULL || buf_size < target_buf_size){
        if(buf!=NULL){
            free(buf);
            buf=NULL;
        }
        buf = (void *)malloc(element_size*target_buf_size);

    }
    return buf;
}
#endif
// int clean_receive(ReceiveStr *receive){
//     // receive->handle_index=0;
//     // receive->buf_size=0;
//     // receive->
// }























