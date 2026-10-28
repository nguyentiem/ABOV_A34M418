TARGET = gpio_blink_pb0
SDK_ROOT := $(abspath ../../../SDK/RSW_StarterKit-A34M418YLN)

SRCS += ../../main.c \
        ../a34m41x_port_init.c \
        ../a34m41x_system_setting.c \
        $(SDK_ROOT)/core/device/a34m41x/source/a34m41x_startup_gcc.s \
        $(SDK_ROOT)/core/device/a34m41x/source/a34m41x_system.c \
        $(SDK_ROOT)/driver/a34m41x/source/a34m41x_gpio.c \
        $(SDK_ROOT)/driver/a34m41x/source/a34m41x_scu.c

INCS += ../ \
        $(SDK_ROOT)/core/cmsis/include \
        $(SDK_ROOT)/core/device/a34m41x/include \
        $(SDK_ROOT)/driver/a34m41x/include

LDS += $(SDK_ROOT)/toolchain/gcc/linker/a34m41x.ld

include $(SDK_ROOT)/toolchain/gcc/makefile/Makefile.a34m41x
