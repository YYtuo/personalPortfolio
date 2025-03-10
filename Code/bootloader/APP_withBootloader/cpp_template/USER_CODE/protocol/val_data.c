#include "val_data.h"
/*
全加和校验
*/
uint16_t get_val_sum(uint16_t org_val,uint8_t *send_data,int size){
    int i=0;
    for(i=0;i<size;++i){
        // sum+=(uint8_t)(send_data[i]);
        org_val+=(uint16_t)(send_data[i]);
    }
    return org_val;
}
/*
异或和校验
*/
uint16_t get_val_xor_sum(uint16_t org_val,uint8_t *send_data,int size){
    int i=0;
    for(i=0;i<size;++i){
        // sum+=(uint8_t)(send_data[i]);
        org_val^=(uint16_t)(send_data[i]);
    }
    return org_val;
}
/*
CRC16校验
*/
uint16_t get_val_crc16(uint16_t org_val,uint8_t *send_data,int size){
    org_val = CRC16_checkTable(org_val, send_data, size);//再校验数据
    return org_val;
}

/*查表法*/
uint16_t CRC16_checkTable(uint8_t org_val, uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;

    iIndex = ucCRCLo ^ org_val;
    ucCRCLo = ( uint8_t )( ucCRCHi ^ crc16_MODBUS_table_Hi[iIndex] );
    ucCRCHi = crc16_MODBUS_table_Lo[iIndex];

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t )( ucCRCHi ^ crc16_MODBUS_table_Hi[iIndex] );
        ucCRCHi = crc16_MODBUS_table_Lo[iIndex];
    }
    return ( uint16_t )( ucCRCHi << 8 | ucCRCLo );
}

/*计算法*/
/***********************************************
 * 函数名：Crc_Check
 * 功能: 对数组的前len个数据进行CRC16校验，
 *      校验码保存在数组最后
 * 入口参数：uint8_t *RSdata：待校验数据数组 (长度=len+2)
 *          uint8_t len: 原始数据长度
 * 输出：校验成功返回1
 *      校验失败返回0
 * ********************************************/
uint8_t Crc_Check(uint8_t *RSdata, uint8_t len)
{
    uint16_t crc;                //定义16位的CRC
    uint8_t i,j;
    crc = 0xffff;
    for( i=0; i<len; i++ )             //数组前len个元素是真实数据
    {
         crc ^= *(RSdata++);
         for(j=8;j>0;j--)
         {
              if(crc & 0x0001) crc = (crc>>1) ^ 0xA001;
              else crc >>= 1;
         }
    }
    i=crc>>8;
    j=crc;
    if( (i==*(RSdata++)) && (j==*(RSdata--)) )
         return 1;                   //CRC校验成功，返回1
    else                             //CRC校验失败，将CRC存入数组
    {
         *(--RSdata) = i;     //存放CRC的高八位;
         *(++RSdata) = j;     //CRC低八位
         return 0;
    }
}

