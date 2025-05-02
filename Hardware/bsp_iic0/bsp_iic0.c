#include "bsp_iic0.h"
//切换软件和硬件IIC

#if IIC0_TYPE //硬实现申明
	//初始化
	static void bsp_hard_iic_init(void);

	//向IIC设备写入数据
	static uint8_t bsp_hard_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

	//向IIC设备写入一列数据
	//dat: 列第一个元素地址
	//col: 一列有几个元素
	//row: 需要发送多少行数据  
	static uint8_t bsp_hard_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

	//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
	static uint8_t bsp_hard_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);
#else         //软实现申明
	//初始化
	static void bsp_soft_iic_init(void);

	//向IIC设备写入数据
	static uint8_t bsp_soft_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

	//向IIC设备写入一列数据
	//dat: 列第一个元素地址
	//col: 一列有几个元素
	//row: 需要发送多少行数据  
	static uint8_t bsp_soft_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

	//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
	static uint8_t bsp_soft_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);
#endif


//初始化
void bsp_iic0_init(void){
//软件IIC或硬件IIC初始化
#if IIC0_TYPE
	//硬件
	bsp_hard_iic_init();
#else
	//软件
	bsp_soft_iic_init();
#endif
}

//向IIC设备写入数据
uint8_t bsp_iic0_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
#if IIC0_TYPE
	//硬件
	return bsp_hard_iic_write(addr, reg, dat, n);
#else
	//软件
	return bsp_soft_iic_write( addr, reg, dat, n);
#endif
}

//向IIC设备写入一列数据
//dat: 列第一个元素地址
//col: 一列有几个元素
//row: 需要发送多少行数据  
uint8_t bsp_iic0_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
#if IIC0_TYPE
	//硬件
	return bsp_hard_iic_write_col( addr, reg, dat, col, row);
#else
	//软件
	return bsp_soft_iic_write_col( addr, reg, dat, col, row);
#endif
}

//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
uint8_t bsp_iic0_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
#if IIC0_TYPE
	//硬件
	return bsp_hard_iic_read( addr, reg, buff, n);
#else
	//软件
	return bsp_soft_iic_read( addr, reg, buff, n);
#endif
}




/**************************** IIC0软实现 ****************************/
#if IIC0_TYPE //硬实现

//IIC引脚GPIO初始化
static void iic_gpio_config(void){
	/****************** GPIO config **********************/
	// 时钟配置
	rcu_periph_clock_enable(IIC0_SCL_RCU);
	// 设置复用功能
	gpio_af_set(IIC0_SCL_PORT, IIC0_SCL_AF, IIC0_SCL_PIN);
	// 设置输出模式
	gpio_mode_set(IIC0_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IIC0_SCL_PIN);
	gpio_output_options_set(IIC0_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC0_SCL_PIN);

	// 时钟配置
	rcu_periph_clock_enable(IIC0_SDA_RCU);
	// 设置复用功能
	gpio_af_set(IIC0_SDA_PORT, IIC0_SDA_AF, IIC0_SDA_PIN);
	// 设置输出模式
	gpio_mode_set(IIC0_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IIC0_SDA_PIN);
	gpio_output_options_set(IIC0_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC0_SDA_PIN);
	
}

//IIC初始化
static void iic_per_config(void){
	/****************** I2C config  **********************/
	i2c_deinit(IIC0_NUM);
	// 时钟配置
	rcu_periph_clock_enable(IIC0_RCU);
	// I2C速率配置
	i2c_clock_config(IIC0_NUM, IIC0_SPEED, I2C_DTCY_2);//SCL低电平/高电平=2
	
	
	// 使能i2c
	//i2c_mode_addr_config(IIC0_NUM, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
	i2c_enable(IIC0_NUM);
	//接收1byte之后自动响应
	i2c_ack_config(IIC0_NUM, I2C_ACK_ENABLE);
}

//PB6 SCL
//PB7 SDA
//配置成开漏输出,不需要上拉
//初始化
static void bsp_hard_iic_init(void){
	//GPIO初始化
	iic_gpio_config();
	//IIC初始化
	iic_per_config();
}

