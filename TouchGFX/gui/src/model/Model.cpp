#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{
}

void Model::tick()
{


	if (tickCount == 30)
		    {
		getTime();
		    tickCount = 0;
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
