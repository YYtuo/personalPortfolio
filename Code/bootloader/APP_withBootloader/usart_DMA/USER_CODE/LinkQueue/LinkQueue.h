#ifndef __LINKQUEUE_H
#define __LINKQUEUE_H

#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "bsp_usart.h"	
#include "IAP.h"

// #define DOUBLE_LINK     // 双向链表宏

/*常规链表定义，一个链表只能放一种类型的结点*/
//队列节点 
typedef struct Node{
    #ifdef DOUBLE_LINK
    struct Node *prev;//上一个结点
    #endif // DOUBLE_LINK

	uint8_t *fileName;       //结点数据，根据实际需要修改
	struct Node *next;//下一个结点
}Node_TypeDef;
/*end of 常规列表定义*/

/*内核链表定义，只有指针域，数据域自定义*/
// typedef struct LinkNode{
//     #ifdef DOUBLE_LINK
// 	struct LinkNode *prev; //上一个结点
//     #endif // DOUBLE_LINK

// 	struct LinkNode *next; //下一个结点
// }LinkNode_TypeDef;

// // 用户数据节点，其中链表作为数据成员
// typedef struct Person{
//     LinkNode_TypeDef node;
//     char name[64];
//     int age;
// }Person;
/*end of 内核链表定义*/

/*链式队列定义*/
typedef struct LinkQueue{
	struct Node * Front;//队首指针
	struct Node * Rear;//队尾指针
}LinkQueue_TypeDef;
/*end of 链式队列定义*/

/*循环链式队列定义*/
typedef struct Circle_LinkQueue{
	struct Node * Rear;         //队尾指针
    uint8_t queue_nodeNum;      // 队中的结点数
}Circle_LinkQueue_TypeDef;
/*end of 循环链式队列定义*/

Circle_LinkQueue_TypeDef* Create_Circle_LinkQueue(void);
uint8_t isCircleQueueEmpty(Circle_LinkQueue_TypeDef *qu);
uint8_t getCircleQueue_NodeNum(Circle_LinkQueue_TypeDef *qu);
uint8_t Ins_CircleQueue(Circle_LinkQueue_TypeDef* qu, uint8_t *fileName);
uint8_t Del_CircleQueue(Circle_LinkQueue_TypeDef* qu, uint8_t *fileName);




#endif

