#include <gui/screen2_screen/Screen2View.hpp>
#include "main.h"

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
	Screen2ViewBase::setupScreen();
	  TIM5->CCR1=20;
	  TimeView();
	  POWER_OFF();
}

void Screen2View::tearDownScreen()
{
   Screen2ViewBase::tearDownScreen();
}
void Screen2View::handleTickEvent()
{
    TimeView();
}
void Screen2View:: TimeView()
{
	hour=presenter->getHour();
	analogClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
	digitalClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
	if(hour>21||hour<9){
	//analogClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
	analogClock1.setVisible(false);
	digitalClock1.setVisible(true);
	digitalClock1.invalidate();
	analogClock1.invalidate();
	}
	else{
    // digitalClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
     digitalClock1.setVisible(false);
     analogClock1.setVisible(true);
     digitalClock1.invalidate();
     analogClock1.invalidate();
	}
}
