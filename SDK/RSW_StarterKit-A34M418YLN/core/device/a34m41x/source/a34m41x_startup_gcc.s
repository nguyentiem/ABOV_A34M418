/**************************************************************************//**
 * @file     a34m41x_startup_gcc.s
 * @brief    CMSIS-Core(M) Device Startup File for A34M41X Device
 * @version  V2.1.0
 * @date     04. August 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

	.syntax	unified
	.arch	armv6-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x00000400
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0x00000000
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .vectors
	.align 2
	.globl	__Vectors
__Vectors:
	.long	__StackTop            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	MemManage_Handler     /* MPU Handler */
	.long	BusFault_Handler      /* BUS Fault Handler */
	.long	UsageFault_Handler    /* UsageFault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	DebugMon_Handler      /* Debug Monitor Handler */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	/* External interrupts */
	.long	LVI_IRQHandler
	.long	SYSCLKFAIL_IRQHandler
	.long	XOSCFAIL_IRQHandler
	.long	SOSCFAIL_IRQHandler
	.long	0
	.long	0
	.long	WDT_IRQHandler
	.long	0
	.long	FRT0_IRQHandler
	.long	FRT1_IRQHandler
	.long	0
	.long	CFMC_IRQHandler
	.long	DFMC_IRQHandler
	.long	0
	.long	0
	.long	TIMER0_IRQHandler
	.long	TIMER1_IRQHandler
	.long	TIMER2_IRQHandler
	.long	TIMER3_IRQHandler
	.long	TIMER4_IRQHandler
	.long	TIMER5_IRQHandler
	.long	TIMER6_IRQHandler
	.long	TIMER7_IRQHandler
	.long	TIMER8_IRQHandler
	.long	TIMER9_IRQHandler
	.long	0
	.long	0
	.long	RNG_IRQHandler
	.long	AES128_IRQHandler
	.long	0
	.long	0
	.long	QEI0_IRQHandler
	.long	QEI1_IRQHandler
	.long	0
	.long	0
	.long	0
	.long	GPIOA_IRQHandler
	.long	GPIOB_IRQHandler
	.long	GPIOC_IRQHandler
	.long	GPIOD_IRQHandler
	.long	GPIOE_IRQHandler
	.long	GPIOF_IRQHandler
	.long	GPIOG_IRQHandler
	.long	0
	.long	0
	.long	MPWM0PROT_IRQHandler
	.long	MPWM0OVV_IRQHandler
	.long	MPWM0U_IRQHandler
	.long	MPWM0V_IRQHandler
	.long	MPWM0W_IRQHandler
	.long	MPWM1PROT_IRQHandler
	.long	MPWM1OVV_IRQHandler
	.long	MPWM1U_IRQHandler
	.long	MPWM1V_IRQHandler
	.long	MPWM1W_IRQHandler
	.long	SPI0_IRQHandler
	.long	SPI1_IRQHandler
	.long	SPI2_IRQHandler
	.long	0
	.long	0
	.long	I2C0_IRQHandler
	.long   I2C1_IRQHandler
	.long	0
	.long   UART0_IRQHandler
	.long   UART0_IRQHandler
	.long   UART0_IRQHandler
	.long   UART0_IRQHandler
	.long   UART0_IRQHandler
	.long   UART0_IRQHandler
	.long	0
	.long   CAN_IRQHandler
	.long	0
	.long   0
	.long   ADC0_IRQHandler
	.long   ADC1_IRQHandler
	.long   ADC2_IRQHandler
	.long	0
	.long   0
	.long   COMP0_IRQHandler
	.long   COMP1_IRQHandler
	.long   COMP2_IRQHandler
	.long   COMP3_IRQHandler
	.long   0
	.long   0
	.long   CRC_IRQHandler

	.size	__Vectors, . - __Vectors

	.text
	.thumb
	.thumb_func
	.align	1
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:

#ifndef __NO_SYSTEM_INIT

.sys_init:

	bl	SystemInit
#endif

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	blt	.L_loop0_0_done
	ldr	r0, [r1, r3]
	str	r0, [r2, r3]
	b	.L_loop0_0

.L_loop0_0_done:
	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

	subs	r3, r2
	ble	.L_loop1_done

.L_loop1:
	subs	r3, #4
	ldr	r0, [r1,r3]
	str	r0, [r2,r3]
	bgt	.L_loop1

.L_loop1_done:
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	blt	.L_loop2_0_done
	str	r0, [r1, r2]
	b	.L_loop2_0
