/***********************************************************************
* @file		main.c
* @brief	Contains all macro definitions and function prototypes
* 			support for  firmware library	
* @author	ABOV team
*
************************************************************************
*
*ABOV Disclaimer
*IMPORTANT NOTICE – PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, 
*corrections, enhancements, modifications, and improvements to 
*ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness 
*of the information included herein. Purchasers should obtain the 
*latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and 
*use of ABOV products and ABOV assumes no liability for application 
*assistance or the design of purchasers’ products. No license, express 
*or implied, to any intellectual property rights is granted by ABOV 
*herein. ABOV disclaims all express and implied warranties and shall 
*not be responsible or liable for any injuries or damages related to 
*use of ABOV products in such unauthorized applications. ABOV and 
*the ABOV logo are trademarks of ABOV. All other product or service 
*names are the property of their respective owners. Information in 
*this document supersedes and replaces the information previously 
*supplied in any former versions of this document.
*© 2020 ABOV Semiconductor – All rights reserved
**********************************************************************/

/*-----------------------------------------------------------------------------
* Program Description:
/*-----------------------------------------------------------------------------
* This program demonstrates how to configure the StarterKit_A34M418 to transmit or 
* receive by a SPI using polling mode. 

* Board: StarterKit_A34M418   (embeds a A34M418 device)
* SS(Slave Selection) Pin: PA12
* Clock Pin: PA13
* MOSI Pin: PA14
* MISO Pin: PA15
* master transmit/receive, slave transmit/receive
*/

/*-----------------------------------------------------------------------------
 *                          A34M418 Clock
*-----------------------------------------------------------------------------
 *    System Clock source          High Speend External oscillator (HSE)
 *    SYSCLK(MHz)                    8MHz
 *    HCLK(MHz) - Core Clock      8MHz
 *    PCLK(MHz) - Peri Clock        8MHz
*/

/*-----------------------------------------------------------------------------
* par Directory contents 
*-----------------------------------------------------------------------------
  - a34m41x_debug.c             debug file
  - main.h                           Header for main.c module  
  - main.c                           Main program
  - a34m41x_port_init.c           PortInit 
  - a34m41x_system_setting.c   system source file
  - a34m41x_spi.c                 spi driver file
*/

/*-----------------------------------------------------------------------------
*par Hardware and Software environment 
*-----------------------------------------------------------------------------
  - This example runs on A34M418 devices.    
  - This example has been tested with two StarterKit boards embedding
    a A34M418 device and can be easily tailored to any other supported device 
    and development board.
*/

/*-----------------------------------------------------------------------------
*The UART is configured as follows:
*-----------------------------------------------------------------------------
*    - BaudRate = 38400 baud  
*    - Word Length = 8 bits (8 data bits, no parity bit)
*    - One Stop Bit
*    - No parity

*   UART0 clock  - MCCR clock source
*   PC.14   - UART0 RX
*   PC.15   - UART0 TX
*   Interrupt - UART0_IRQn
*/

/*-----------------------------------------------------------------------------
* How To Test: StarterKit_A34M418  board
*-----------------------------------------------------------------------------
* 1) StarterKit_A34M418 Connect. 
* 2) Connect the StarterKit_A34M418  board to a PC using a mini USB cable.
* 2) Compile and download code to the StarterKit_A34M418 board.
*    In  Keil(uVision V5 )Software ,  select Build Target -> Download -> Debug from the menu bar,
*    click the Debug button in the quick menu, or press Ctrl+F5.
* 4) On the PC, open HyperTerminal (or any other terminal program) and connect
*    to the  UART Port at 38400 baud rate and 8-N-1.
* 5) Run the code.
*    In Keil(uVision V5 )Software, select Download  -> Run  from the menu bar,
*    click the Resume button in the quick menu, or press F5.
* 6) Using a terminal program on the PC, input test following commands.
*    { "ms",        cmd_master_send,      "send value to slave", "\targ1 = send len , ex) ms 010\n"},
*    { "sr",        cmd_slave_recv,       "recv value from master", "\targ1 = recv len , ex) sr 010\n"},
*/
