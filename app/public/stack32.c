/**************************************************************************************************
**                                                                                               **
**  文件名称:  Stack.c                                                                           **
**  版权所有:  CopyRight @ Fujian Guode Medical Technology Co.,Ltd 2015                          **
**  文件描述:  队列                                                                              **
**  ===========================================================================================  **
**  创建信息:  | 2015-5-5 | lwp | 创建本模块                                                     **
**  ===========================================================================================  **
**  修改信息:  单击此处添加....                                                                  **
**************************************************************************************************/
#include "type.h" 
#include "stack32.h" 
/*************************************************************************************************/
/*                           相关定义区域                                                        */
/*************************************************************************************************/

#ifdef MAXSSIZE
#undef MAXSSIZE
#endif
#define MAXSSIZE         5
/*************************************************************************************************/
/*                           函数声明区域                                                        */
/*************************************************************************************************/
/**************************************************************************************************
**  函数名称:  InitStack
**  功能描述:  初始化队列
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
**************************************************************************************************/
void InitStack(stack_t* s, u32_t *buff, u32_t maxsize)
{    
    s->base = buff;
    s->front = s->rear = 0;
    s->maxsize =maxsize;
} 
/**************************************************************************************************
**  函数名称:  EnStack
**  功能描述:  入队
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
**************************************************************************************************/
u8_t EnStack(stack_t* s,u32_t e)
{
    if (((s->rear+1)%s->maxsize) == s->front){
        return 0;                                                                    /* 队列满 */
    }

    if (s->rear == s->maxsize) {
        s->rear = s->maxsize;
    }
    s->base[s->rear]=e;
    s->rear=s->rear+1;
    return 1;
} 
/**************************************************************************************************
**  函数名称:  DeStack
**  功能描述:  出队
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
**************************************************************************************************/
u8_t DeStack(stack_t* s,u32_t* e)
{
    if(s->front == s->rear){
        return 0;
    }

    *e = s->base[s->rear-1];

    s->rear = s->rear-1;

    return 1;
}
/**************************************************************************************************
**  函数名称:  IsStack
**  功能描述:  出队
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
**************************************************************************************************/
u8_t IsStack(stack_t* s)
{
    if (((s->rear+1)% s->maxsize) == s->front){
        return 0;                                                                    /* 队列满 */
    }
    return 1;
}