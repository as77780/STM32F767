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
	 presenter->SetInput(0);
	// presenter->SetVol(presenter->GetVolume());

}
void ScreenInputMenuView::FunInterBut1()
{
	 presenter->SetInput(1);
	//presenter->SetVol(presenter->GetVolume());
}
void ScreenInputMenuView::FunInterBut2()
{
	 presenter->SetInput(2);
	//presenter->SetVol(presenter->GetVolume());
}
void ScreenInputMenuView::FunInterBut3()
{
	 presenter->SetInput(3);
	//presenter->SetVol(presenter->GetVolume());
}
