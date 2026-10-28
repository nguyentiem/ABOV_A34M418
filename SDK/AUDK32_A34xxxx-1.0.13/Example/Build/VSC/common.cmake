set(COMMON_SOURCES
    "${PROJECT_DIR}/Framework/CMSIS/Device/ABOV/Source/startup_${PRODUCT_LOWER}.c"
    "${PROJECT_DIR}/Framework/CMSIS/Device/ABOV/Source/system_${PRODUCT_LOWER}.c"

    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug.c"
    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug_cmd.c"
    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug_hardfault.c"
    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug_log.c"
    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug_retarget.c"
    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Source/debug_serial.c"
)

file(GLOB_RECURSE PLATFORM_HAL_SOURCES "${PROJECT_DIR}/Platform/HAL/*.c")
list(APPEND COMMON_SOURCES ${PLATFORM_HAL_SOURCES})

set(COMMON_INCLUDES
    "${PROJECT_DIR}/Framework/CMSIS/Core/Include"
    "${PROJECT_DIR}/Framework/CMSIS/Device/ABOV/Include"

    "${PROJECT_DIR}/ProductConfig/Config"

    "${PROJECT_DIR}/Platform/HAL/Include"
    "${PROJECT_DIR}/Platform/HAL/HPL/Include"

    "${PROJECT_DIR}/Platform/Library/ABOV/Debug/Include"
)
