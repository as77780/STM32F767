/*
 * NEC_Decode.h
 *
 *  Created on: Mar 9, 2016
 *      Author: peter
 */





#ifndef INC_NEC_DECODE_H_
#define INC_NEC_DECODE_H_

#include <stdint.h>
#include "main.h"
//#include "stm32f7xx_hal.h"
#define MAX_CAPT_COUNT 32
#define IR_NO_COMMAND   0

#define IR_NEC_START_PULSE      9000
#define IR_NEC_START_WIDTH      13500

#define IR_NEC_0_PULSE          562
#define IR_NEC_0_WIDTH          1125

#define IR_NEC_1_PULSE          562
#define IR_NEC_1_WIDTH          2250

#define IR_NEC_TOL                      5

/////////////////////////////////////////////////////////////////////////////
#define P_POWER ((address==0)&(cmd==69))
#define P_M_LEFT ((address==0)&(cmd==18))
#define P_M_WRITE ((address==0)&(cmd==64))
#define P_ITEM_WRITE ((address==0)&(cmd==30))
#define P_ITEM_LEFT ((address==0)&(cmd==10))
#define P_INPUT1 ((address==0)&(cmd==12))
#define P_INPUT2 ((address==0)&(cmd==24))
#define P_INPUT3 ((address==0)&(cmd==94))
#define P_INPUT4 ((address==0)&(cmd==8))
#define P_BASS ((address==0)&(cmd==17))
#define P_VOLUME ((address==0)&(cmd==20))
#define P_MIDDLE ((address==0)&(cmd==21))
#define P_TREBLE ((address==0)&(cmd==23))
#define P_GAIN_PL ((address==0)&(cmd==25))
#define P_GAIN_MIN ((address==0)&(cmd==22))
#define P_CHENGE_BUD ((address==0)&(cmd==68))
#define P_CHENGE_TIME ((address==0)&(cmd==15))
#define P_CHENGE_MUTE ((address==0)&(cmd==71))
#define P_CHENGE_TIME_B  ((address==0)&(cmd==0))
#define P_prog  ((address==0)&(cmd==66))
#define P_stop  ((address==0)&(cmd==70))
#define P_PL_LEFT  ((address==0)&(cmd==07))
#define P_PL_Right  ((address==0)&(cmd==21))
#define P_PL_V_PL  ((address==0)&(cmd==90))
#define P_PL_V_MIN ((address==0)&(cmd==28))
#define P_PL_MUZ ((address==0)&(cmd==82))
#define P_MUZ_BUD ((address==0)&(cmd==74))
////////////////////////////////////////////////////////////////////////////





// ���� ������ ������ ���������� �������� � �������� ��������� � ���������
#define checkVal(var,val,tol) (var>(val*(100-tol)/100) && var<(val*(100+tol)/100))

typedef enum {
	null,Power,P_forward,P_back,P_break,bud,hdd,radio,vol_add,vol_sub,dream,InputInt,input1,input2,input3
}pult_comand;


typedef enum {
    NEC_NOT_EXTENDED, NEC_EXTENDED
} NEC_TYPE;

typedef enum {
    NEC_INIT, NEC_AGC_OK, NEC_AGC_FAIL, NEC_FAIL, NEC_OK,REC_second,REC_Recording,NEC_one
} NEC_STATE;

typedef struct {
	int rawTimerData[32];
    uint8_t decoded[4];


    NEC_STATE state;

    TIM_HandleTypeDef *timerHandle;

    uint32_t timerChannel,timerChannel1;
    HAL_TIM_ActiveChannel timerChannelActive,timerChannelActive1;

    uint16_t timingBitBoundary;
    uint16_t timingAgcBoundary;
    NEC_TYPE type;

    pult_comand (*NEC_DecodedCallback)(uint16_t, uint8_t);
    void (*NEC_ErrorCallback)();
    void (*NEC_RepeatCallback)();
} NEC;


//void NEC_Init(TIM_HandleTypeDef* handle);
void NEC_Init(TIM_HandleTypeDef* handle);
void NEC_DeInit(NEC* handle);

pult_comand NEC_TIM_IC_CaptureCallback(NEC* handle);


pult_comand myNecDecodedCallback(uint16_t address, uint8_t cmd);
void myNecErrorCallback();
void myNecRepeatCallback();

void NEC_Read(NEC* handle);

#endif /* INC_NEC_DECODE_H_ */
