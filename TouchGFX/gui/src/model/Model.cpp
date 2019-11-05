#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "../../../../BoardDriver/OW_LL/ow.h"
#include "../../../../BoardDriver/OW_LL/ow_device_ds18x20.h"
#include "../../../../BoardDriver/OW_LL/scan_devices.h"
#include <stdio.h>
#include "../../../../BoardDriver/EEPROM.h"
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
	 float avg_temp;
	    size_t avg_temp_count;

	if (tickCount == 30)
		    {
		getTime();
		    tickCount = 0;
		    printf("Start temperature conversion\r\n");
		               ow_ds18x20_start(&ow, NULL);
		    if (rom_found) {
		        avg_temp = 0;
		               avg_temp_count = 0;
		               for (size_t i = 0; i < rom_found; i++) {
		                   if (ow_ds18x20_is_b(&ow, &rom_ids[i])) {
		                       float temp;
		                       uint8_t resolution = ow_ds18x20_get_resolution(&ow, &rom_ids[i]);
		                       if (ow_ds18x20_read(&ow, &rom_ids[i], &temp)) {
		                           printf("Sensor %02u temperature is %d.%d degrees (%u bits resolution)\r\n",
		                               (unsigned)i, (int)temp, (int)((temp * 1000.0f) - (((int)temp) * 1000)), (unsigned)resolution);

		                           avg_temp += temp;
		                           avg_temp_count++;
		                       } else {
		                           printf("Could not read temperature on sensor %u\r\n", (unsigned)i);
		                       }
		                   }
		               }
		               if (avg_temp_count > 0) {
		                   avg_temp = avg_temp / avg_temp_count;
		               }
		               printf("Average temperature: %d.%d degrees\r\n", (int)avg_temp, (int)((avg_temp * 100.0f) - ((int)avg_temp) * 100));
		           }

		    }


		/*
		    str++;
		    EEPROM_writeData(0,&str,1);
		    EEPROM_readData(0,&str1,1);
		    debug(str1);
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



