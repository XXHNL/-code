#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H

#include "gd32f4xx.h"
#include "systick.h"

#ifndef u32
#define u32 uint32_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u8
#define u8 uint8_t
#endif


// ��ʼ��������
void Buzzer_init();

// ����ָ��Ƶ�ʲ���
void Buzzer_play(u16 hz_val);

// ����ָ������������ 1,2,3,4��..7
void Buzzer_beep(u8 hz_val_index);

// ֹͣ����
void Buzzer_stop();

void Beep_node(u8 index);


#endif