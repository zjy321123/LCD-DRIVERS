#include <stdio.h>
#include "task.h"
#include "gpio.h"

/* 全局变量定义 */
Task g_taskList[MAX_TASKS]; // 任务列表
uint32_t g_systemTick = 0;
/*******************************************************************************
 * 函数名称: Scheduler_Run
 * 函数功能: 任务调度运行
 * 参    数: 无
 * 返 回 值: 无
 * 说    明: 遍历任务列表，按周期执行任务
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
void Scheduler_Run(void)
{
    uint32_t currentTick = g_systemTick;

    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (g_taskList[i].state == TASK_READY &&
            (currentTick - g_taskList[i].lastRun >= g_taskList[i].period))
        {
            g_taskList[i].lastRun = currentTick;
            g_taskList[i].run(currentTick);
        }
    }
}

/*******************************************************************************
 * 函数名称: SysTick_Task
 * 函数功能: 系统时钟任务
 * 参    数: 无
 * 返 回 值: 无
 * 说    明: 每次系统时钟中断，系统时钟加1
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
void SysTick_Task(void)
{
    g_systemTick++; // 系统时钟加1
}

/*******************************************************************************
 * 函数名称: Task_Init
 * 函数功能: 任务初始化
 * 参    数: 无
 * 返 回 值: 无
 * 说    明: 初始化任务列表，设置任务周期和初始状态
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
void Task_Init(void)
{
    g_taskList[0] = (Task){Task1, 1, 0, TASK_READY};
    g_taskList[1] = (Task){Task2, 10, 0, TASK_READY};
    g_taskList[2] = (Task){Task3, 50, 0, TASK_READY};
    g_taskList[3] = (Task){Task4, 250, 0, TASK_READY};
    g_taskList[4] = (Task){Task5, 1000, 0, TASK_READY};
}

/*******************************************************************************
 * 函数名称: Task1
 * 函数功能: 任务1执行
 * 参    数: currentTick - 当前系统时钟
 * 返 回 值: 无
 * 说    明: 任务1的具体逻辑
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
static void Task1(uint32_t currentTick)
{

    g_taskList[0].state = TASK_RUNNING;

    g_taskList[0].state = TASK_READY;
}

/*******************************************************************************
 * 函数名称: Task2
 * 函数功能: 任务2执行
 * 参    数: currentTick - 当前系统时钟
 * 返 回 值: 无
 * 说    明: 任务2的具体逻辑
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
static void Task2(uint32_t currentTick)
{
    g_taskList[1].state = TASK_RUNNING;
    g_taskList[1].state = TASK_READY;
}

/*******************************************************************************
 * 函数名称: Task3
 * 函数功能: 任务3执行
 * 参    数: currentTick - 当前系统时钟
 * 返 回 值: 无
 * 说    明: 任务3的具体逻辑
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
static void Task3(uint32_t currentTick)
{
    g_taskList[2].state = TASK_RUNNING;

    g_taskList[2].state = TASK_READY;
}

/*******************************************************************************
 * 函数名称: Task4
 * 函数功能: 任务4执行
 * 参    数: currentTick - 当前系统时钟
 * 返 回 值: 无
 * 说    明: 任务4的具体逻辑
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
static void Task4(uint32_t currentTick)
{
    g_taskList[3].state = TASK_RUNNING;
    g_taskList[3].state = TASK_READY;
}

/*******************************************************************************
 * 函数名称: Task5
 * 函数功能: 任务5执行
 * 参    数: currentTick - 当前系统时钟
 * 返 回 值: 无
 * 说    明: 任务5的具体逻辑
 * 修改记录: 2025.04.15, ZJY 创建函数
 ******************************************************************************/
static void Task5(uint32_t currentTick)
{
    g_taskList[4].state = TASK_RUNNING;

    g_taskList[4].state = TASK_READY;
}