//IIC标志位
#define	TIMEOUT	50000
static uint8_t I2C_wait(uint32_t flag) {//返回1 没有标志位  0:有标志位
    uint16_t cnt = 0;

    while(!i2c_flag_get(IIC0_NUM, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t flag) {//返回1 有标志位 0:没有标志位
    uint16_t cnt = 0;

    while(i2c_flag_get(IIC0_NUM, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
		return 0;
}

//向IIC设备写入数据(设备地址:7bit)  0:成功 其它:失败
static uint8_t bsp_hard_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 写:0 读:1
	//			1101 1000

	/************* start ***********************/
	// 等待I2C闲置
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// start
	i2c_start_on_bus(IIC0_NUM);
	// 等待I2C主设备成功发送起始信号
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;

	/************* device address **************/
	// 发送设备地址
	i2c_master_addressing(IIC0_NUM, w_addr, I2C_TRANSMITTER);
	// 等待地址发送完成
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************ register address ************/
	// 寄存器地址
	// 等待发送数据缓冲区为空
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// 发送数据
	i2c_data_transmit(IIC0_NUM, reg);

	// 等待数据发送完成
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/***************** data ******************/
	// 发送数据
	uint32_t i;
	for(i = 0; i < n; i++) {
			uint32_t d = dat[i];

			// 等待发送数据缓冲区为空
			if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_DAT_ERROR;

			// 发送数据
			i2c_data_transmit(IIC0_NUM, d);

			// 等待数据发送完成
			if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_DAT_ERROR;
	}
	
	/***************** stop ********************/
	// stop
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	return IIC_SUCCESS;
}

//向IIC设备写入一列数据
//dat: 列第一个元素地址
//col: 一列有几个元素
//row: 需要发送多少行数据  
static uint8_t bsp_hard_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 写:0 读:1
	//			1101 1000

	/************* start ***********************/
	// 等待I2C闲置
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// start
	i2c_start_on_bus(IIC0_NUM);
	// 等待I2C主设备成功发送起始信号
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;

	/************* device address **************/
	// 发送设备地址
	i2c_master_addressing(IIC0_NUM, w_addr, I2C_TRANSMITTER);
	// 等待地址发送完成
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************ register address ************/
	// 寄存器地址
	// 等待发送数据缓冲区为空
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// 发送数据
	i2c_data_transmit(IIC0_NUM, reg);

	// 等待数据发送完成
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/***************** data ******************/
	// 发送数据
	uint32_t i;
	for(i = 0; i < row; i++) {
			uint32_t d = dat[i*col];

			// 等待发送数据缓冲区为空
			if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_DAT_ERROR;

			// 发送数据
			i2c_data_transmit(IIC0_NUM, d);

			// 等待数据发送完成
			if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_DAT_ERROR;
	}
	
	/***************** stop ********************/
	// stop
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	return IIC_SUCCESS;
}

//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
//返回0:成功 返回其它值:失败
static uint8_t bsp_hard_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
	uint8_t new_addr = (addr<<1);
	/************* start ***********************/
	// 等待I2C空闲
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// 发送启动信号
	i2c_start_on_bus(IIC0_NUM);
	// 等待I2C主设备成功发送起始信号
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;
	/************* device address **************/
	// 发送从设备地址
	i2c_master_addressing(IIC0_NUM, new_addr, I2C_TRANSMITTER);

	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	/********** register address **************/
	// 等待发送缓冲区	
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// 发送寄存器地址
	i2c_data_transmit(IIC0_NUM, reg);

	// 等待发送数据完成	
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/************* start ***********************/
	// 发送再启动信号
	i2c_start_on_bus(IIC0_NUM);

	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;
	/************* device address **************/
	// 发送从设备地址
	i2c_master_addressing(IIC0_NUM, new_addr, I2C_RECEIVER);
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************* data **************/
	//ack
	i2c_ack_config(IIC0_NUM, I2C_ACK_ENABLE);
	// 接收一个数据后，自动发送ACK
	i2c_ackpos_config(IIC0_NUM, I2C_ACKPOS_CURRENT);
	// 确认ACK已启用
	if(I2C_wait(I2C_CTL0(IIC0_NUM) & I2C_CTL0_ACKEN)) return IIC_AUTO_ACK_ERROR;

	// 读取数据
	uint32_t i;
	for (i = 0; i < n; i++) {
			if (i == n - 1) {
					// 在读取最后一个字节之前，禁用ACK，配置为自动NACK
					i2c_ack_config(IIC0_NUM, I2C_ACK_DISABLE);
			}

			// 等待接收缓冲区不为空
			if(I2C_wait(I2C_FLAG_RBNE)) return IIC_RECV_BYTE_ERROR;
			
			buff[i] = i2c_data_receive(IIC0_NUM);//自动发送ack,最后1byte 禁止自动发送ack
	}
	
	/***************** stop ********************/
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	
	return IIC_SUCCESS;
}

#else         //软实现

//PB6 SCL
//PB7 SDA
//配置成开漏输出,不需要上拉
//初始化
static void bsp_soft_iic_init(void){
	//PB6 SCL
	rcu_periph_clock_enable(IIC0_SCL_RCU);
	gpio_mode_set(IIC0_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IIC0_SCL_PIN);
	gpio_output_options_set(IIC0_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, IIC0_SCL_PIN);
	//PB7 SDA
	rcu_periph_clock_enable(IIC0_SDA_RCU);
	gpio_mode_set(IIC0_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IIC0_SDA_PIN);
	gpio_output_options_set(IIC0_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, IIC0_SDA_PIN);
}
//发送起始信号
static void iic_start(void){
	//确定SDA模式
	SDA_OUT;
	//SDA高电平,持续输出
	SDA_H;
	DELAY;
	//SCL高电平,持续输出
	SCL_H;
	DELAY;
	//SDA低电平,持续输出
	SDA_L;
	DELAY;
	//SCL低电平,持续输出
	SCL_L;
	DELAY;
}
//发送1byte
static void iic_send_byte(uint8_t dat){
	//0xD5   1101 0101
	//>>7    1 &0x01 =  1
	//>>6    11&0x01 =  1
	//>>5    110&0x01 = 0
	//SDA输出
	SDA_OUT;
	for(int i=7;i>=0;i--){
		//发送1bit
		//SDA需要根据当前位设置高低电平
		if((dat>>i)&0x01){
			//SDA拉高
			SDA_H;
		}else{
			//SDA拉低
			SDA_L;
		}
		//给时间让SDA能够拉高
		DELAY;
		//SCL拉高,持续输出
		SCL_H;
		DELAY;
		//SCL拉低,持续输出
		SCL_L;
		DELAY;
	}
}
//等待响应 0:成功   1:失败
static uint8_t iic_wait_ack(void){
	//SDA输出
	SDA_OUT;
	//SDA状态不确定
	//SCL低电平
	//SDA拉高,持续输出
	SDA_H;
	DELAY;
	//SCL拉高,持续使出
	SCL_H;
	DELAY;
	//SDA交出控制权,DELAY
	SDA_IN;
	DELAY;
	//读取SDA状态
	if(SDA_STATUS==SET){
		//应答失败
		return 1;
	}
	//应答成功  拉低SCL SDA变成输出模式
	SCL_L;
	SDA_OUT;
	DELAY;
	return 0;
}
//停止信号
static void iic_stop(void){
	//SDA模式
	SDA_OUT;
	//SDA拉低,SCL拉低
	SDA_L;
	SCL_L;
	DELAY;
	//SCL拉高,持续输出
	SCL_H;
	DELAY;
	//SDA拉高,持续输出
	SDA_H;
	DELAY;
}

//读取1byte
static uint8_t iic_read_byte(void){
	//0x55 0101 0101
	uint8_t dat = 0;
	//0000 0000
	//0000 0000 | 0<<7  0000 0000
	//0000 0000 | 1<<6  0100 0000
	//0100 0000 | 0<<5  0100 0000
	//0100 0000 | 1<<4  0001 0000  0101 0000
	//0001 0000
	//SDA设置成输入模式
	SDA_IN;
	//循环读取8bit
//	for(int i=0;i<8;i++){
	for(int i=7;i>=0;i--){
		//SCL拉低,持续输出
		SCL_L;
		DELAY;
		//SCL拉高,等一段时间,读取数据
		SCL_H;
		DELAY;
		dat |= (SDA_STATUS<<i);
	}
	//SCL恢复低电平
	SCL_L;
	return dat;
}

//发送响应
static void iic_send_ack(void){
	//SDA输出
	SDA_OUT;
	//SDA拉低,持续输出
	SDA_L;
	DELAY;
	//SCL拉高,持续输出
	SCL_H;
	DELAY;
	//SCL拉低,持续输出
	SCL_L;
	DELAY;
}

//发送空响应
static void iic_send_noack(void){
	//SDA输出
	SDA_OUT;
	//SDA拉低,持续输出
	SDA_H;
	DELAY;
	//SCL拉高,持续输出
	SCL_H;
	DELAY;
	//SCL拉低,持续输出
	SCL_L;
	DELAY;
}

//向IIC设备写入数据(设备地址:7bit)
static uint8_t bsp_soft_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 写:0 读:1
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//1.起始信号
	iic_start();
	//2.设备地址+写
	iic_send_byte(w_addr);
	//3.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.寄存器地址
	iic_send_byte(reg);
	//5.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.发送数据(循环)
	//7.等待响应
	for(int i=0;i<n;i++){
		//发送1byte数据
//		iic_send_byte(dat[i]);
		iic_send_byte(*(dat+i));
		//等待响应
		if(iic_wait_ack()!=0){
			//停止通信
			iic_stop();
			return IIC_SEND_DAT_ERROR;
		}
	}
	
	//8.结束通信
	iic_stop();
	//成功
	return IIC_SUCCESS;
}

