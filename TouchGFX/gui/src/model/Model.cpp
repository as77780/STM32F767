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
extern osMessageQId QueueIncoderHandle;
extern NEC nec;
ModelListener m;
Model model;







Model::Model() : modelListener(0)
{
	Count=GetVolume();

}


void Model::tick()
{
	struct_temp *qstruct;
	osEvent evt;

	CheckIncoder();
	CheckPult();

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
void Model::CheckPult(){
	osEvent event;
	pult_comand pult;
		event = osMessageGet(QueueIncoderHandle,0);
		 if (event.status == osEventMessage){
			 pult=(pult_comand)event.value.v;
			 if(pult==InputInt){
				 SetInput(0);
			 }
			 else if(pult==input1){
				 SetInput(1);
			 }
			 else if(pult==input2){
				 SetInput(2);
			 }
			 else if(pult==input3){
				 SetInput(3);
			 }
			 else if(pult==vol_add){
				 Count++;
				 tda7439VolumeSet(Count);
			 			 }
			 else if(pult==vol_sub){
			 				 Count--;
			 				 tda7439VolumeSet(Count);
			 			 			 }
			 else if(pult==P_break){
			 				play.Stop();
			 			 }
			 else if(pult==radio){
			 			play.PlayRadio();
			 	    }
			 else if(pult==hdd){
						play.PlayHDD();
				 	    }
			 else if(pult==bud){
						play.PlayBUD();
					   }
			 else if(pult==P_forward){
				play.FF_Play();
			   }
			 else if(pult==P_back){
							play.Back_Play();
						   }
			 else if(pult==Power){
				 PowerFlag=1;
			 		  }

		 }

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

void Model::SetInput(uint8_t in){
	tda7439SetInput(in);
	SetVol(GetVolume());

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

