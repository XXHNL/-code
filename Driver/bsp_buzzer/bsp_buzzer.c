#include "bsp_buzzer.h"
//			   C	 D    E 	F	 G	 A	  B	   C`
//u16 hz[] = {523, 587, 659, 698, 784, 880, 988, 1047};

//            C			D			E			 F			G			A			B		  C`
u16 hz[] = { 1047, 1175, 1319,  1397, 1568, 1760, 1976, 2093 };
// >= 2564
void timer_gpio_config(uint32_t gpio_rcu, uint32_t gpio_port, uint32_t gpio_pin, uint32_t gpio_af){
    rcu_periph_clock_enable(gpio_rcu);                                             
		/* ����gpioģʽ */															  
    gpio_mode_set(gpio_port, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_pin);              
    gpio_output_options_set(gpio_port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, gpio_pin);  
    gpio_af_set(gpio_port, gpio_af, gpio_pin);                                     
}

void timer_init_config(uint32_t timer_periph, uint16_t t_prescaler, uint32_t t_period) {
    /*��ʼ������ */
    timer_parameter_struct initpara;
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&initpara);
    /* ������Ҫ����ֵ ��Ƶϵ�� ������ʵ�ָ��͵�timerƵ�ʣ� */
    initpara.prescaler 	= t_prescaler - 1;
    /* 1�����ڵļ���(period Max: 65535) Freq >= 2564  */
    initpara.period		= t_period - 1;
    /* initialize TIMER counter */
    timer_init(timer_periph, &initpara);
    /* enable a TIMER */
    timer_enable(timer_periph);
}


void timer_channel_config(uint32_t timer_periph, uint16_t channel) {
    /* TIMER ͨ��������� */
    timer_oc_parameter_struct ocpara;
    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&ocpara);
    ocpara.outputstate  = (uint16_t)TIMER_CCX_ENABLE;
    /* ����������� configure TIMER channel output function */
    timer_channel_output_config(timer_periph, channel, &ocpara);
    /* ����ͨ���������Ƚ�ģʽ configure TIMER channel output compare mode */
    timer_channel_output_mode_config(timer_periph, channel, TIMER_OC_MODE_PWM0);
}

#define TIMER_RCU		  RCU_TIMER1
#define TIMER_PERIPH 	TIMER1
#define TIMER_CHANNEL	TIMER_CH_1
#define PRESCALER  		10

void Buzzer_init() {
    // PB9 Timer1_CH1 AF2
    // GPIO ----------------------------------------
    timer_gpio_config(RCU_GPIOB, GPIOB, GPIO_PIN_9, GPIO_AF_1);

    // TIMER----------------------------------------
    rcu_periph_clock_enable(TIMER_RCU);
    timer_deinit(TIMER_PERIPH);
    /* ����Ƶ��*/
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    // ͨ������ -------------------------------------
    timer_channel_config(TIMER_PERIPH, TIMER_CHANNEL);
}

void Buzzer_beep(u8 hz_val_index) { // 1,2,3,4 ... 7

    u16 hz_val = hz[hz_val_index - 1];
    Buzzer_play(hz_val);
}

void Buzzer_play(u16 hz_val) {
    // ����ϵͳ��Ƶ�ͷ�Ƶϵ������������
    uint16_t period = SystemCoreClock / (hz_val * PRESCALER);
    // ���÷�Ƶϵ�������ڼ���ֵ
    timer_init_config(TIMER_PERIPH, PRESCALER, period);
    // �����������ֵ����Χ��[0, ���ڼ���ֵ)
    timer_channel_output_pulse_value_config(TIMER_PERIPH, TIMER_CHANNEL, (uint32_t)(period * 0.5f));
}

void Buzzer_stop() {
    timer_disable(TIMER_PERIPH);
}


void Beep_node(u8 index){
        if(index == Low){
            for(int i = 0; i < index; i++){
            Buzzer_beep(7);
            delay_1ms(30);
            Buzzer_stop();
            delay_1ms(30);
             }
        }else if(index == Medium){
            for(int i = 0; i < index; i++){
            Buzzer_beep(7);
            delay_1ms(20);
            Buzzer_stop();
            delay_1ms(20);
             }
        }else if(index == Hign){
            for(int i = 0; i < index; i++){
            Buzzer_beep(7);
            delay_1ms(15);
            Buzzer_stop();
            delay_1ms(15);
             }
        }else{
            for(int i = 0; i < index; i++){
                Buzzer_beep(7);
            }
        }
   
}