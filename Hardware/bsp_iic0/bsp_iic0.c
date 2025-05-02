#include "bsp_iic0.h"
//�л������Ӳ��IIC

#if IIC0_TYPE //Ӳʵ������
	//��ʼ��
	static void bsp_hard_iic_init(void);

	//��IIC�豸д������
	static uint8_t bsp_hard_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

	//��IIC�豸д��һ������
	//dat: �е�һ��Ԫ�ص�ַ
	//col: һ���м���Ԫ��
	//row: ��Ҫ���Ͷ���������  
	static uint8_t bsp_hard_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

	//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
	static uint8_t bsp_hard_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);
#else         //��ʵ������
	//��ʼ��
	static void bsp_soft_iic_init(void);

	//��IIC�豸д������
	static uint8_t bsp_soft_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

	//��IIC�豸д��һ������
	//dat: �е�һ��Ԫ�ص�ַ
	//col: һ���м���Ԫ��
	//row: ��Ҫ���Ͷ���������  
	static uint8_t bsp_soft_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

	//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
	static uint8_t bsp_soft_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);
#endif


//��ʼ��
void bsp_iic0_init(void){
//���IIC��Ӳ��IIC��ʼ��
#if IIC0_TYPE
	//Ӳ��
	bsp_hard_iic_init();
#else
	//���
	bsp_soft_iic_init();
#endif
}

//��IIC�豸д������
uint8_t bsp_iic0_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
#if IIC0_TYPE
	//Ӳ��
	return bsp_hard_iic_write(addr, reg, dat, n);
#else
	//���
	return bsp_soft_iic_write( addr, reg, dat, n);
#endif
}

//��IIC�豸д��һ������
//dat: �е�һ��Ԫ�ص�ַ
//col: һ���м���Ԫ��
//row: ��Ҫ���Ͷ���������  
uint8_t bsp_iic0_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
#if IIC0_TYPE
	//Ӳ��
	return bsp_hard_iic_write_col( addr, reg, dat, col, row);
#else
	//���
	return bsp_soft_iic_write_col( addr, reg, dat, col, row);
#endif
}

//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
uint8_t bsp_iic0_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
#if IIC0_TYPE
	//Ӳ��
	return bsp_hard_iic_read( addr, reg, buff, n);
#else
	//���
	return bsp_soft_iic_read( addr, reg, buff, n);
#endif
}




/**************************** IIC0��ʵ�� ****************************/
#if IIC0_TYPE //Ӳʵ��

//IIC����GPIO��ʼ��
static void iic_gpio_config(void){
	/****************** GPIO config **********************/
	// ʱ������
	rcu_periph_clock_enable(IIC0_SCL_RCU);
	// ���ø��ù���
	gpio_af_set(IIC0_SCL_PORT, IIC0_SCL_AF, IIC0_SCL_PIN);
	// �������ģʽ
	gpio_mode_set(IIC0_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IIC0_SCL_PIN);
	gpio_output_options_set(IIC0_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC0_SCL_PIN);

	// ʱ������
	rcu_periph_clock_enable(IIC0_SDA_RCU);
	// ���ø��ù���
	gpio_af_set(IIC0_SDA_PORT, IIC0_SDA_AF, IIC0_SDA_PIN);
	// �������ģʽ
	gpio_mode_set(IIC0_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IIC0_SDA_PIN);
	gpio_output_options_set(IIC0_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC0_SDA_PIN);
	
}

//IIC��ʼ��
static void iic_per_config(void){
	/****************** I2C config  **********************/
	i2c_deinit(IIC0_NUM);
	// ʱ������
	rcu_periph_clock_enable(IIC0_RCU);
	// I2C��������
	i2c_clock_config(IIC0_NUM, IIC0_SPEED, I2C_DTCY_2);//SCL�͵�ƽ/�ߵ�ƽ=2
	
	
	// ʹ��i2c
	//i2c_mode_addr_config(IIC0_NUM, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
	i2c_enable(IIC0_NUM);
	//����1byte֮���Զ���Ӧ
	i2c_ack_config(IIC0_NUM, I2C_ACK_ENABLE);
}

//PB6 SCL
//PB7 SDA
//���óɿ�©���,����Ҫ����
//��ʼ��
static void bsp_hard_iic_init(void){
	//GPIO��ʼ��
	iic_gpio_config();
	//IIC��ʼ��
	iic_per_config();
}

