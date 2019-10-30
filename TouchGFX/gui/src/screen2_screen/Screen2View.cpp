#include <gui/screen2_screen/Screen2View.hpp>


Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
	Screen2ViewBase::setupScreen();
	  TIM5->CCR1=100;
	  TimeView();
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
	analogClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());
}
