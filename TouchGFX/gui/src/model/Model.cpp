#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "../../../../BoardDriver/OW_LL/ow.h"
#include "../../../../BoardDriver/OW_LL/ow_device_ds18x20.h"
#include "../../../../BoardDriver/OW_LL/scan_devices.h"
#include <stdio.h>
#include "../../../../BoardDriver/EEPROM.h"
extern TIM_HandleTypeDef htim4;


uint8_t str,str1;

ow_t ow;
ow_rom_t rom_ids[20];
size_t rom_found;

Model::Model() : modelListener(0)
{
	 ow_init(&ow, NULL);
	 if (scan_onewire_devices(&ow, rom_ids, OW_ARRAYSIZE(rom_ids), &rom_found) == owOK) {
	            printf("Devices scanned, found %d devices!\r\n", (int)rom_found);
	        } else {
	            printf("Device scan error\r\n");
	        }
	        if (rom_found == 0) {

	        }
}

void Model::tick()
{
	if (tickCount == 60)
		    {
		getTime();
		    tickCount = 0;
		//    printf("Start temperature conversion\r\n");

		               ow_ds18x20_start(&ow, NULL);
		    if (rom_found) {
		                for (size_t i = 0; i < rom_found; i++) {
		                   if (ow_ds18x20_is_b(&ow, &rom_ids[i])) {
		                       float temp;
		                    uint8_t resolution = ow_ds18x20_get_resolution(&ow, &rom_ids[i]);
		                       if (ow_ds18x20_read(&ow, &rom_ids[i], &temp)) {
		                           printf("Sensor %02u temperature is %d.%d degrees (%u bits resolution)\r\n",
		                               (unsigned)i, (int)temp, (int)((temp * 1000.0f) - (((int)temp) * 1000)), (unsigned)resolution);
                                       temper[i]=temp;
		                           } else {
		                    //       printf("Could not read temperature on sensor %u\r\n", (unsigned)i);
		                       }
		                   }
		               }

		           }
		    temp_check((uint8_t)temper[t_power],(uint8_t)temper[T_sound]);

		    }


/*
		    str++;
		    EEPROM_writeData(str,&str,1);
		    EEPROM_readData(str,&str1,1);
		    printf("EEprom read: %d\r\n",str1 );
*/
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


void Model::temp_check(uint8_t t_pow,uint8_t t_amp){


	if((35<t_amp)&&(t_amp<50)){
		if(((t_amp - 30)*((uint8_t)(t_amp/10)))>15){
		TIM4->CCR1=(t_amp - 30)*((uint8_t)(t_amp/10));
		                }
			       }
	 if  (t_amp>=50) {
		TIM4->CCR1=100;
	}
	if(t_amp<=35) {
		TIM4->CCR1=10;
	}



	if((40<t_pow)&&(t_pow<50)){
			if(((t_pow - 30)*((uint8_t)(t_pow/10)))>20){
			TIM4->CCR2=(t_pow - 30)*((uint8_t)(t_pow/10));
			}
		}
		 if (t_pow>=50) {
			TIM4->CCR2=100;
		}
		 if(t_pow<=40) {
			TIM4->CCR2=20;
		}
		 FAN1Speed=(uint8_t)TIM4->CCR1;
		 FAN2Speed=(uint8_t)TIM4->CCR2;

}
