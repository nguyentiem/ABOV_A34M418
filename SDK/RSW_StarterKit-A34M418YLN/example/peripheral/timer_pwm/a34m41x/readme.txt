
/**
 * @addtogroup  example_peripheral
 * @{
 * @defgroup    example_timer_pwm TIMER PWM
 * @brief       Timer PWM
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

This example uses timer to generate PWM signal.

Timer is configured to generate PWM signals.
When the timer expires, the interrupt handler registered during initialization is invoked.

Timer clock and prescaler allows application to select different interval ranges.

With PWM mode, duty event and period event can be configured.
PWM mode can also enable PWM signal pulses output to a specified pin.
See also the available pin mux settings.


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
5) Watch the log messages.
   Watch the pin output level changes.
   

@endverbatim
* @} */

