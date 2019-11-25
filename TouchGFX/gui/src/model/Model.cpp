#include <gui/model/Model.hpp>

#include <gui/model/ModelListener.hpp>

#include <stdio.h>
#include "../../../../BoardDriver/EEPROM.h"
#include "MyTask.h"
#include <gui/screeninputmenu_screen/ScreenInputMenuView.hpp>

#include "cmsis_os.h"
extern TIM_HandleTypeDef htim4;
STAT_t Eth_INET,Eth_REDy[task_N];

typedef  struct struct_temp_t {
	float temper[2];
}struct_temp;
extern osMailQId mail;
extern NEC nec;
ModelListener m;
Model model;


void myNecDecodedCallback(uint16_t address, uint8_t cmd) {
	m.bind(&model);
	if(P_POWER){
	//POWER_CAP=1;
		}
	if(P_INPUT1){
		SetInput(0);
model.SetVol(model.GetVolume());
			}
	if(P_INPUT2){
		SetInput(1);
model.SetVol(model.GetVolume());
			// bat_view();
		}
	if(P_INPUT3){
		SetInput(2);

		}
	if(P_INPUT4){
		SetInput(3);

		}
	if(P_stop){
	//	P_STOP();
	}
	if(P_MUZ_BUD){
		//		PL_BUD();
			}
	if(P_prog){
	//	PL_RAD();
			}
	if(P_PL_MUZ){
	//	PL_MUZ();
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
		//if((Eth_REDy[0]==complit)&((pl==on)|(rad==on)|(bud==on))){
			//R_Left(&huart7);
		R_Left_eth();
	//	}

			}
	if(P_PL_Right){
		//if((Eth_REDy[0]==complit)&((pl==on)|(rad==on)|(bud==on))){
		//	R_Right(&huart7);
			R_Right_eth();
	//	}

			}
	if(P_GAIN_PL){

				}
	if(P_GAIN_MIN){

				}
	if(P_CHENGE_BUD){

	}


	 NEC_Read(&nec);

}




Model::Model() : modelListener(0)
{
	Count=GetVolume();

}


void Model::tick()
{
	struct_temp *qstruct;
	osEvent evt;

	CheckIncoder();

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
	if (tickCount1 == 600)  {
		tickCount1=0;
		if(IsLogin(1)==1){
				    	Inet_test_eth();
				    }
	}
			tickCount++;
			tickCount1++;

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

uint8_t Model::GetStatLogin(){
	if(Eth_REDy[0]==complit){
		return 1;
	}
	else{
		return 0;
	}
}
uint8_t Model::GetStatInet(){
	if(Eth_INET==complit){
			return 1;
		}
		else{
			return 0;
		}
}

void Model::CheckIncoder(){
	//osEvent event;
	//event = osMessageGet(QueueIncoderHandle,0);
	//  if (event.status == osEventMessage){
	 if ( enc.capture_is_ready&(EncActiv)){
		 if(enc.EncDirect){
//		  if(event.value.v){
		  if(Count<80){
		  	 Count++;
		  	 SetVolume(Count);
		  }

	  }
	  else{
		  if(Count>0){
		  	 Count--;
		  	  SetVolume(Count);
		  	      }

	  }
		 enc.capture_is_ready=0;
	   }

}

uint8_t Player::PlayBUD(){
if((Eth_REDy[0]==complit)){
	if((HDD)||(Bud)||(Radio)){
						Stop();
					}
	prog_bud_eth();
	Bud=1;
	return 1;
	}
	return 0;
}
uint8_t Player::PlayRadio(){
	if(Eth_INET==complit){
		if((HDD)||(Bud)||(Radio)){
					Stop();
				}
	prog_eth();
	Radio=1;
	return 1;
		}
	return 0;
	}
uint8_t Player::PlayHDD(){
	if((Eth_REDy[0]==complit)){
		if((HDD)||(Bud)||(Radio)){
			Stop();
		}
	prog_pl_eth();
	HDD=1;
	return 1;
	}
	return 0;
	}
void Player::Stop(){
	if((HDD)||(Radio)||(Bud)){
		R_stop_eth();
		osDelay(500);
		Radio=0;
		HDD=0;
		Bud=0;
	}
}
uint8_t Player::EnyPlay(){
	if((HDD)||(Radio)||(Bud)){
	return 1;
	}

	return 0;

}
void Player::FF_Play(){
	if(EnyPlay()){R_Right_eth();}
	}
void Player::Back_Play(){
	if(EnyPlay()){R_Left_eth();}
}
uint8_t Player::GetState(){
	if(HDD){return 1;}
	if(Radio){return 2;}
	if(Bud){return 3;}
	return 0;
}

