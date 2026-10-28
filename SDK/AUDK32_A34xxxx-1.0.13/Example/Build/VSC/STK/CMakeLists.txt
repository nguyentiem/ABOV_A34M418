cmake_minimum_required(VERSION 3.15)

string(TOLOWER "${AUDK32_TARGET}" AUDK32_TARGET_LOWER)
string(TOLOWER "${TARGET_BOARD}" TARGET_BOARD_LOWER)
string(TOLOWER "${PROJECT}" PROJECT_LOWER)
string(TOLOWER "${PRODUCT}" PRODUCT_LOWER)

project(${PROJECT_LOWER} C ASM)

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../../../../")

file(MAKE_DIRECTORY "${PROJECT_DIR}/.vscode")
configure_file("${CMAKE_SOURCE_DIR}/../debug-launch.json" "${PROJECT_DIR}/.vscode/launch.json" @ONLY)

include("${CMAKE_SOURCE_DIR}/../common.cmake")

file(GLOB_RECURSE EXAMPLE_SOURCES "${PROJECT_DIR}/Example/Source/GpioLedBlinking/*.c")

add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${EXAMPLE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

target_compile_options(${PROJECT_NAME}
    PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COMPILER_C_FLAGS}>
        $<$<COMPILE_LANGUAGE:ASM>:${COMPILER_ASM_FLAGS}>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        EXTRN_ABOV_EXAMPLE_CONFIG
        EXTRN_SUBFAMILY_${AUDK32_TARGET}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${COMMON_INCLUDES}"
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        ${COMPILER_LINKER_FLAGS}
        $<$<C_COMPILER_ID:GNU>:-T${PROJECT_DIR}/Framework/CMSIS/Device/ABOV/Source/GCC/gcc_${AUDK32_TARGET_LOWER}.ld>
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>.map>
)
