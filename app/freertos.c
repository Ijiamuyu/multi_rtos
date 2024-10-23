#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "console.h"

#define TASK1_TASK_PRIO     2
#define TASK1_STK_SIZE      128
xTaskHandle Task1Task_Handler;


#define TASK2_TASK_PRIO     3
#define TASK2_STK_SIZE      128
xTaskHandle Task2Task_Handler;


volatile uint32_t thread1_cnt=0;
volatile uint32_t thread2_cnt=0;

//task1任务函数
void task1_task(void *pvParameters)
{
    while(1)
    {
        console_printf("freertos 1 application running %d times\r\n",++thread1_cnt);
        vTaskDelay(50); //延时500ms
    }
}

//task2任务函数
void task2_task(void *pvParameters)
{
    while(1)
    {
        console_printf("freertos 2 application running %d times\r\n",++thread2_cnt);
        vTaskDelay(100); //延时200ms
    }
}

int main(void)
{
    console_printf("\nBuild in %s(%s)\n", __DATE__,__TIME__);
    console_printf("freertos start\n");
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,
                (const char*    )"task1_task",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_TASK_PRIO,
                (xTaskHandle*  )&Task1Task_Handler);
    //创建TASK2任务
    xTaskCreate((TaskFunction_t )task2_task,
                (const char*    )"task2_task",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_TASK_PRIO,
                (xTaskHandle*  )&Task2Task_Handler);
    vTaskStartScheduler();
    hang();
}


