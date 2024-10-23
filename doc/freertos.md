# overall
本文主要介绍freertos的移植要点

# tarce
以riscv的freertos为例，追踪代码调用过程
```c
main
    -->|soc_init
        -->|......
        -->|soc_post_init //port
            -->|__RV_CSR_SET(CSR_MIE, MIE_MEIE)
            -->|__enable_irq()
    -->|osal_create_task(task_init_app, "init_app", 256, 1, NULL)
        -->|xTaskCreate((TaskFunction_t)func, name, stack_size, param, task_priority, &xHandle_SysMgr)
            -->|pxStack = pvPortMalloc( ( ( ( size_t ) usStackDepth ) * sizeof( StackType_t )))
            -->|pxNewTCB = (TCB_t *) pvPortMalloc(sizeof(TCB_t))
            -->|prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) usStackDepth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, NULL )
            -->|prvAddNewTaskToReadyList( pxNewTCB )
    -->|osal_start_scheduler()
        -->|vTaskStartScheduler()
            -->|xReturn = xTaskCreate( prvIdleTask,
                            configIDLE_TASK_NAME,
                            configMINIMAL_STACK_SIZE,
                            ( void * ) NULL,
                            portPRIVILEGE_BIT,
                            &xIdleTaskHandle );
            -->|xReturn = xTimerCreateTimerTask()
                -->|prvCheckForValidListAndQueue()
                -->|xReturn = xTaskCreate( prvTimerTask,
                                    configTIMER_SERVICE_TASK_NAME,
                                    configTIMER_TASK_STACK_DEPTH,
                                    NULL,
                                    ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                    &xTimerTaskHandle )
                -->|configASSERT( xReturn )
            -->|portDISABLE_INTERRUPTS()
            -->|portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() //nop
            -->|traceTASK_SWITCHED_IN()
            -->|xPortStartScheduler()
                -->|__disable_irq()
                -->|soc_set_interrupt_threshold(0)
                -->|vPortSetupTimerInterrupt() //port
                    -->|soc_set_interrupt_priority
                    -->|soc_interrupt_enable
                    -->|SysTick_Config
                -->|__enable_software_irq()
                -->|xPortStartFirstTask()
                -->|prvTaskExitError()
            -->|( void ) xIdleTaskHandle
            -->|( void ) uxTopUsedPriority


xPortSysTickHandler
    -->|rv_timer_isr //#define xPortSysTickHandler rv_timer_isr
        -->|portDISABLE_INTERRUPTS()
        -->|SysTick_Reload(SYSTICK_TICK_CONST)
        -->|xTaskIncrementTick()
        -->|portYIELD() //portYIELD == soc_trigger_software_irq
            -->|soc_trigger_software_irq()
        -->|portENABLE_INTERRUPTS()
```
在上述代码中，使用外部timer中断以及软件中断来切换任务

# riscv-virt平台


