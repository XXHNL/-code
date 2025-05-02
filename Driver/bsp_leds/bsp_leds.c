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
  {RCU_GPIOD, GPIOD, GPIO_PIN_10},       // LED3
  {RCU_GPIOD, GPIOD, GPIO_PIN_11},       // LED4
};
// 用于计算数组长度的宏
#define MAX_LED_COUNT	(sizeof(g_gpio_list) / sizeof(Led_GPIO_t))
static void GPIO_config(rcu_periph_enum rcu, uint32_t port, uint32_t pin) {
  // 初始化为推挽输出模式
  rcu_periph_clock_enable(rcu);
  gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
  gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}

void led_init() {

  uint8_t count = MAX_LED_COUNT;
  for(uint8_t i = 0; i < count; i++) {
    Led_GPIO_t gpio = g_gpio_list[i];
    // 初始化
    GPIO_config(gpio.rcu,gpio.port, gpio.pin);
    // 默认全部拉高(关闭)
    gpio_bit_write(gpio.port, gpio.pin, SET);
  }

  // 总开关拉低(打开)
  gpio_bit_write(g_gpio_list[0].port, g_gpio_list[0].pin, RESET);
}
// 开灯
void led_turn_on(uint8_t led_index) {
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, RESET);
}

// 关灯
void led_turn_off(uint8_t led_index) {
  Led_GPIO_t gpio = g_gpio_list[led_index];
  gpio_bit_write(gpio.port, gpio.pin, SET);
}