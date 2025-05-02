#include "MQ2.h"
int gas; // 定义外部变量gas
void ADC_config(){

  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
  
  
  rcu_periph_clock_enable(RCU_ADC0);
  adc_deinit();
  adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
  adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
  adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
  adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_15);
  adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
  adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);//????????????
  adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
  adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

  adc_enable(ADC0);
  delay_1ms(1);
  adc_calibration_enable(ADC0);

   // DMA
   adc_dma_request_after_last_enable(ADC0);
   adc_dma_mode_enable(ADC0);

   // ?ò??ADC
   adc_enable(ADC0);

   // ??×?ADC
   adc_calibration_enable(ADC0);
   delay_1ms(10);

   // ??·????ù
   adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);

}

int ADC_get_gas(){
  
  adc_software_trigger_enable(ADC0 , ADC_ROUTINE_CHANNEL);
  while(SET != adc_flag_get(ADC0, ADC_FLAG_EOC));
  adc_flag_clear(ADC0, ADC_FLAG_EOC);
  uint16_t adc_value = adc_routine_data_read(ADC0);
  float VGAS = (adc_value * 5) / 4095;
  gas = (0.2 - VGAS) * 25+95;
  return gas;
}