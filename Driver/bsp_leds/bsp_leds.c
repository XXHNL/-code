#include "bsp_leds.h"

// 声明gpio初始化所需参数的结构体
typedef struct {
  rcu_periph_enum rcu;
  uint32_t port;
  uint32_t pin;
} Led_GPIO_t;

// 声明所有gpio对应参数的数组
Led_GPIO_t g_gpio_list[] = {
  {RCU_GPIOC, GPIOC, GPIO_PIN_6},        // LED_SW
  {RCU_GPIOD, GPIOD, GPIO_PIN_8},        // LED1
  {RCU_GPIOD, GPIOD, GPIO_PIN_9},        // LED2
  {RCU_GPIOD, GPIOD, GPIO_PIN_12},       // LED5 - PWM控制
  {RCU_GPIOD, GPIOD, GPIO_PIN_11},       // LED4
};
// 用于计算数组长度的宏
#define MAX_LED_COUNT	(sizeof(g_gpio_list) / sizeof(Led_GPIO_t))

// LED5 PWM控制相关定义
#define LED5_TIMER TIMER3
#define LED5_TIMER_RCU RCU_TIMER3
#define LED5_TIMER_CH TIMER_CH_0
#define LED5_GPIO_AF GPIO_AF_2

// PWM参数定义
#define PWM_PRESCALER (168-1)  // 168MHz/168 = 1MHz
#define PWM_PERIOD 999  // 1MHz/1000 = 1KHz PWM频率，占空比范围0-999
static void GPIO_config(rcu_periph_enum rcu, uint32_t port, uint32_t pin) {
  // 初始化为推挽输出模式
  rcu_periph_clock_enable(rcu);
  gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
  gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}

// LED5 PWM初始化函数
static void LED5_PWM_Init(void) {
  // 1. 配置GPIO为复用功能
  rcu_periph_clock_enable(g_gpio_list[LED5].rcu);
  gpio_mode_set(g_gpio_list[LED5].port, GPIO_MODE_AF, GPIO_PUPD_NONE, g_gpio_list[LED5].pin);
  gpio_output_options_set(g_gpio_list[LED5].port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, g_gpio_list[LED5].pin);
  gpio_af_set(g_gpio_list[LED5].port, LED5_GPIO_AF, g_gpio_list[LED5].pin);
  
  // 2. 配置定时器
  timer_deinit(LED5_TIMER);
  rcu_periph_clock_enable(LED5_TIMER_RCU);
  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 4*84 = 168MHz
  
  // 3. 初始化定时器参数
  timer_parameter_struct timer_init_param;
  timer_struct_para_init(&timer_init_param);
  timer_init_param.prescaler = PWM_PRESCALER;
  timer_init_param.period = PWM_PERIOD;
  timer_init(LED5_TIMER, &timer_init_param);
  
  // 4. 配置PWM模式
  timer_oc_parameter_struct oc_config;
  timer_channel_output_struct_para_init(&oc_config);
  oc_config.outputstate = TIMER_CCX_ENABLE;
  oc_config.ocpolarity = TIMER_OC_POLARITY_HIGH;
  timer_channel_output_config(LED5_TIMER, LED5_TIMER_CH, &oc_config);
  
  // 5. 设置PWM模式
  timer_channel_output_mode_config(LED5_TIMER, LED5_TIMER_CH, TIMER_OC_MODE_PWM0);
  timer_channel_output_pulse_value_config(LED5_TIMER, LED5_TIMER_CH, 0); // 初始占空比为0
  
  // 6. 启动定时器
  timer_enable(LED5_TIMER);
}

void led_init() {

  uint8_t count = MAX_LED_COUNT;
  for(uint8_t i = 0; i < count; i++) {
    // 跳过LED5，它将使用PWM控制
    if(i == LED5) continue;
    
    Led_GPIO_t gpio = g_gpio_list[i];
    // 初始化
    GPIO_config(gpio.rcu,gpio.port, gpio.pin);
    // 默认全部拉高(关闭)
    gpio_bit_write(gpio.port, gpio.pin, SET);
  }

  // 总开关拉低(打开)
  gpio_bit_write(g_gpio_list[0].port, g_gpio_list[0].pin, RESET);
  
  // 初始化LED5的PWM控制
  LED5_PWM_Init();
}

// 开灯
void led_turn_on(uint8_t led_index) {
  // 如果是LED5，则使用最大亮度
  if(led_index == LED5) {
    led_set_brightness(PWM_PERIOD);
    return;
  }
  
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, RESET);
}

// 关灯
void led_turn_off(uint8_t led_index) {
  // 如果是LED5，则设置亮度为0
  if(led_index == LED5) {
    led_set_brightness(0);
    return;
  }
  
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, SET);
}

// 设置LED5的亮度 (0-999，0为关闭，999为最亮)
void led_set_brightness(uint16_t brightness) {
  // 限制亮度范围
  if(brightness > PWM_PERIOD) {
    brightness = PWM_PERIOD;
  }
  
  // 设置PWM占空比
  timer_channel_output_pulse_value_config(LED5_TIMER, LED5_TIMER_CH, brightness);
}