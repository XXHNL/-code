#ifndef	__DHT11_H
#define	__DHT11_H

#include "gd32f4xx.h"
#include "systick.h"


#define PIN1_RCU RCU_GPIOB
#define PIN1_PORT GPIOB
#define PIN1_PIN GPIO_PIN_15

typedef struct{

		double temperature;
		double humidity;
	  int status; 
	
}TH;	

void DHT11_Init();

TH DHT11_GetTH(); 

#endif