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

#include "cmsis_os2.h"
#include "RTX_Config.h"


#define THREAD1_STACK_SIZE    256
static uint64_t thread1_stack[THREAD1_STACK_SIZE / 8];
static osThreadId_t thread1_id;
static const osThreadAttr_t thread1_attr =
{
    .stack_mem = thread1_stack,
    .stack_size = sizeof(thread1_stack),
};

#define THREAD2_STACK_SIZE    256
static uint64_t thread2_stack[THREAD1_STACK_SIZE / 8];
static osThreadId_t thread2_id;
static const osThreadAttr_t thread2_attr =
{
    .stack_mem = thread2_stack,
    .stack_size = sizeof(thread2_stack),
};

#define THREAD3_STACK_SIZE    256
static uint64_t thread3_stack[THREAD3_STACK_SIZE / 8];
static osThreadId_t thread3_id;
static const osThreadAttr_t thread3_attr =
{
    .stack_mem = thread3_stack,
    .stack_size = sizeof(thread3_stack),
};


static osTimerId_t timer1;

static osSemaphoreId_t sync_sem;


static void timer1_func(void *arg)
{
    LOG("Timer expired\n");
}

static void thread1_func(void *arg)
{
    /* create a timer */
    timer1 = osTimerNew(timer1_func, osTimerPeriodic, NULL, NULL);

    /* start timer */
    osTimerStart(timer1, 5000);

    /* normally thread should not return */
    while (1)
    {
        /* Delay for 1 second */
        osDelay(1000);

        /* log */
        LOG("Task1 running\n");
    }
}

static void thread2_func(void *arg)
{
    /* delay for a while so that thread2 starts slightly behind thread1 */
    osDelay(500);

    /* normally thread should not return */
    while (1)
    {
        /* Delay for 3 second */
        osDelay(3000);

        /* log */
        LOG("Task2 signalling thread3\n");

        /* semaphore post: to signal thread3 */
        osSemaphoreRelease(sync_sem);
    }
}

static void thread3_func(void *arg)
{
    /* normally thread should not return */
    while (1)
    {
        /* semaphore wait: wait for someone to signal */
        osSemaphoreAcquire(sync_sem, osWaitForever);

        /* log */
        LOG("Task3 signalled\n");
    }
}


static void mainloop(void)
{
    /* initialize the kernel */
    osKernelInitialize();

    /* create a binary semaphore */
    sync_sem = osSemaphoreNew(1, 0, NULL);

    /* Start thread1 */
    thread1_id = osThreadNew(thread1_func, NULL, &thread1_attr);
    if (thread1_id == NULL)
    {
        LOG("error creating thread\n");
    }

    /* Start thread2 */
    thread2_id = osThreadNew(thread2_func, NULL, &thread2_attr);
    if (thread2_id == NULL)
    {
        LOG("error creating thread\n");
    }

    /* Start thread3 */
    thread3_id = osThreadNew(thread3_func, NULL, &thread3_attr);
    if (thread3_id == NULL)
    {
        LOG("error creating thread\n");
    }

    /* start the kernel. */
    osKernelStart();

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
    LOG("\t RTX\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

