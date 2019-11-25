/*
 * NEC_Decode.c
 *
 *  Created on: Mar 9, 2016
 *      Author: peter
 */



#include"NEC_Decode.h"

void TIM3_Init(TIM_HandleTypeDef* handle);


NEC nec;

void TIM3_Init(TIM_HandleTypeDef* handle)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  handle->Instance = TIM3;
  handle->Init.Prescaler = 107;
  handle->Init.CounterMode = TIM_COUNTERMODE_UP;
  handle->Init.Period = 0xFFFF;
  handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(handle) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(handle, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_IC_Init(handle) != HAL_OK)
  {
    Error_Handler();
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sSlaveConfig.TriggerFilter = 4;
  if (HAL_TIM_SlaveConfigSynchronization(handle, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(handle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 4;
  if (HAL_TIM_IC_ConfigChannel(handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
          }
  /*
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim_base->Instance==TIM3)
    {

      __HAL_RCC_TIM3_CLK_ENABLE();

      __HAL_RCC_GPIOC_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_6;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


      HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(TIM3_IRQn);
*/

}

void NEC_Init(TIM_HandleTypeDef* handle){
	TIM3_Init(handle);
	nec.timerHandle =handle;
	   nec.state=NEC_one;
	   nec.timerChannel = TIM_CHANNEL_1;
	   nec.timerChannel1 = TIM_CHANNEL_2;
	   nec.timerChannelActive = HAL_TIM_ACTIVE_CHANNEL_1;
	   nec.timerChannelActive1 = HAL_TIM_ACTIVE_CHANNEL_2;

	 //  nec.timingBitBoundary = 1600;
	 //  nec.timingAgcBoundary = 11500;
	   nec.timingBitBoundary = 1680;
	   nec.timingAgcBoundary = 12500;
	   nec.type = NEC_NOT_EXTENDED;

	   nec.NEC_DecodedCallback = myNecDecodedCallback;
	   nec.NEC_ErrorCallback = myNecErrorCallback;
	   nec.NEC_RepeatCallback = myNecRepeatCallback;

	   NEC_Read(&nec);

}

uint8_t NEC_TIM_IC_CaptureCallback(NEC* handle) {
	static	uint8_t mach, c=0;
   uint16_t cap;
   static uint8_t copy[2];
   static uint8_t captCount;
   uint8_t res_IR;
  // uint8_t mach;
	 if (handle->timerHandle->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	    {

		 cap = HAL_TIM_ReadCapturedValue(handle->timerHandle, handle->timerChannel);

		 if ((checkVal(cap, 11250, 10))&c) {
			 handle->state = REC_second;
			 mach=1;
		 return 0;
				 }
		 if( (res_IR>0)& (mach==1)&&(checkVal(cap, 31500, IR_NEC_TOL))){
			 if(copy[1]==22|copy[1]==25){
						 handle->NEC_DecodedCallback(copy[0], copy[1]);
			 }
			            if(res_IR<50){         //reset repeat put in timer:   if(res_IR!=0){res_IR--;}
			 	 				 res_IR=50;}
			             mach=0;
						 return 0;
				 }

		 if (checkVal(cap, IR_NEC_START_WIDTH, IR_NEC_TOL)) {
		 handle->state = REC_second;
		 return 0;
		 }
		 if( (handle->state == REC_second)&&(checkVal(cap, IR_NEC_0_WIDTH, IR_NEC_TOL))){

			 handle->state = REC_Recording;
			 mach=0;

		 }
		 if (handle->state == REC_Recording & captCount<MAX_CAPT_COUNT) {
		 cap = HAL_TIM_ReadCapturedValue(handle->timerHandle, handle->timerChannel);
		 handle->rawTimerData[captCount]=cap;
		 captCount++;
	//	 if (!checkVal(cap, IR_NEC_START_WIDTH, IR_NEC_TOL)) {
			// if (cap < handle->timingAgcBoundary) {
			//  handle->NEC_RepeatCallback();
		         }

		 if(captCount==MAX_CAPT_COUNT){
			 captCount=0;
			 handle->state = NEC_AGC_OK;
		 }
		 if (handle->state == NEC_AGC_OK) {

		    //     HAL_TIM_IC_Stop_DMA(handle->timerHandle, handle->timerChannel);
		         int pos;
		         for ( pos = 0; pos < 32; pos++) {
		             int time = handle->rawTimerData[pos];
		             if (time > handle->timingBitBoundary) {
		                 handle->decoded[pos / 8] |= 1 << (pos % 8);
		             } else {
		                 handle->decoded[pos / 8] &= ~(1 << (pos % 8));
		             }
		         }

		         uint8_t valid = 1;

		         uint8_t naddr = ~handle->decoded[0];
		         uint8_t ncmd = ~handle->decoded[2];
		         uint32_t dubl=((uint32_t)( handle->decoded[0] << 24) | (uint32_t) ( handle->decoded[1]<<16)|(uint32_t) ( handle->decoded[2]<<8)|(uint32_t) ( handle->decoded[3])) ;

		         if (handle->type == NEC_NOT_EXTENDED && handle->decoded[1] != naddr)
		             valid = 0;
		         if (handle->decoded[3] != ncmd)
		             valid = 0;

		         handle->state = NEC_OK;
		         mach=0;
		         if (valid){

		        	copy[0]=handle->decoded[0];
		        	copy[1]=handle->decoded[2];

		        			             handle->NEC_DecodedCallback(handle->decoded[0], handle->decoded[2]);
                     c=valid;
                     res_IR=50;


		         }
		             else
		             handle->NEC_ErrorCallback();
		     }


	    }


}

void NEC_Read(NEC* handle) {

if(handle->state ==NEC_one){
      HAL_TIM_IC_Start_IT(handle->timerHandle, handle->timerChannel);
}

}


void myNecErrorCallback() {
      NEC_Read(&nec);
}

void myNecRepeatCallback() {
    NEC_Read(&nec);
}


/*
 * insert in main
 */
/*
void myNecDecodedCallback(uint16_t address, uint8_t cmd) {
	if(P_POWER){
	POWER_CAP=1;
		}
	if(P_INPUT1){
		tda7439SetInput(0);
		 bat_view();
	}
	if(P_INPUT2){
		tda7439SetInput(1);
			 bat_view();
		}
	if(P_INPUT3){
			tda7439SetInput(2);
			 bat_view();
		}
	if(P_INPUT4){
			 tda7439SetInput(3);
			 bat_view();
		}
	if(P_stop){
		P_STOP();
	}
	if(P_MUZ_BUD){
				PL_BUD();
			}
	if(P_prog){
		PL_RAD();
			}
	if(P_PL_MUZ){
		PL_MUZ();
			}
	if(P_PL_V_PL){
		//R_V_PL(&huart7);
		R_V_PL_eth();
		}
	if(P_PL_V_MIN){
		//R_V_Min(&huart7);
	R_V_Min_eth();
	}
	if(P_PL_LEFT){
		if((Eth_REDy[0]==complit)&((pl==on)|(rad==on)|(bud==on))){
			//R_Left(&huart7);
		R_Left_eth();
		}

			}
	if(P_PL_Right){
		if((Eth_REDy[0]==complit)&((pl==on)|(rad==on)|(bud==on))){
		//	R_Right(&huart7);
			R_Right_eth();
		}

			}
	if(P_GAIN_PL){
		spin_test=SPINBOX_GetValue(hSpin);
					    		spin_test++;
					     				SPINBOX_SetValue(hSpin,spin_test);

				}
	if(P_GAIN_MIN){
		spin_test=SPINBOX_GetValue(hSpin);
                                             if(spin_test!=0)
                                            	 spin_test--;
				  	    		            	SPINBOX_SetValue(hSpin,spin_test);
				}
	if(P_CHENGE_BUD){
		if((sleep==off)&(power_stat==POW_ON)){
			sleep=on;
					}
		else {
			sleep=off;
				}
	}


	 NEC_Read(&nec);

}

*/
