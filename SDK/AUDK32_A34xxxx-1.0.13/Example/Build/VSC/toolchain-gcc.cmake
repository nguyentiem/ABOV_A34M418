set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(CMAKE_HOST_WIN32)
    set(BIN_EXT ".exe")
else()
    set(BIN_EXT "")
endif()

if(NOT DEFINED ARM_TOOLCHAIN_PATH)
    set(ARM_TOOLCHAIN_PATH "C:/Program Files (x86)/abov/eMStudio32/bin/13.2 Rel1" CACHE PATH "ARM Toolchain")
endif()

set(CMAKE_C_COMPILER   "${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc${BIN_EXT}" CACHE PATH "C Compiler")
set(CMAKE_ASM_COMPILER "${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc${BIN_EXT}" CACHE PATH "ASM Compiler")

set(ARCH_COMMON_FLAGS "-mcpu=cortex-m4 -mthumb")

set(CMAKE_C_FLAGS_INIT   "${ARCH_COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${ARCH_COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${ARCH_COMMON_FLAGS}")

set(COMPILER_C_FLAGS
   -g
   -O0
   -ffunction-sections
   -fdata-sections
   -std=gnu11
)

set(COMPILER_ASM_FLAGS
    -g
)

set(COMPILER_LINKER_FLAGS
    -Wl,--gc-sections
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,--no-warn-rwx-segment
)
