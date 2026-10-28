
/**
 * @addtogroup  example_fs
 * @{
 * @defgroup    example_nvs NVS
 * @brief       Non-Volatile-Storage
 * @verbatim


-----------------------------------------------------------------------------
ABOV Disclaimer
-----------------------------------------------------------------------------

IMPORTANT NOTICE – PLEASE READ CAREFULLY
ABOV Semiconductor ("ABOV") reserves the right to make changes, 
corrections, enhancements, modifications, and improvements to 
ABOV products and/or to this document at any time without notice. 
ABOV does not give warranties as to the accuracy or completeness 
of the information included herein. Purchasers should obtain the 
latest relevant information of ABOV products before placing orders. 
Purchasers are entirely responsible for the choice, selection, and 
use of ABOV products and ABOV assumes no liability for application 
assistance or the design of purchasers’ products. No license, express 
or implied, to any intellectual property rights is granted by ABOV 
herein. ABOV disclaims all express and implied warranties and shall 
not be responsible or liable for any injuries or damages related to 
use of ABOV products in such unauthorized applications. ABOV and 
the ABOV logo are trademarks of ABOV. All other product or service 
names are the property of their respective owners. Information in 
this document supersedes and replaces the information previously 
supplied in any former versions of this document.
© 2020 ABOV Semiconductor – All rights reserved


-----------------------------------------------------------------------------
Program Description
-----------------------------------------------------------------------------

MCU flash memory can be used as a file system.

This example demonstrates a use of NVS library to write or read flash memory
which is configured as a file system. In order to use the flash memory as a
strage, it must be properly erased, written, and read.

NVS allows access to an item with an unique index value. 
[index 1] = [value 1]
[index 2] = [value 2]

Each value can have different sizes.


-----------------------------------------------------------------------------
Clock
-----------------------------------------------------------------------------
System Clock source         High Speed External oscillator (HSE)
SYSCLK(MHz)                 8MHz
HCLK(MHz) - Core Clock      8MHz
PCLK(MHz) - Peri Clock      8MHz


-----------------------------------------------------------------------------
Directory contents 
-----------------------------------------------------------------------------
main.h                      header for main.c module  
main.c                      main program
a34m41x_debug.c             debug file
a34m41x_port_init.c         portInit 
a34m41x_system_setting.c    system source file


-----------------------------------------------------------------------------
Hardware and Software environment 
-----------------------------------------------------------------------------
This example runs on A34M418 devices.    
This example has been tested with two StarterKit boards embedding
a A34M418 device and can be easily tailored to any other supported device 
and development board.


-----------------------------------------------------------------------------
UART debug
-----------------------------------------------------------------------------
BaudRate                    38400 baud  
Word Length                 8 bits (8 data bits, no parity bit)
One Stop Bit
No parity

UART0 clock                 MCCR clock source
PC.14                       UART0 RX
PC.15                       UART0 TX

-----------------------------------------------------------------------------
How To Test: StarterKit_A34M418  board
-----------------------------------------------------------------------------

1) Connect StarterKit_A34M418 board to a PC using a mini USB cable.
2) Compile and download the code to the board.
   In Keil (uVision V5), select [Build Target] -> [Download] -> [Debug] from the menu bar.
   Click the [Debug] button in the quick menu, or press Ctrl+F5.
3) On the PC, open HyperTerminal (or any other terminal program)
   Configure the port at 38400 and 8-N-1
4) Run the code
   In Keil (uVision V5), select [Download] -> [Run] from the menu bar.
   Click the [Resume] button in the quick menu, or press F5.
5) Watch the log messages
   Use the following commands to read or write a value permanently.
    { "ni", cmd_nvs_init, "nvs init" },
    { "nr", cmd_nvs_read, "nvs read", "arg1=id" },
    { "nw", cmd_nvs_write, "nvs write", "arg1=id, arg2=str" },
    { "nd", cmd_nvs_delete, "nvs delete", "arg1=id" },
    { "nc", cmd_nvs_clear, "nvs clear" },
    { "ns", cmd_nvs_space, "nvs space" },
    { "fr", cmd_flash_read, "flash read", "arg1=addr,arg2=len" },
    { "fw", cmd_flash_write, "flash write", "arg1=addr,arg2=str" },
    { "fe", cmd_flash_erase, "flash erase", "arg1=addr" },

   For example, store two items
   # ni
   # nw 1 1234
   # nw 2 abcdef
   Reset the board, and read the items previously written.
   # nr 1
   # nr 2
   Clear the whole contents
   # nc

   Flash memory content can be access with commands such as "fr", "fw", "fe" to test
   how nvs works on actual flash memory contents.

@endverbatim
* @} */

