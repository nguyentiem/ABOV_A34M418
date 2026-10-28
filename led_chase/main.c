#include "main.h"
#include "a34m41x_system.h"

#define LED_COUNT 6U
#define STEP_DELAY_MS 200U

static const enum gpio_pin led_pins[LED_COUNT] = {
    PIN0, PIN1, PIN2, PIN3, PIN4, PIN5
};

int main(void)
{
    uint32_t i;

    Port_Init();
    System_Setting_Config();

    for (i = 0; i < LED_COUNT; ++i)
    {
        HAL_GPIO_ClearPin(PORTB, led_pins[i]);
        HAL_GPIO_SetOutput(PORTB, led_pins[i], GPIO_OUTPUT_PUSH_PULL);
        HAL_GPIO_SetMode(PORTB, led_pins[i], GPIO_MODE_OUTPUT);
    }

    while (1)
    {
        for (i = 0; i < LED_COUNT; ++i)
        {
            HAL_GPIO_SetPin(PORTB, led_pins[i]);
            SystemDelay(STEP_DELAY_MS);
            HAL_GPIO_ClearPin(PORTB, led_pins[i]);
        }
    }
}
