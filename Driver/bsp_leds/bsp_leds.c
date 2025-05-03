#include "bsp_leds.h"

// ����gpio��ʼ����������Ľṹ��
typedef struct {
  rcu_periph_enum rcu;
  uint32_t port;
  uint32_t pin;
} Led_GPIO_t;

// ��������gpio��Ӧ����������
Led_GPIO_t g_gpio_list[] = {
  {RCU_GPIOC, GPIOC, GPIO_PIN_6},        // LED_SW
  {RCU_GPIOD, GPIOD, GPIO_PIN_8},        // LED1
  {RCU_GPIOD, GPIOD, GPIO_PIN_9},        // LED2
  {RCU_GPIOD, GPIOD, GPIO_PIN_12},       // LED5 - PWM����
  {RCU_GPIOD, GPIOD, GPIO_PIN_11},       // LED4
};
// ���ڼ������鳤�ȵĺ�
#define MAX_LED_COUNT	(sizeof(g_gpio_list) / sizeof(Led_GPIO_t))

// LED5 PWM������ض���
#define LED5_TIMER TIMER3
#define LED5_TIMER_RCU RCU_TIMER3
#define LED5_TIMER_CH TIMER_CH_0
#define LED5_GPIO_AF GPIO_AF_2

// PWM��������
#define PWM_PRESCALER (168-1)  // 168MHz/168 = 1MHz
#define PWM_PERIOD 999  // 1MHz/1000 = 1KHz PWMƵ�ʣ�ռ�ձȷ�Χ0-999
static void GPIO_config(rcu_periph_enum rcu, uint32_t port, uint32_t pin) {
  // ��ʼ��Ϊ�������ģʽ
  rcu_periph_clock_enable(rcu);
  gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
  gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}

// LED5 PWM��ʼ������
static void LED5_PWM_Init(void) {
  // 1. ����GPIOΪ���ù���
  rcu_periph_clock_enable(g_gpio_list[LED5].rcu);
  gpio_mode_set(g_gpio_list[LED5].port, GPIO_MODE_AF, GPIO_PUPD_NONE, g_gpio_list[LED5].pin);
  gpio_output_options_set(g_gpio_list[LED5].port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, g_gpio_list[LED5].pin);
  gpio_af_set(g_gpio_list[LED5].port, LED5_GPIO_AF, g_gpio_list[LED5].pin);
  
  // 2. ���ö�ʱ��
  timer_deinit(LED5_TIMER);
  rcu_periph_clock_enable(LED5_TIMER_RCU);
  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 4*84 = 168MHz
  
  // 3. ��ʼ����ʱ������
  timer_parameter_struct timer_init_param;
  timer_struct_para_init(&timer_init_param);
  timer_init_param.prescaler = PWM_PRESCALER;
  timer_init_param.period = PWM_PERIOD;
  timer_init(LED5_TIMER, &timer_init_param);
  
  // 4. ����PWMģʽ
  timer_oc_parameter_struct oc_config;
  timer_channel_output_struct_para_init(&oc_config);
  oc_config.outputstate = TIMER_CCX_ENABLE;
  oc_config.ocpolarity = TIMER_OC_POLARITY_HIGH;
  timer_channel_output_config(LED5_TIMER, LED5_TIMER_CH, &oc_config);
  
  // 5. ����PWMģʽ
  timer_channel_output_mode_config(LED5_TIMER, LED5_TIMER_CH, TIMER_OC_MODE_PWM0);
  timer_channel_output_pulse_value_config(LED5_TIMER, LED5_TIMER_CH, 0); // ��ʼռ�ձ�Ϊ0
  
  // 6. ������ʱ��
  timer_enable(LED5_TIMER);
}

void led_init() {

  uint8_t count = MAX_LED_COUNT;
  for(uint8_t i = 0; i < count; i++) {
    // ����LED5������ʹ��PWM����
    if(i == LED5) continue;
    
    Led_GPIO_t gpio = g_gpio_list[i];
    // ��ʼ��
    GPIO_config(gpio.rcu,gpio.port, gpio.pin);
    // Ĭ��ȫ������(�ر�)
    gpio_bit_write(gpio.port, gpio.pin, SET);
  }

  // �ܿ�������(��)
  gpio_bit_write(g_gpio_list[0].port, g_gpio_list[0].pin, RESET);
  
  // ��ʼ��LED5��PWM����
  LED5_PWM_Init();
}

// ����
void led_turn_on(uint8_t led_index) {
  // �����LED5����ʹ���������
  if(led_index == LED5) {
    led_set_brightness(PWM_PERIOD);
    return;
  }
  
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, RESET);
}

// �ص�
void led_turn_off(uint8_t led_index) {
  // �����LED5������������Ϊ0
  if(led_index == LED5) {
    led_set_brightness(0);
    return;
  }
  
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, SET);
}

// ����LED5������ (0-999��0Ϊ�رգ�999Ϊ����)
void led_set_brightness(uint16_t brightness) {
  // �������ȷ�Χ
  if(brightness > PWM_PERIOD) {
    brightness = PWM_PERIOD;
  }
  
  // ����PWMռ�ձ�
  timer_channel_output_pulse_value_config(LED5_TIMER, LED5_TIMER_CH, brightness);
}