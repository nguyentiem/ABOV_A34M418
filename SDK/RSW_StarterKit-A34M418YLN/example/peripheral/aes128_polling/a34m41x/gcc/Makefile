TARGET		= app

SRCS_APP	+= \
		../../main.c \
		../a34m41x_debug.c \
		../a34m41x_port_init.c \
		../a34m41x_system_setting.c \

SRCS_CORE	+= \
		../../../../../core/device/a34m41x/source/a34m41x_startup_gcc.s \
		../../../../../core/device/a34m41x/source/a34m41x_system.c \

SRCS_DRIVER	+= \
		../../../../../driver/a34m41x/source/a34m41x_aes128.c \
		../../../../../driver/a34m41x/source/a34m41x_dma.c \
		../../../../../driver/a34m41x/source/a34m41x_gpio.c \
		../../../../../driver/a34m41x/source/a34m41x_scu.c \
		../../../../../driver/a34m41x/source/a34m41x_uart.c \

SRCS_LIBRARY	+= \
		../../../../../library/debug/source/debug_cmd.c \
		../../../../../library/debug/source/debug_hardfault.c \
		../../../../../library/debug/source/debug_log.c \
		../../../../../library/debug/source/debug_retarget.c \
		../../../../../library/debug/source/debug_serial.c \
		../../../../../library/tinycrypt/lib/source/utils.c \
		../../../../../library/tinycrypt/lib/source/aes_decrypt.c \
		../../../../../library/tinycrypt/lib/source/aes_encrypt.c \

SRCS		+= \
		$(SRCS_APP) $(SRCS_CORE) $(SRCS_DRIVER) $(SRCS_LIBRARY)

INCS		+= \
		../ \
		../../ \
		../../../../../core/cmsis/include \
		../../../../../core/device/a34m41x/include \
		../../../../../driver/a34m41x/include \
		../../../../../library/debug/include \
		../../../../../library/tinycrypt/lib/include \
		../../../../../toolchain/gcc/linker \

DEFS		+= CONFIG_DEBUG=1 CONFIG_DEBUG_SERIAL_FIFO=1
		
LDS		+= \
		../../../../../toolchain/gcc/linker/a34m41x.ld

include ../../../../../toolchain/gcc/makefile/Makefile.a34m41x

