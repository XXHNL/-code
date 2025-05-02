#include "bsp_timer8.h"

//GPIO���ų�ʼ��
static void gpio_config(void){
	//PE5
	rcu_periph_clock_enable(RCU_GPIOE);
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
	//���ø��ù��ܱ��
	gpio_af_set(GPIOE,GPIO_AF_3,GPIO_PIN_5);
}
//timer3��ʼ��
static void timer8_config(void){
	/****************** 1.��ʱ��������������(������ʱ��) ******************/
	//���ö�ʱ��
	timer_deinit(TIMER8);
	//1.��ʱ��
	rcu_periph_clock_enable(RCU_TIMER8);
	//*����ʱ����ʱ�ӱ�Ƶ��168M(��������)
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//4*84  168M
	//2.��ʼ��
	timer_parameter_struct initpara;
	//�ṹ���ʼ��
	timer_struct_para_init(&initpara);
//	uint16_t clockdivision;                     ʱ�ӽ�Ƶ(���벶����õ�)
//	uint8_t  repetitioncounter;                 �ظ�����(ֻ�и߼���ʱ�����������)
//	uint16_t alignedmode;                       ���뷽ʽ  TIMER_COUNTER_EDGE ��Ե����
//	uint16_t counterdirection;                  ��������  TIMER_COUNTER_UP ���ϼ���(���ж�ʱ����֧��)
	
	//Ԥ��Ƶ�������ڲ����Զ���1
	//��Ƶ  100  168000000/16800 10000hz
	//84000000    Ԥ��Ƶ:16800  Ƶ��:84000000/16800   Ƶ��:5000hz  5000<=>1s
	initpara.prescaler = PRESCALER;  //Ԥ��Ƶ
	//��װ��ֵ  ������ʱ����ʱ��   168000000
	initpara.period = PERIOD;     //����(��װ��ֵ) ���ܳ���65535
	//��ʱ����ʼ��
	timer_init(TIMER8,&initpara);
//	//��nvic�ж�����
//	nvic_irq_enable(TIMER3_IRQn,2,2);
//	//�򿪶�ʱ���ж�(���ϼ����ж�)
//	timer_interrupt_enable(TIMER3,TIMER_INT_UP);


	/****************** ���϶�ʱ�����PWM���� ******************/
	//��ʱ�����pwm�Ľṹ��
	timer_oc_parameter_struct ocpara;
	//�ṹ���ʼ��
	timer_channel_output_struct_para_init(&ocpara);
	//�򿪻��ǹر�ͨ��
	ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
//	ocpara->outputnstate = TIMER_CCXN_DISABLE;
	ocpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
//	ocpara->ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
//	ocpara->ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
//	ocpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	
	//pwm�������
	timer_channel_output_config(TIMER8,TIMER_CH_0,&ocpara);
	//���ģʽ����(PWM0Ĭ�ϸߵ�ƽ  PWM1Ĭ�ϵ͵�ƽ)
	timer_channel_output_mode_config(TIMER8,TIMER_CH_0,TIMER_OC_MODE_PWM0);
	//ռ�ձ�  ����3:ռ�ձȼ���ֵ
	timer_channel_output_pulse_value_config(TIMER8,TIMER_CH_0,0);
	//3.ʹ�ܶ�ʱ��
	timer_enable(TIMER8);
}

//timer8��ʼ��
void bsp_timer8_config(void){
	//��ʼ��
	gpio_config();
	//��ʱ������
	timer8_config();
}

//����ռ�ձ�
void bsp_timer8_set_pulse_value(uint16_t value){
	timer_channel_output_pulse_value_config(TIMER8,TIMER_CH_0,value);
}