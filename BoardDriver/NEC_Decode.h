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

////////////////////////////////////////////////////////////////////////////





// ���� ������ ������ ���������� �������� � �������� ��������� � ���������
#define checkVal(var,val,tol) (var>(val*(100-tol)/100) && var<(val*(100+tol)/100))




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

    void (*NEC_DecodedCallback)(uint16_t, uint8_t);
    void (*NEC_ErrorCallback)();
    void (*NEC_RepeatCallback)();
} NEC;


//void NEC_Init(TIM_HandleTypeDef* handle);
void NEC_Init(TIM_HandleTypeDef* handle);
void NEC_DeInit(NEC* handle);

uint8_t NEC_TIM_IC_CaptureCallback(NEC* handle);


void myNecDecodedCallback(uint16_t address, uint8_t cmd);
void myNecErrorCallback();
void myNecRepeatCallback();

void NEC_Read(NEC* handle);

#endif /* INC_NEC_DECODE_H_ */