.L_loop2_0_done:

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0

	subs	r2, r1
	ble	.L_loop3_done

.L_loop3:
	subs	r2, #4
	str	r0, [r1, r2]
	bgt	.L_loop3
.L_loop3_done:
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __START
#define __START _start
#endif
	bl	__START

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	.align  1
	.thumb_func
	.weak   HardFault_Handler
	.type   HardFault_Handler, %function
HardFault_Handler:
	movs    r0, #4
	mov     r1, lr
	tst     r0, r1
	beq     __HardFault_MSP
	mrs     r0, psp
	ldr     r1,=HardFault_HandlerC
	bx      r1
__HardFault_MSP:
	mrs     r0, msp
	ldr     r1,=HardFault_HandlerC
	bx      r1

	.align  1
	.thumb_func
	.weak   WDT_IRQHandler
	.type   WDT_IRQHandler, %function
WDT_IRQHandler:
	movs    r0, #4
	mov     r1, lr
	tst     r0, r1
	beq     __WDT_MSP
	mrs     r0, psp
	ldr     r1,=WDT_IRQHandlerC
	bx      r1
__WDT_MSP:
	mrs     r0, msp
	ldr     r1,=WDT_IRQHandlerC
	bx      r1

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_HandlerC
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
    def_irq_handler LVI_IRQHandler
    def_irq_handler SYSCLKFAIL_IRQHandler
    def_irq_handler XOSCFAIL_IRQHandler
    def_irq_handler SOSCFAIL_IRQHandler
    def_irq_handler WDT_IRQHandlerC
    def_irq_handler FRT0_IRQHandler
    def_irq_handler FRT1_IRQHandler
    def_irq_handler CFMC_IRQHandler
    def_irq_handler DFMC_IRQHandler
    def_irq_handler TIMER0_IRQHandler
    def_irq_handler TIMER1_IRQHandler
    def_irq_handler TIMER2_IRQHandler
    def_irq_handler TIMER3_IRQHandler
    def_irq_handler TIMER4_IRQHandler
    def_irq_handler TIMER5_IRQHandler
    def_irq_handler TIMER6_IRQHandler
    def_irq_handler TIMER7_IRQHandler
    def_irq_handler TIMER8_IRQHandler
    def_irq_handler TIMER9_IRQHandler
    def_irq_handler RNG_IRQHandler
    def_irq_handler AES128_IRQHandler
    def_irq_handler QEI0_IRQHandler
    def_irq_handler QEI1_IRQHandler
    def_irq_handler GPIOA_IRQHandler
    def_irq_handler GPIOB_IRQHandler
    def_irq_handler GPIOC_IRQHandler
    def_irq_handler GPIOD_IRQHandler
    def_irq_handler GPIOE_IRQHandler
    def_irq_handler GPIOF_IRQHandler
    def_irq_handler GPIOG_IRQHandler
    def_irq_handler MPWM0PROT_IRQHandler
    def_irq_handler MPWM0OVV_IRQHandler
    def_irq_handler MPWM0U_IRQHandler
    def_irq_handler MPWM0V_IRQHandler
    def_irq_handler MPWM0W_IRQHandler
    def_irq_handler MPWM1PROT_IRQHandler
    def_irq_handler MPWM1OVV_IRQHandler
    def_irq_handler MPWM1U_IRQHandler
    def_irq_handler MPWM1V_IRQHandler
    def_irq_handler MPWM1W_IRQHandler
    def_irq_handler SPI0_IRQHandler
    def_irq_handler SPI1_IRQHandler
    def_irq_handler SPI2_IRQHandler
    def_irq_handler I2C0_IRQHandler
    def_irq_handler I2C1_IRQHandler
    def_irq_handler UART0_IRQHandler
    def_irq_handler UART1_IRQHandler
    def_irq_handler UART2_IRQHandler
    def_irq_handler UART3_IRQHandler
    def_irq_handler UART4_IRQHandler
    def_irq_handler UART5_IRQHandler
    def_irq_handler CAN_IRQHandler
    def_irq_handler ADC0_IRQHandler
    def_irq_handler ADC1_IRQHandler
    def_irq_handler ADC2_IRQHandler
    def_irq_handler COMP0_IRQHandler
    def_irq_handler COMP1_IRQHandler
    def_irq_handler COMP2_IRQHandler
    def_irq_handler COMP3_IRQHandler
    def_irq_handler CRC_IRQHandler

	.end
