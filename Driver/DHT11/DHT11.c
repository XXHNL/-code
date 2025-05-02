#include "DHT11.h"
#include "bsp_usart0.h"



static void  DHT11_Out(){
	rcu_periph_clock_enable(PIN1_RCU);
	gpio_mode_set(PIN1_PORT ,GPIO_MODE_OUTPUT , GPIO_PUPD_PULLUP , PIN1_PIN);
	gpio_output_options_set(PIN1_PORT , GPIO_OTYPE_PP , GPIO_OSPEED_MAX , PIN1_PIN);
}


static void DHT11_In(){
	rcu_periph_clock_enable(PIN1_RCU);
	gpio_mode_set(PIN1_PORT ,GPIO_MODE_INPUT , GPIO_PUPD_PULLUP , PIN1_PIN);
}


static void DHT11_Start(){

	DHT11_Out();

	gpio_bit_reset(PIN1_PORT , PIN1_PIN);

	delay_1ms(20);

	gpio_bit_set(PIN1_PORT , PIN1_PIN);
	delay_1us(20);
}


static uint8_t DHT11_Check(){

	uint8_t count = 0;

	DHT11_In();


	while(gpio_input_bit_get(PIN1_PORT , PIN1_PIN) == 0&& count < 100){
		count++;
		delay_1us(1);
	}
	if(count >= 100){
		return 1;
	}else{
		count = 0;
	}

	while(gpio_input_bit_get(PIN1_PORT , PIN1_PIN) == 1 && count < 100){
		count++;
		delay_1us(1);
	}
	if(count >= 100){
		return 1;
	}
	
	return 0;
}


static uint8_t DHT11_ReadBit(){

    uint8_t count = 0;


	while(gpio_input_bit_get(PIN1_PORT , PIN1_PIN) == 0 && count < 100){
		count++;
		delay_1us(1);
	}

	count = 0;

	while(gpio_input_bit_get(PIN1_PORT , PIN1_PIN) == 1 && count < 100){
		count++;
		delay_1us(1);
	}

	if (count > 40)
	{
		return 1;
	}else{
		return 0;
	}
	
	//delay_1us(40);//等待40us
	//if(gpio_input_bit_get(PIN1_PORT , PIN1_PIN) == 1)return 1;
	//else return 0;

}


/**
 * 读取一个字节的数据
 * 
 * 本函数负责从DHT11传感器读取一个字节（8位）的数据
 * 它通过调用DHT11_ReadBit函数逐位读取数据，并将这些数据组合成一个字节
 * 
 * 返回值: 读取到的一个字节的数据
 */
static uint8_t DHT11_ReadByte(){

    // 初始化计数器i和数据变量dat
    uint8_t i = 0;
    uint8_t dat = 0;
    
    // 循环8次，读取8位数据
    for (i = 0; i < 8; i++)
    {
        // 读取一位数据，并将其左移到dat的相应位置
		
        dat <<= 1;
        dat |= DHT11_ReadBit();
    }

    // 返回读取到的字节
    return dat;

}
TH DHT11_GetTH(){

	TH th;

	int count = 0;

	uint8_t dat[5] = {0};

	DHT11_Start();
	if(DHT11_Check() == 0){
	    for (uint8_t i = 0; i < 5; i++)
		{
			dat[i] = DHT11_ReadByte();
		}
		
		if(dat[0] + dat[1] + dat[2] + dat[3]  != dat[4]){
			th.status = 0;
			return th;
		}else{
			th.status = 1;
		    th.humidity     = dat[0] + dat[1] * 0.1;

			th.temperature  = dat[2] + (dat[3] & 0x7f) * 0.1;

			if(dat[3] & 0x80){
			th.temperature = - (th.temperature);
			}
		}
		
	}

    return th;
}


void DHT11_Init(){

	DHT11_Start();
	DHT11_Check();

}