//IIC��־λ
#define	TIMEOUT	50000
static uint8_t I2C_wait(uint32_t flag) {//����1 û�б�־λ  0:�б�־λ
    uint16_t cnt = 0;

    while(!i2c_flag_get(IIC0_NUM, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t flag) {//����1 �б�־λ 0:û�б�־λ
    uint16_t cnt = 0;

    while(i2c_flag_get(IIC0_NUM, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
		return 0;
}

//��IIC�豸д������(�豸��ַ:7bit)  0:�ɹ� ����:ʧ��
static uint8_t bsp_hard_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 д:0 ��:1
	//			1101 1000

	/************* start ***********************/
	// �ȴ�I2C����
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// start
	i2c_start_on_bus(IIC0_NUM);
	// �ȴ�I2C���豸�ɹ�������ʼ�ź�
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;

	/************* device address **************/
	// �����豸��ַ
	i2c_master_addressing(IIC0_NUM, w_addr, I2C_TRANSMITTER);
	// �ȴ���ַ�������
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************ register address ************/
	// �Ĵ�����ַ
	// �ȴ��������ݻ�����Ϊ��
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// ��������
	i2c_data_transmit(IIC0_NUM, reg);

	// �ȴ����ݷ������
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/***************** data ******************/
	// ��������
	uint32_t i;
	for(i = 0; i < n; i++) {
			uint32_t d = dat[i];

			// �ȴ��������ݻ�����Ϊ��
			if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_DAT_ERROR;

			// ��������
			i2c_data_transmit(IIC0_NUM, d);

			// �ȴ����ݷ������
			if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_DAT_ERROR;
	}
	
	/***************** stop ********************/
	// stop
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	return IIC_SUCCESS;
}

//��IIC�豸д��һ������
//dat: �е�һ��Ԫ�ص�ַ
//col: һ���м���Ԫ��
//row: ��Ҫ���Ͷ���������  
static uint8_t bsp_hard_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 д:0 ��:1
	//			1101 1000

	/************* start ***********************/
	// �ȴ�I2C����
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// start
	i2c_start_on_bus(IIC0_NUM);
	// �ȴ�I2C���豸�ɹ�������ʼ�ź�
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;

	/************* device address **************/
	// �����豸��ַ
	i2c_master_addressing(IIC0_NUM, w_addr, I2C_TRANSMITTER);
	// �ȴ���ַ�������
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************ register address ************/
	// �Ĵ�����ַ
	// �ȴ��������ݻ�����Ϊ��
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// ��������
	i2c_data_transmit(IIC0_NUM, reg);

	// �ȴ����ݷ������
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/***************** data ******************/
	// ��������
	uint32_t i;
	for(i = 0; i < row; i++) {
			uint32_t d = dat[i*col];

			// �ȴ��������ݻ�����Ϊ��
			if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_DAT_ERROR;

			// ��������
			i2c_data_transmit(IIC0_NUM, d);

			// �ȴ����ݷ������
			if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_DAT_ERROR;
	}
	
	/***************** stop ********************/
	// stop
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	return IIC_SUCCESS;
}

//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
//����0:�ɹ� ��������ֵ:ʧ��
static uint8_t bsp_hard_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
	uint8_t new_addr = (addr<<1);
	/************* start ***********************/
	// �ȴ�I2C����
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return IIC_BUS_BSY_ERROR;
	// ���������ź�
	i2c_start_on_bus(IIC0_NUM);
	// �ȴ�I2C���豸�ɹ�������ʼ�ź�
	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;
	/************* device address **************/
	// ���ʹ��豸��ַ
	i2c_master_addressing(IIC0_NUM, new_addr, I2C_TRANSMITTER);

	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	/********** register address **************/
	// �ȴ����ͻ�����	
	if(I2C_wait(I2C_FLAG_TBE)) return IIC_SEND_REG_ERROR;

	// ���ͼĴ�����ַ
	i2c_data_transmit(IIC0_NUM, reg);

	// �ȴ������������	
	if(I2C_wait(I2C_FLAG_BTC)) return IIC_SEND_REG_ERROR;
	/************* start ***********************/
	// �����������ź�
	i2c_start_on_bus(IIC0_NUM);

	if(I2C_wait(I2C_FLAG_SBSEND)) return IIC_START_ERROR;
	/************* device address **************/
	// ���ʹ��豸��ַ
	i2c_master_addressing(IIC0_NUM, new_addr, I2C_RECEIVER);
	if(I2C_wait(I2C_FLAG_ADDSEND)) return IIC_SEND_ADDR_ERROR;
	i2c_flag_clear(IIC0_NUM, I2C_FLAG_ADDSEND);
	
	/************* data **************/
	//ack
	i2c_ack_config(IIC0_NUM, I2C_ACK_ENABLE);
	// ����һ�����ݺ��Զ�����ACK
	i2c_ackpos_config(IIC0_NUM, I2C_ACKPOS_CURRENT);
	// ȷ��ACK������
	if(I2C_wait(I2C_CTL0(IIC0_NUM) & I2C_CTL0_ACKEN)) return IIC_AUTO_ACK_ERROR;

	// ��ȡ����
	uint32_t i;
	for (i = 0; i < n; i++) {
			if (i == n - 1) {
					// �ڶ�ȡ���һ���ֽ�֮ǰ������ACK������Ϊ�Զ�NACK
					i2c_ack_config(IIC0_NUM, I2C_ACK_DISABLE);
			}

			// �ȴ����ջ�������Ϊ��
			if(I2C_wait(I2C_FLAG_RBNE)) return IIC_RECV_BYTE_ERROR;
			
			buff[i] = i2c_data_receive(IIC0_NUM);//�Զ�����ack,���1byte ��ֹ�Զ�����ack
	}
	
	/***************** stop ********************/
	i2c_stop_on_bus(IIC0_NUM);

	if(I2C_waitn(I2C_CTL0(IIC0_NUM)&I2C_CTL0_STOP)) return IIC_STOP_ERROR;
	
	return IIC_SUCCESS;
}

#else         //��ʵ��

//PB6 SCL
//PB7 SDA
//���óɿ�©���,����Ҫ����
//��ʼ��
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
//������ʼ�ź�
static void iic_start(void){
	//ȷ��SDAģʽ
	SDA_OUT;
	//SDA�ߵ�ƽ,�������
	SDA_H;
	DELAY;
	//SCL�ߵ�ƽ,�������
	SCL_H;
	DELAY;
	//SDA�͵�ƽ,�������
	SDA_L;
	DELAY;
	//SCL�͵�ƽ,�������
	SCL_L;
	DELAY;
}
//����1byte
static void iic_send_byte(uint8_t dat){
	//0xD5   1101 0101
	//>>7    1 &0x01 =  1
	//>>6    11&0x01 =  1
	//>>5    110&0x01 = 0
	//SDA���
	SDA_OUT;
	for(int i=7;i>=0;i--){
		//����1bit
		//SDA��Ҫ���ݵ�ǰλ���øߵ͵�ƽ
		if((dat>>i)&0x01){
			//SDA����
			SDA_H;
		}else{
			//SDA����
			SDA_L;
		}
		//��ʱ����SDA�ܹ�����
		DELAY;
		//SCL����,�������
		SCL_H;
		DELAY;
		//SCL����,�������
		SCL_L;
		DELAY;
	}
}
//�ȴ���Ӧ 0:�ɹ�   1:ʧ��
static uint8_t iic_wait_ack(void){
	//SDA���
	SDA_OUT;
	//SDA״̬��ȷ��
	//SCL�͵�ƽ
	//SDA����,�������
	SDA_H;
	DELAY;
	//SCL����,����ʹ��
	SCL_H;
	DELAY;
	//SDA��������Ȩ,DELAY
	SDA_IN;
	DELAY;
	//��ȡSDA״̬
	if(SDA_STATUS==SET){
		//Ӧ��ʧ��
		return 1;
	}
	//Ӧ��ɹ�  ����SCL SDA������ģʽ
	SCL_L;
	SDA_OUT;
	DELAY;
	return 0;
}
//ֹͣ�ź�
static void iic_stop(void){
	//SDAģʽ
	SDA_OUT;
	//SDA����,SCL����
	SDA_L;
	SCL_L;
	DELAY;
	//SCL����,�������
	SCL_H;
	DELAY;
	//SDA����,�������
	SDA_H;
	DELAY;
}

//��ȡ1byte
static uint8_t iic_read_byte(void){
	//0x55 0101 0101
	uint8_t dat = 0;
	//0000 0000
	//0000 0000 | 0<<7  0000 0000
	//0000 0000 | 1<<6  0100 0000
	//0100 0000 | 0<<5  0100 0000
	//0100 0000 | 1<<4  0001 0000  0101 0000
	//0001 0000
	//SDA���ó�����ģʽ
	SDA_IN;
	//ѭ����ȡ8bit
//	for(int i=0;i<8;i++){
	for(int i=7;i>=0;i--){
		//SCL����,�������
		SCL_L;
		DELAY;
		//SCL����,��һ��ʱ��,��ȡ����
		SCL_H;
		DELAY;
		dat |= (SDA_STATUS<<i);
	}
	//SCL�ָ��͵�ƽ
	SCL_L;
	return dat;
}

//������Ӧ
static void iic_send_ack(void){
	//SDA���
	SDA_OUT;
	//SDA����,�������
	SDA_L;
	DELAY;
	//SCL����,�������
	SCL_H;
	DELAY;
	//SCL����,�������
	SCL_L;
	DELAY;
}

//���Ϳ���Ӧ
static void iic_send_noack(void){
	//SDA���
	SDA_OUT;
	//SDA����,�������
	SDA_H;
	DELAY;
	//SCL����,�������
	SCL_H;
	DELAY;
	//SCL����,�������
	SCL_L;
	DELAY;
}

//��IIC�豸д������(�豸��ַ:7bit)
static uint8_t bsp_soft_iic_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n){
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 д:0 ��:1
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//1.��ʼ�ź�
	iic_start();
	//2.�豸��ַ+д
	iic_send_byte(w_addr);
	//3.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.�Ĵ�����ַ
	iic_send_byte(reg);
	//5.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.��������(ѭ��)
	//7.�ȴ���Ӧ
	for(int i=0;i<n;i++){
		//����1byte����
//		iic_send_byte(dat[i]);
		iic_send_byte(*(dat+i));
		//�ȴ���Ӧ
		if(iic_wait_ack()!=0){
			//ֹͣͨ��
			iic_stop();
			return IIC_SEND_DAT_ERROR;
		}
	}
	
	//8.����ͨ��
	iic_stop();
	//�ɹ�
	return IIC_SUCCESS;
}

