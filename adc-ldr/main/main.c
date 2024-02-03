#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int ldr_values;

void app_main(void)
{
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
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    
}
