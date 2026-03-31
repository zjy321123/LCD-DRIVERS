#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 5 // 最大任务数量

/* 任务状态枚举 */
typedef enum
{
    TASK_READY,    // 任务已准备
    TASK_RUNNING,  // 任务运行中
    TASK_WAITING,  // 任务等待中
    TASK_SUSPENDED // 任务挂起
} TaskState;

/* 任务结构体定义 */
typedef struct
{
    void (*run)(uint32_t currentTick); // 任务函数指针，参数为当前时钟周期
    uint32_t period;                   // 任务执行周期
    uint32_t lastRun;                  // 任务最后一次执行的时钟周期
    TaskState state;                   // 任务当前状态
} Task;

/* 全局变量声明 */
extern Task g_taskList[MAX_TASKS];

/* 函数声明 */
extern void Scheduler_Run(void);
extern void SysTick_Task(void);
extern void Task_Init(void);

/* 任务函数声明 */
static void Task1(uint32_t currentTick);
static void Task2(uint32_t currentTick);
static void Task3(uint32_t currentTick);
static void Task4(uint32_t currentTick);
static void Task5(uint32_t currentTick);

#endif /* TASK_H */


