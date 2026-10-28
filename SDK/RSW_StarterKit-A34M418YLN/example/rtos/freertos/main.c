/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       Main Example Code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "debug_cmd.h"
#include "debug_log.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


static uint32_t idle_stack[configMINIMAL_STACK_SIZE];
static uint32_t timer_stack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    void *newTCB = pvPortMalloc(96 /* TCB_t size */);

    *ppxIdleTaskTCBBuffer = (StaticTask_t *)newTCB;
    *ppxIdleTaskStackBuffer = (StackType_t *)idle_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;

    (void)ppxIdleTaskTCBBuffer;
    (void)ppxIdleTaskStackBuffer;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    void *newTCB = pvPortMalloc(96 /* TCB_t size */);

    *ppxTimerTaskTCBBuffer = (StaticTask_t *)newTCB;
    *ppxTimerTaskStackBuffer = (StackType_t *)timer_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;

    (void)ppxTimerTaskTCBBuffer;
    (void)ppxTimerTaskStackBuffer;
}

#define TASK1_STACK_SIZE    256
static uint32_t task1_stack[TASK1_STACK_SIZE];
static TaskHandle_t task1_handle;

#define TASK2_STACK_SIZE    256
static uint32_t task2_stack[TASK1_STACK_SIZE];
static TaskHandle_t task2_handle;

#define TASK3_STACK_SIZE    256
static uint32_t task3_stack[TASK3_STACK_SIZE];
static TaskHandle_t task3_handle;

static TimerHandle_t timer1;

static QueueHandle_t sync_sem;


static void timer1_func(TimerHandle_t xTimer)
{
    LOG("Timer expired\n");
}

static void task1_func(void *arg)
{
    /* create a timer */
    timer1 = xTimerCreate("timer1", pdMS_TO_TICKS(5000), pdTRUE, NULL, timer1_func);

    /* start timer */
    xTimerStart(timer1, portMAX_DELAY);

    /* normally task should not return */
    while (1)
    {
        /* Delay for 1 second */
        vTaskDelay((TickType_t)pdMS_TO_TICKS(1000));

        /* log */
        LOG("Task1 running\n");
    }
}

static void task2_func(void *arg)
{
    /* delay for a while so that task2 starts slightly behind task1 */
    vTaskDelay((TickType_t)pdMS_TO_TICKS(500));

    /* normally task should not return */
    while (1)
    {
        /* Delay for 1 second */
        vTaskDelay((TickType_t)pdMS_TO_TICKS(3000));

        /* log */
        LOG("Task2 signalling task3\n");

        /* semaphore post: to signal task3 */
        xSemaphoreGive(sync_sem);
    }
}

static void task3_func(void *arg)
{
    /* normally task should not return */
    while (1)
    {
        /* semaphore wait: wait for someone to signal */
        xSemaphoreTake(sync_sem, portMAX_DELAY);

        /* log */
        LOG("Task3 signalled\n");
    }
}


static void mainloop(void)
{
    StaticTask_t *newTCB;

    /* create a binary semaphore */
    sync_sem = xSemaphoreCreateCounting(1, 0);

    /* Start task1 */
    newTCB = pvPortMalloc(sizeof(StaticTask_t));
    task1_handle = xTaskCreateStatic(task1_func,
                                     "task1",
                                     TASK1_STACK_SIZE,
                                     NULL,
                                     5,
                                     (StackType_t *)task1_stack,
                                     newTCB);
    if (task1_handle == NULL)
    {
        LOG("error creating task\n");
    }

    /* Start task2 */
    newTCB = pvPortMalloc(sizeof(StaticTask_t));
    task2_handle = xTaskCreateStatic(task2_func,
                                     "task2",
                                     TASK2_STACK_SIZE,
                                     NULL,
                                     5,
                                     (StackType_t *)task2_stack,
                                     newTCB);
    if (task2_handle == NULL)
    {
        LOG("error creating task\n");
    }

    /* Start task3 */
    newTCB = pvPortMalloc(sizeof(StaticTask_t));
    task3_handle = xTaskCreateStatic(task3_func,
                                     "task3",
                                     TASK3_STACK_SIZE,
                                     NULL,
                                     5,
                                     (StackType_t *)task3_stack,
                                     newTCB);
    if (task3_handle == NULL)
    {
        LOG("error creating task\n");
    }


    /* start the scheduler. */
    vTaskStartScheduler();

    /* should not return */
    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t FreeRTOS\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

