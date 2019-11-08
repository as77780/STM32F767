#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <stdio.h>
#include "../../../../BoardDriver/EEPROM.h"

#include "cmsis_os.h"
extern TIM_HandleTypeDef htim4;

typedef  struct struct_temp_t {
	float temper[2];
}struct_temp;
extern osMailQId mail;





Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	struct_temp *qstruct;
	osEvent evt;

	evt = osMailGet(mail, osFeature_Wait);
			   			 if (evt.status == osEventMail) {
			   				qstruct=(struct_temp*)evt.value.p;
			   				 temper[t_power]=qstruct->temper[t_power];
			   				 temper[T_sound]=qstruct->temper[T_sound];
			   			 }
	if (tickCount == 60)
		    {
		    tickCount = 0;

		    getTime();
		    FAN1Speed=(uint8_t)TIM4->CCR1;
		    FAN2Speed=(uint8_t)TIM4->CCR2;
		    	    }
			tickCount++;

}
void Model::getTime(){
	                 RTC_TimeTypeDef sTime = {0};
			 		 RTC_DateTypeDef sDate = {0};

			 	       	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			 	    	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			 	    	hour =sTime.Hours;
			 	    	minute =sTime.Minutes ;
			 	    	second=sTime.Seconds;
}



