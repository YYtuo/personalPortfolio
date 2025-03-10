  
/* Define to prevent recursive -----------------------------------------------*/
#ifndef __USBH_HID_REMOTE_H
#define __USBH_HID_REMOTE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usbh_hid_core.h"
#include "usbh_usr.h"

//方向键宏定义
#define UP_KEY 0x00
#define RIGHT_KEY 0x02
#define DOWN_KEY 0x04
#define LEFT_KEY 0x06

#define ROCKER_LR_ZERO 0X80
#define ROCKER_UD_ZERO 0X7F

typedef enum {
    remote_mode_error = 0,              //遥控器数据解析初始化失败
    red_mode = 1,           //红灯模式，摇杆输出模拟量
    green_mode = 2          //绿灯模式，遥感输出数字量
}Remote_Mode;

typedef enum {
    right_key = 1,
    left_key = 2,
    up_key = 3,
    down_key = 4,
	
    triangle = 5,
    circular = 6,
    fork = 7,
    square = 8,
    L1 = 9,
    R1 = 10,
    L2 = 11,
    R2 = 12,

    select = 13,
    start = 14,
    left_rocker = 15,
    right_rocker = 16,
    mode = 17

}Remote_Key_NO_;

typedef enum{
    left_rocker_lr = 1,
    left_rocker_ud = 2,
    right_rocker_lr = 3,
    right_rocker_ud = 4
}Remote_Rocker_NO_;


// typedef enum {
//     up = UP_KEY,
//     right = RIGHT_KEY,
//     down = DOWN_KEY,
//     left = LEFT_KEY
// }DirectionKey_Val;

//遥控器主按键寄存器
typedef struct Remote_primaryKeyReg{
    uint8_t triangle:1;     //三角形按键
    uint8_t circular:1;     //圆形按键
    uint8_t fork:1;         //叉形按键
    uint8_t square:1;       //正方形按键

    uint8_t L1:1;       //肩键
    uint8_t R1:1;       //肩键
    uint8_t L2:1;       //肩键
    uint8_t R2:1;       //肩键
}Remote_primaryKeyReg_TypeDef;

//遥控器功能按键寄存器
typedef struct Remote_funtionKeyReg{
    uint8_t select:1;               //select按键
    uint8_t start:1;                //start按键
    uint8_t left_rocker:1;          //左摇杆按键
    uint8_t right_rocker:1;         //右摇杆按键

    uint8_t mode:1;       //mode键
    uint8_t :0;

}Remote_funtionKeyReg_TypeDef;

//遥控器摇杆模拟量寄存器
typedef struct Remote_rockerValReg{
    uint8_t left_rocker_lr;         //左摇杆左右
    uint8_t left_rocker_ud;         //左摇杆上下
    uint8_t right_rocker_lr;        //右摇杆左右
    uint8_t right_rocker_ud;        //右摇杆上下
}Remote_rockerValReg_TypeDef;

//遥控器按键压力值
typedef struct Remote_KeyPressureVal{
    //buff[7-10]
    uint8_t right_key;
    uint8_t left_key;
    uint8_t up_key;
    uint8_t down_key;

    //buff[11-14]
    uint8_t triangle;
    uint8_t circular;
    uint8_t fork;
    uint8_t square;

    //buff[15-18]
    uint8_t L1;
    uint8_t R1;
    uint8_t L2;
    uint8_t R2;
}Remote_KeyPressureVal_TypeDef;

//遥控器按键按下次数寄存器
typedef struct Remote_KeyPressedTimes{
    uint8_t select;               //select按键
    uint8_t start;                //start按键
    uint8_t left_rocker;          //左摇杆按键
    uint8_t right_rocker;         //右摇杆按键

    uint8_t mode;       //mode键


    uint8_t right_key;
    uint8_t left_key;
    uint8_t up_key;
    uint8_t down_key;

    uint8_t triangle;
    uint8_t circular;
    uint8_t fork;
    uint8_t square;

    uint8_t L1;
    uint8_t R1;
    uint8_t L2;
    uint8_t R2;
}Remote_KeyPressedTimes_TypeDef;



//遥控器状态寄存器
typedef struct Remote_state{
	uint8_t judge_mode_flag;                        // 初次上电使用，用于识别遥控器当前模式

    uint8_t remote_en:1;                              // 遥控器上下控标志位，1上控，0下控
    Remote_Mode preMODE:2;                            //记录前一次是绿灯还是红灯模式
    Remote_Mode MODE:2;                         //遥控器模式，0：红灯模式。1：绿灯模式

    uint8_t :0;

    //buff[0]
    Remote_primaryKeyReg_TypeDef primaryKeyReg;      //肩键、右边按键寄存器
    //buff[1]
    Remote_funtionKeyReg_TypeDef funtionKeyReg;      //功能键、摇杆按键寄存器
    //buff[2]
    // DirectionKey_Val DirectionKey;              //方向键状态
    uint8_t DirectionKey;
    //buff[3]-[6]
    Remote_rockerValReg_TypeDef rockerValReg;         //摇杆寄存器
    // uint8_t left_rocker_lr;                     //左摇杆左右
    // uint8_t left_rocker_ud;                     //左摇杆上下
    // uint8_t right_rocker_lr;                    //右摇杆左右
    // uint8_t right_rocker_ud;                    //右摇杆上下
    //buff[7-18]
    Remote_KeyPressureVal_TypeDef keyPressure;       //各个按键的压力值
    Remote_KeyPressedTimes_TypeDef keyPressedTimes;//各个按键按下的次数
}Remote_state_TypeDef;


extern HID_cb_TypeDef HID_REMOTE_cb;
extern Remote_state_TypeDef HID_Remote;






void USR_REMOTE_Init(void);
void USR_REMOTE_ProcessData(Remote_state_TypeDef* Remote_data);


uint8_t remote_getPermission(Remote_state_TypeDef* Remote_data);
Remote_Mode remote_getMode(Remote_state_TypeDef* Remote_data);
uint8_t remote_getRockerVal(Remote_state_TypeDef* Remote_data, Remote_Rocker_NO_ rocker_NO_);
uint8_t remote_getPressureVal(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_);
uint8_t remote_getPressureTimes(Remote_state_TypeDef* Remote_data,Remote_Key_NO_ key_NO_);



Remote_Mode remote_changeMode(Remote_state_TypeDef* Remote_data);
uint8_t remote_setPermission(Remote_state_TypeDef *Remote_data);
void record_keyPressedTimes(Remote_state_TypeDef *RemoteData, Remote_Key_NO_ key_NO_);

#endif /* __USBH_HID_REMOTE_H */

