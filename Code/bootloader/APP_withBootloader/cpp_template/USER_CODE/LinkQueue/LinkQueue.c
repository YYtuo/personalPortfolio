#include "LinkQueue.h"
#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**********************************
 * 函数名：Create_LinkQueue
 * 功能：初始化一个队列
 * 输入：无
 * 输出：返回队列指针
 * 备注：单（双）链表实现队列
**********************************/
LinkQueue_TypeDef* Create_LinkQueue(void){
    // 创建一个链表，并分配内存
    LinkQueue_TypeDef *Queue = (LinkQueue_TypeDef *)mymalloc(SRAMIN,sizeof(LinkQueue_TypeDef));
    
    // 创建链表的头节点，并分配内存
    Node_TypeDef *HeadNode = (Node_TypeDef *)mymalloc(SRAMIN,sizeof(Node_TypeDef));

    if(Queue == NULL){
        #ifdef PRINTF
        printf("Queue creat error:RAM ran out!");
        #endif
        return 0;
    }
    if(HeadNode == NULL){
        #ifdef PRINTF
        printf("HeadNode creat error:RAM ran out!");
        #endif
        return 0;
    }

    // 链表参数初始化
    // 队首指向头节点
    Queue->Front = HeadNode;
    // 队尾指向头节点
    Queue->Rear = HeadNode;

    // 头节点初始化
    // 头结点没有上一个节点，此时队列为空
    #ifdef DOUBLE_LINK
    HeadNode->prev = NULL;
    #endif // DOUBLE_LINK

    // // 头节点的数据用来记录整个链表的有效结点数(除了头节点)
    // HeadNode->date = 0;

    // 头结点没有下一个节点，此时队列为空.
    HeadNode->next = NULL;

    return Queue;
}

/*===================================================================*/
// 以下为循环单链表队列
/*===================================================================*/

/**********************************
 * 函数名：Create_Circle_LinkQueue
 * 功能：初始化一个环状队列
 * 输入：无
 * 输出：返回队列指针
 * 备注：循环链表实现队列
**********************************/
Circle_LinkQueue_TypeDef* Create_Circle_LinkQueue(void){
    // 创建一个链表，并分配内存
    Circle_LinkQueue_TypeDef *Circle_Queue = (Circle_LinkQueue_TypeDef *)mymalloc(SRAMIN,sizeof(Circle_LinkQueue_TypeDef));
    
    // // 创建链表的头节点，并分配内存
    // Node_TypeDef *HeadNode = (Node_TypeDef *)mymalloc(SRAMIN,sizeof(Node_TypeDef));

    if(Circle_Queue == NULL){
        #ifdef PRINTF
        printf("Queue creat error:RAM ran out!");
        #endif
        return 0;
    }
    // if(HeadNode == NULL){
    //     #ifdef PRINTF
    //     printf("HeadNode creat error:RAM ran out!");
    //     #endif
    //     return 0;
    // }

    // 链表参数初始化
    // // 队首指向头节点
    // Queue->Front = HeadNode;
    // 队尾指向头节点
    Circle_Queue->Rear = NULL;
    // // 队列为空，结点数等于0
    Circle_Queue->queue_nodeNum = 0;

    return Circle_Queue;
}
// 1：空；0：非空
uint8_t isCircleQueueEmpty(Circle_LinkQueue_TypeDef *qu){
    return (qu->Rear == NULL);
}

/**********************************
 * 函数名：Delete_Circle_LinkQueue
 * 功能：删除整个环形队列
 * 输入：qu:待删除的队列
 * 输出：成功返回1，失败返回0
 * 备注：无
**********************************/
uint8_t Delete_Circle_LinkQueue(Circle_LinkQueue_TypeDef *qu){
    // 队列不为空，先清除所有节点，再删除队列
    if(!isCircleQueueEmpty(qu)){
        /*这里放删除节点的函数*/
        /*end of 删除节点的函数*/
    }

    // 释放队列的空间
    qu->Rear = NULL;
    qu->queue_nodeNum = 0;
    myfree(SRAMIN,qu);
    return 1;

}

