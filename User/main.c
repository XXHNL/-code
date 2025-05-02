#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_usart0.h"
#include "bsp_iic0.h"

#include "App.h"


//任务句柄,通过任务句柄可以操作任务
TaskHandle_t start_task_handler;
TaskHandle_t balance_task_handler;
TaskHandle_t input_task_handler;
TaskHandle_t show_task_handler;
TaskHandle_t send_cur_task_handler;
TaskHandle_t send_pid_task_handler;
TaskHandle_t parse_task_handler;
TaskHandle_t mqtt_task_handler;
//任务创建返回值
BaseType_t code;

// 全局变量定义

int setLightValue = 0;     // 设置的光照值
int lightValue = 0;        // 当前光照强度

static void sys_init(void){
	//timer8初始化
	bsp_timer8_config();

	//初始化IIC0
	bsp_iic0_init();
	DHT11_Init();
  OLED_Init();
  usart_init();
  BH1750_Init();
	led_init();
	//初始化App_Input
	App_Input_init();
	//初始化App_Balance
	App_Control_init();
	//初始化App_Show
	App_Show_init();
	ESP8266_Init();
	ADC_config();
	Buzzer_init();
}

//保证可以调用delay
void start_task(void * param){//等待eg的bit0 和 bit2
	//初始化任务
	sys_init();
	
	//平衡小球任务
	code = xTaskCreate(
					App_Control_task,//任务函数
					"App_Control_task",//任务名称
					128, //任务栈内存大小(最大65535) 最终申请内存  128*4byte
					NULL, //任务参数
					2,    //任务优先级,优先级越大,优先级越高  不能大于configMAX_PRIORITIES
					&balance_task_handler);
	//按键任务
	code = xTaskCreate(
					App_Input_task,//任务函数
					"App_Input_task",//任务名称
					128, //任务栈内存大小(最大65535) 最终申请内存  128*4byte
					NULL, //任务参数
					2,    //任务优先级,优先级越大,优先级越高  不能大于configMAX_PRIORITIES
					&input_task_handler);
	
	code = xTaskCreate(
						App_recev_task,//任务函数
						"App_recev_task",//任务名称
						128, //任务栈内存大小(最大65535) 最终申请内存  128*4byte
						NULL, //任务参数
						2,    //任务优先级,优先级越大,优先级越高  不能大于configMAX_PRIORITIES
						&mqtt_task_handler);
	//显示任务
	code = xTaskCreate(
					App_Show_task,//任务函数
					"App_Show_task",//任务名称
					128, //任务栈内存大小(最大65535) 最终申请内存  128*4byte
					NULL, //任务参数
					2,    //任务优先级,优先级越大,优先级越高  不能大于configMAX_PRIORITIES
					&show_task_handler);
	//任务结束,删除任务
	vTaskDelete(NULL); //删除本任务
}


//USART1
int main(void)
{
	//优先级分组
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	//动态创建任务(不需要手动申请和释放内存)
	code = xTaskCreate(
					start_task,//任务函数
					"start_task",//任务名称
					128, //任务栈内存大小(最大65535) 最终申请内存  128*4byte
					NULL, //任务参数
					2,    //任务优先级,优先级越大,优先级越高  不能大于configMAX_PRIORITIES
					&start_task_handler
	);

	//任务创建只有一种情况会失败,内存不足
	if(code!=pdPASS){
		printf("create task1 error\n");
	}
	//开始任务调度  开启系统滴答时钟,根据时间任务调度
	vTaskStartScheduler();
	
	while(1) {}
}
