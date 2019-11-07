#include <gui/screeninputmenu_screen/ScreenInputMenuView.hpp>

ScreenInputMenuView::ScreenInputMenuView()
{

}

void ScreenInputMenuView::setupScreen()
{
    ScreenInputMenuViewBase::setupScreen();
uint8_t in=GetInput();
if(in==3){
	InternalBut.setSelected(true);
	ButInput_1.setSelected(false);
	ButInput_2.setSelected(false);
	ButInput_3.setSelected(false);
}
else if(in==2){
	InternalBut.setSelected(false);
		ButInput_1.setSelected(true);
		ButInput_2.setSelected(false);
		ButInput_3.setSelected(false);
}
else if(in==1){
	InternalBut.setSelected(false);
		ButInput_1.setSelected(false);
		ButInput_2.setSelected(true);
		ButInput_3.setSelected(false);
}
else if(in==0){
	InternalBut.setSelected(false);
		ButInput_1.setSelected(false);
		ButInput_2.setSelected(false);
		ButInput_3.setSelected(true);
}
}

void ScreenInputMenuView::tearDownScreen()
{
    ScreenInputMenuViewBase::tearDownScreen();
}

void ScreenInputMenuView::FunInterButInt()
{
	SetInput(0);
}
void ScreenInputMenuView::FunInterBut1()
{
	SetInput(1);
}
void ScreenInputMenuView::FunInterBut2()
{
	SetInput(2);
}
void ScreenInputMenuView::FunInterBut3()
{
	SetInput(3);
}
