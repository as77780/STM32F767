#include <gui/screen2_screen/Screen2View.hpp>
#include "main.h"
#include "cmsis_os.h"

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
	Screen2ViewBase::setupScreen();
	  LED_50();
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
    PowerOff();

}

void Screen2View::PowerOff(){
	if((presenter->CheckPowerFlag())==1){
	      //   presenter->ResetPowerFlag();
	         PowerOnPrepare();
	         LED_50();
	    		        osDelay(2000);
	    		        application().gotoMainScreenCoverTransitionEast();
	    	}
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
    image1.setAlpha(0);
    image1.invalidate();
	}
	else{
    // digitalClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
     digitalClock1.setVisible(false);
     analogClock1.setVisible(true);
     digitalClock1.invalidate();
     analogClock1.invalidate();
     image1.setAlpha(255);
     image1.invalidate();
	}
}