//向IIC设备写入一列数据
//dat: 列第一个元素地址
//col: 一列有几个元素
//row: 需要发送多少行数据  
static uint8_t bsp_soft_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 写:0 读:1
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//1.起始信号
	iic_start();
	//2.设备地址+写
	iic_send_byte(w_addr);
	//3.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.寄存器地址
	iic_send_byte(reg);
	//5.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.发送数据(循环)
	//7.等待响应
	for(int i=0;i<row;i++){
		//发送1byte数据
//		iic_send_byte(dat[i]);
		iic_send_byte(*(dat+i*col));
		//等待响应
		if(iic_wait_ack()!=0){
			//停止通信
			iic_stop();
			return IIC_SEND_DAT_ERROR;
		}
	}
	
	//8.结束通信
	iic_stop();
	//成功
	return IIC_SUCCESS;
}


//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
static uint8_t bsp_soft_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
	//读写地址  1010 0011
	uint8_t w_addr = (addr<<1)+0;
	uint8_t r_addr = (addr<<1)+1;
	//1.发送起始信号
	iic_start();
	//2.发设备地址(7bit+写)
	iic_send_byte(w_addr);
	//3.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.发寄存器地址
	iic_send_byte(reg);
	//5.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.发起开始信号
	iic_start();
	//8.发设备地址(7bit+读)
	iic_send_byte(r_addr);
	//9.等待响应
	if(iic_wait_ack()!=0){
		//停止通信
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//10.读1byte
	//11.发响应
	//12.读最后1byte
	//13.发空响应
	for(int i=0;i<n;i++){//3byte
		//读取数据放入buff中
		buff[i] = iic_read_byte();
		if(i==n-1){//接收最后一个byte,发送空响应
			iic_send_noack();
		}else{
			//发送响应
			iic_send_ack();
		}
	}
	//14.停止信号
	iic_stop();
	return IIC_SUCCESS;
}

#endif
