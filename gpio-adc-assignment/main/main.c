#include <stdio.h>
#include "driver/gpio.h"
#include "driver/dac_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    // Configure gpios as output
    gpio_config_t io_config = {
        .pin_bit_mask = ((1ULL << GPIO_NUM_23) | (1ULL << GPIO_NUM_22) | (1ULL << GPIO_NUM_19) | (1ULL << GPIO_NUM_18)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_config);

    // Configure dac channel 0 
    dac_oneshot_handle_t led_channel_0_handle;
    dac_oneshot_config_t led_channel_0 = {
        .chan_id = DAC_CHAN_0
    };

    // register a channel with a handle
    dac_oneshot_new_channel(&led_channel_0, &led_channel_0_handle);

    // Configure dac channel 1
    dac_oneshot_handle_t led_channel_1_handle;
    dac_oneshot_config_t led_channel_1 = {
        .chan_id = DAC_CHAN_1
    };

    // register a channel with a handle
    dac_oneshot_new_channel(&led_channel_1, &led_channel_1_handle);

    while(1){   
        //Turn all leds off except 23 and 19
        gpio_set_level(GPIO_NUM_23, 1);
        gpio_set_level(GPIO_NUM_19, 1);
        gpio_set_level(GPIO_NUM_22, 0);
        gpio_set_level(GPIO_NUM_18, 0);
        dac_oneshot_output_voltage(led_channel_0_handle, 0);
        dac_oneshot_output_voltage(led_channel_1_handle, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        //Turn all leds off except 22 and 18
        gpio_set_level(GPIO_NUM_23, 0);
        gpio_set_level(GPIO_NUM_19, 0);
        gpio_set_level(GPIO_NUM_22, 1);
        gpio_set_level(GPIO_NUM_18, 1);
        dac_oneshot_output_voltage(led_channel_0_handle, 0);
        dac_oneshot_output_voltage(led_channel_1_handle, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);

        //Turn all leds off increase channel 0 and 1 from 0 to 255 in 30 step
        gpio_set_level(GPIO_NUM_23, 0);
        gpio_set_level(GPIO_NUM_19, 0);
        gpio_set_level(GPIO_NUM_22, 1);
        gpio_set_level(GPIO_NUM_18, 1);
        
        for (int i = 0; i <= 255; i+=30)
        {
            dac_oneshot_output_voltage(led_channel_0_handle, i);
            dac_oneshot_output_voltage(led_channel_1_handle, i);
            vTaskDelay(500/portTICK_PERIOD_MS); 
        }
     
        //Turn all leds off decrease channel 0 from 0 to 255 and increase chan 1 from 0 to 255 in 30 step
        for (int i = 255, j = 0; i >= 0 && j <= 255; i= i-30, j = j+30)
        {         
            dac_oneshot_output_voltage(led_channel_0_handle, i);
            dac_oneshot_output_voltage(led_channel_1_handle, j);
            vTaskDelay(1000/portTICK_PERIOD_MS); 
        }

        //Turn all leds off except 23 and 19
        gpio_set_level(GPIO_NUM_23, 0);
        gpio_set_level(GPIO_NUM_19, 0);
        gpio_set_level(GPIO_NUM_22, 0);
        gpio_set_level(GPIO_NUM_18, 0);
        dac_oneshot_output_voltage(led_channel_0_handle, 0);
        dac_oneshot_output_voltage(led_channel_1_handle, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
