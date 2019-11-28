#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}
void Screen1View::handleTickEvent(){
	if((presenter->CheckPowerFlag())==1){
		application().gotoScreen2ScreenCoverTransitionNorth();
   	}
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    TIM5->CCR1=100;
    hour = presenter->getHour();
    minute = presenter->getMinute();

    Unicode::snprintf(textAreaHourBuffer, TEXTAREAHOUR_SIZE, "%02d", hour);
    Unicode::snprintf(textAreaMinuteBuffer, TEXTAREAMINUTE_SIZE, "%02d", minute);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}


void Screen1View::buttonHourUpClicked()
{
    hour = (hour + 1) % 24; // Keep new value in range 0..23
    Unicode::snprintf(textAreaHourBuffer, TEXTAREAHOUR_SIZE, "%02d", hour);
    textAreaHour.invalidate();
}

void Screen1View::buttonHourDownClicked()
{
    hour = (hour + 24 - 1) % 24; // Keep new value in range 0..23
    Unicode::snprintf(textAreaHourBuffer, TEXTAREAHOUR_SIZE, "%02d", hour);
    textAreaHour.invalidate();
}

void Screen1View::buttonMinuteUpClicked()
{
    minute = (minute + 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(textAreaMinuteBuffer, TEXTAREAMINUTE_SIZE, "%02d", minute);
    textAreaMinute.invalidate();
}

void Screen1View::buttonMinuteDownClicked()
{
    minute = (minute + 60 - 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(textAreaMinuteBuffer, TEXTAREAMINUTE_SIZE, "%02d", minute);
    textAreaMinute.invalidate();
}
void Screen1View::buttonSaveClicked()
{
	 RTC_TimeTypeDef sTime ;

	 sTime.Hours = (uint8_t)hour;
	 sTime.Minutes = (uint8_t)minute;
	 sTime.Seconds = 0;
	 sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	 sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	 HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) ;
	 /*
	    sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
  sDate.Month = RTC_MONTH_NOVEMBER;
  sDate.Date = 0x7;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  */

}

