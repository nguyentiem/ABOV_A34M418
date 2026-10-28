#include "main.h"
#include "a34m41x_system.h"

int main(void)
{
    Port_Init();
    System_Setting_Config();

    HAL_GPIO_SetMode(PORTB, PIN0, GPIO_MODE_OUTPUT);
    HAL_GPIO_SetOutput(PORTB, PIN0, GPIO_OUTPUT_PUSH_PULL);

    while (1)
    {
        HAL_GPIO_SetPin(PORTB, PIN0);
        SystemDelay(500);
        HAL_GPIO_ClearPin(PORTB, PIN0);
        SystemDelay(500);
    }
}