//��IIC�豸д��һ������
//dat: �е�һ��Ԫ�ص�ַ
//col: һ���м���Ԫ��
//row: ��Ҫ���Ͷ���������  
static uint8_t bsp_soft_iic_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row){
	//0x51  0101 0001   1010 0010
	//addr  0110 1100 д:0 ��:1
	//			1101 1000
	uint8_t w_addr = (addr<<1)+0;
	//1.��ʼ�ź�
	iic_start();
	//2.�豸��ַ+д
	iic_send_byte(w_addr);
	//3.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.�Ĵ�����ַ
	iic_send_byte(reg);
	//5.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.��������(ѭ��)
	//7.�ȴ���Ӧ
	for(int i=0;i<row;i++){
		//����1byte����
//		iic_send_byte(dat[i]);
		iic_send_byte(*(dat+i*col));
		//�ȴ���Ӧ
		if(iic_wait_ack()!=0){
			//ֹͣͨ��
			iic_stop();
			return IIC_SEND_DAT_ERROR;
		}
	}
	
	//8.����ͨ��
	iic_stop();
	//�ɹ�
	return IIC_SUCCESS;
}


//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
static uint8_t bsp_soft_iic_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n){
	//��д��ַ  1010 0011
	uint8_t w_addr = (addr<<1)+0;
	uint8_t r_addr = (addr<<1)+1;
	//1.������ʼ�ź�
	iic_start();
	//2.���豸��ַ(7bit+д)
	iic_send_byte(w_addr);
	//3.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//4.���Ĵ�����ַ
	iic_send_byte(reg);
	//5.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_REG_ERROR;
	}
	//6.����ʼ�ź�
	iic_start();
	//8.���豸��ַ(7bit+��)
	iic_send_byte(r_addr);
	//9.�ȴ���Ӧ
	if(iic_wait_ack()!=0){
		//ֹͣͨ��
		iic_stop();
		return IIC_SEND_ADDR_ERROR;
	}
	//10.��1byte
	//11.����Ӧ
	//12.�����1byte
	//13.������Ӧ
	for(int i=0;i<n;i++){//3byte
		//��ȡ���ݷ���buff��
		buff[i] = iic_read_byte();
		if(i==n-1){//�������һ��byte,���Ϳ���Ӧ
			iic_send_noack();
		}else{
			//������Ӧ
			iic_send_ack();
		}
	}
	//14.ֹͣ�ź�
	iic_stop();
	return IIC_SUCCESS;
}

#endif
