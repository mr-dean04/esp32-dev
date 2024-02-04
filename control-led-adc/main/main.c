#include <stdio.h>
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Use a ldr to control/blink a led
 * When the ldr values are above the threshold, led should be off
 * When the ldr values are below the threshold, led should be on
*/

#define LDR_THRESHOLD 1500

int ldr_values;

void app_main(void)
{
    //configure the led as an output
    gpio_config_t gpio_output = {
        .pin_bit_mask = GPIO_NUM_23,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    }; 
    gpio_config(&gpio_output);
    
    // Create a resource allocation for adc unit
    adc_oneshot_unit_init_cfg_t ldr_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };

    adc_oneshot_unit_handle_t ldr_handle;
    adc_oneshot_new_unit(&ldr_config, &ldr_handle);
    
    // configure unit
    adc_oneshot_chan_cfg_t ldr_channel_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(ldr_handle, ADC_CHANNEL_6, &ldr_channel_cfg); 
      
    while(1){
        //read value from the unit
        adc_oneshot_read(ldr_handle, ADC_CHANNEL_6, &ldr_values);
        printf("The ldr values are %d\n", ldr_values);
        if (ldr_values >= LDR_THRESHOLD){
            gpio_set_level(GPIO_NUM_23, 0);
        }
        else {
            gpio_set_level(GPIO_NUM_23, 1);
        }
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}