/**********************************
 * 函数名：getCircleQueue_NodeNum
 * 功能：获取队列中的节点数
 * 输入：队列指针
 * 输出：节点数
 * 备注：无
**********************************/
// uint8_t getCircleQueue_NodeNum(Circle_LinkQueue_TypeDef *qu){
//     // 环形队列的队尾指向头节点 
//     Node_TypeDef* node_index = qu->Rear;    // 通过队尾,找到头节点,node_index指向第一个节点
//     uint8_t num = 0;

//     if(node_index == NULL)return num;
//     while(node_index->next != qu->Rear){
//         // 往下查询,直到查到第一个节点,表示遍历结束
//         node_index = node_index->next;  
//         num++;
//     }
    
//     num++;  // 不要忘记最后一个节点
//     return num;
// }
uint8_t getCircleQueue_NodeNum(Circle_LinkQueue_TypeDef *qu){
    return qu->queue_nodeNum;
}


/**********************************
 * 函数名：Ins_CircleQueue
 * 功能：向环形队列中插入一个节点
 * 输入：qu：队列指针，*fileList：二维数组指针
 * 输出：插入成功返回当前队列结点数，插入失败返回0
 * 备注：无
**********************************/
uint8_t Ins_CircleQueue(Circle_LinkQueue_TypeDef* qu, uint8_t *fileName){
    // 初始化一个新的节点，为新节点分配内存并且填充值
    Node_TypeDef* new_node = (Node_TypeDef*)mymalloc(SRAMIN,sizeof(Node_TypeDef));
    if(new_node == NULL){
        #ifdef PRINTF
        printf("new node creat error!");
        #endif
        return 0;
    }

    /*用户节点赋值*/
    new_node->fileName = fileName;
    /*end of 用户节点赋值*/

    // 将新节点链接进链表
    // 空队列，第一个节点指向自己
    if(qu->Rear==NULL){
        new_node->next = new_node;
        // qu->Rear = new_node;
    }
    else{
        new_node->next = qu->Rear->next;    // 先确认新节点指针域的指向，再断开队尾节点指针域的指向
        qu->Rear->next = new_node;
        // qu->Rear = new_node;
    }
    qu->queue_nodeNum++;
    qu->Rear = new_node;    // 队尾指针指向最新节点
    return qu->queue_nodeNum;
}

/**********************************
 * 函数名：Del_CircleQueue
 * 功能：从环形队列中删除一个节点
 * 输入：qu：队列指针，*fileList：二维数组指针
 * 输出：删除成功返回当前队列结点数，插入失败返回0
 * 备注：无
**********************************/
uint8_t Del_CircleQueue(Circle_LinkQueue_TypeDef* qu, uint8_t *fileName){
    // 若为空队列，则不做操作
    if(qu->Rear == NULL){
        #ifdef PRINTF
        printf("===The queue is empty!===");
        #endif
        return 0;
    }

    // 临时保存需要删除的节点，供free函数删除
    Node_TypeDef* del_node = 0;

    // 队尾指向自己，说明只有一个节点
    if(qu->Rear == qu->Rear->next){
        del_node = qu->Rear;
        qu->Rear = NULL;
        qu->queue_nodeNum = 0;
    }
    else{
        del_node = qu->Rear->next;  // 满足先进先出规则，从队头移出
        qu->Rear->next = del_node->next;    // 队尾的下一个结点（队头）更新为被移出的节点的下一个
        qu->queue_nodeNum--;
    }

    /*将被移出队的数据传递出去*/
    fileName = del_node->fileName;
	
	fileName = fileName; // 避免警告，可以删除
    /*end of 数据传递*/
    myfree(SRAMIN,del_node);
    return  qu->queue_nodeNum;
}



#ifdef __cplusplus
 }
#endif /* __cplusplus */

