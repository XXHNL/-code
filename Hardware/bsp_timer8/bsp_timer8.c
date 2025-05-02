#include "bsp_timer8.h"

//GPIO引脚初始化
static void gpio_config(void){
	//PE5
	rcu_periph_clock_enable(RCU_GPIOE);
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
	//设置复用功能编号
	gpio_af_set(GPIOE,GPIO_AF_3,GPIO_PIN_5);
}
//timer3初始化
static void timer8_config(void){
	/****************** 1.定时器基本功能配置(基础定时器) ******************/
	//重置定时器
	timer_deinit(TIMER8);
	//1.打开时钟
	rcu_periph_clock_enable(RCU_TIMER8);
	//*将定时器的时钟倍频到168M(容易忘记)
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//4*84  168M
	//2.初始化
	timer_parameter_struct initpara;
	//结构体初始化
	timer_struct_para_init(&initpara);
//	uint16_t clockdivision;                     时钟降频(输入捕获会用到)
//	uint8_t  repetitioncounter;                 重复计数(只有高级定时器有这个功能)
//	uint16_t alignedmode;                       对齐方式  TIMER_COUNTER_EDGE 边缘触发
//	uint16_t counterdirection;                  计数方向  TIMER_COUNTER_UP 向上计数(所有定时器都支持)
	
	//预分频和周期内部会自动加1
	//降频  100  168000000/16800 10000hz
	//84000000    预分频:16800  频率:84000000/16800   频率:5000hz  5000<=>1s
	initpara.prescaler = PRESCALER;  //预分频
	//重装载值  决定定时器的时间   168000000
	initpara.period = PERIOD;     //周期(重装载值) 不能超过65535
	//定时器初始化
	timer_init(TIMER8,&initpara);
//	//打开nvic中断请求
//	nvic_irq_enable(TIMER3_IRQn,2,2);
//	//打开定时器中断(向上计数中断)
//	timer_interrupt_enable(TIMER3,TIMER_INT_UP);


	/****************** 加上定时器输出PWM功能 ******************/
	//定时器输出pwm的结构体
	timer_oc_parameter_struct ocpara;
	//结构体初始化
	timer_channel_output_struct_para_init(&ocpara);
	//打开还是关闭通道
	ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
//	ocpara->outputnstate = TIMER_CCXN_DISABLE;
	ocpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
//	ocpara->ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
//	ocpara->ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
//	ocpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	
	//pwm输出配置
	timer_channel_output_config(TIMER8,TIMER_CH_0,&ocpara);
	//输出模式配置(PWM0默认高电平  PWM1默认低电平)
	timer_channel_output_mode_config(TIMER8,TIMER_CH_0,TIMER_OC_MODE_PWM0);
	//占空比  参数3:占空比计数值
	timer_channel_output_pulse_value_config(TIMER8,TIMER_CH_0,0);
	//3.使能定时器
	timer_enable(TIMER8);
}

//timer8初始化
void bsp_timer8_config(void){
	//初始化
	gpio_config();
	//定时器配置
	timer8_config();
}

//设置占空比
void bsp_timer8_set_pulse_value(uint16_t value){
	timer_channel_output_pulse_value_config(TIMER8,TIMER_CH_0,value);
}