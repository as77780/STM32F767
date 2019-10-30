#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

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

	 sTime.Hours = hour+1;
	 sTime.Minutes = minute;
	 sTime.Seconds = 0;
	 HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) ;

}

