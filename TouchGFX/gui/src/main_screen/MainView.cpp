#include "BitmapDatabase.hpp"
#include <gui/main_screen/MainView.hpp>
#include "cmsis_os.h"
#include "main.h"


extern osMessageQId QueueIncoderHandle;



//uint8_t s;
MainView::MainView()//:scrollWheelAnimateToCallback(this, &MainView::scrollWheelAnimateToHandler)
{

}

void MainView::setupScreen()
{	//scrollWheelVolume.setAnimateToCallback(scrollWheelAnimateToCallback);

	CheckChannel();
    MainViewBase::setupScreen();
    TIM5->CCR1=100;
  	 GetTimeOut();
  	Count=presenter->GetVol();
  	 Unicode::snprintf(textVolumeBuffer,TEXTVOLUME_SIZE,"%02d", Count);
  	 textVolume.invalidate();
  	SOUND_ON();
  	enc.capture_is_ready=0;
}

void MainView::PowerOff(){
	if((presenter->CheckPowerFlag())==1){
	   //  presenter->ResetPowerFlag();
	          	 prepareOff();
			       // buttonPowOff.clearFadeAnimationEndedAction();
			        buttonPowOff.startFadeAnimation(0, 120, touchgfx::EasingEquations::linearEaseIn);
			        osDelay(2000);
			        application().gotoScreen2ScreenCoverTransitionWest();
		}
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}


void MainView::handleTickEvent()
{
	GetTimeOut();
	ViewTemp();
	CheckVOL();
	CheckIkonBat();
	PowerOff();
	CheckChannel();



}

void  MainView::CheckChannel(){
	in=GetInput();
	Unicode::snprintf(channelBuffer,CHANNEL_SIZE,"%d",4-in);
	channel.invalidate();
}

 void  MainView::GetTimeOut()
        {
	    digitalClock1.setTime24Hour(presenter->getHour(), presenter->getMinute(), presenter->getSecond());

	    if(presenter->GetStatLogin()==1){image1.setAlpha(255);image1.invalidate();}

	    if(presenter->GetStatInet()==1){
	    	image1.setBitmap(touchgfx::Bitmap(BITMAP_INTERNET_ID));image1.invalidate();
	     }
	     else if(presenter->GetStatInet()==0){
	    	image1.setBitmap(touchgfx::Bitmap(BITMAP_LINUX_ID));image1.invalidate();
	     }
	    }
 void MainView::ViewTemp() {
	 uint8_t tempPOW=(uint8_t)presenter->getTempPow();
	 uint8_t tempSOUND=(uint8_t)presenter->getTempSound();
	 uint8_t FAN1=(uint8_t)presenter->getFan1();
	 uint8_t FAN2=(uint8_t)presenter->getFan2();
	    /*Progress bar power*/
	    Unicode::snprintfFloat(textTempPowerBuffer,TEXTTEMPPOWER_SIZE,"%0f",presenter->getTempPow());
	 	textTempPower.invalidate();
	 	lineProgressTempPow.setValue(tempPOW);
	 	lineProgressTempPow.invalidate();
	 	  /*Progress bar sound*/
	 	Unicode::snprintfFloat(textTempSoundBuffer,TEXTTEMPSOUND_SIZE,"%0f",presenter->getTempSound());
	 	textTempSound.invalidate();
	 	lineProgressTempSound.setValue(tempSOUND);
	 	lineProgressTempSound.invalidate();
	 	 /*Progress bar FAN1*/
	 	circleProgressFAN1.setValue(FAN1);
	 	circleProgressFAN1.invalidate();
	 	/*Progress bar FAN2*/
	 	circleProgressFAN2.setValue(FAN2);
	 	circleProgressFAN2.invalidate();
/*
 * change color Temp POW indicator
 */
	 	if(tempPOW<40){
	    lineProgressTempPowPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0x00, 0x9E, 0x0D));
	    lineProgressTempPow.setPainter(lineProgressTempPowPainter);
        }

        if((40<=tempPOW)&&(tempPOW <=50)){
	    lineProgressTempPowPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(6, 119, 196));
	    lineProgressTempPow.setPainter(lineProgressTempPowPainter);
	     }

        if((51<=tempPOW) && (tempPOW<=60)){
	    lineProgressTempPowPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xb7, 0xba, 0x00));
	    lineProgressTempPow.setPainter(lineProgressTempPowPainter);
	     }

        if(61<=tempPOW){
	    lineProgressTempPowPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xba,0x09, 0));
	    lineProgressTempPow.setPainter(lineProgressTempPowPainter);
	     }
        /*
         * change color Temp Sound indicator
         */
        if(tempSOUND<40){
        	lineProgressTempSoundPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0x00, 0x9E, 0x0D));
        	lineProgressTempSound.setPainter(lineProgressTempSoundPainter);
                }

        if((40<=tempSOUND) &&(tempSOUND<=50)){
                	lineProgressTempSoundPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(6, 119, 196));
                	lineProgressTempSound.setPainter(lineProgressTempSoundPainter);
        	     }

        if((51<=tempSOUND)&&(tempSOUND<=60)){
                	lineProgressTempSoundPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xb7, 0xba, 0x00));
                	lineProgressTempSound.setPainter(lineProgressTempSoundPainter);
        	     }

        if(61<=tempSOUND){
                	lineProgressTempSoundPainter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xba,0x09, 0));
                	lineProgressTempSound.setPainter(lineProgressTempSoundPainter);
        	     }
                /*
                 * change color Indicator FAN1
                 */
	 	if(FAN1<40){
	 		circleProgressFAN1Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0x00, 0x9E, 0x0D));
	 		circleProgressFAN1.setPainter(circleProgressFAN1Painter);
        }

        if((40<=FAN1)&&(FAN1 <=60)){
        	circleProgressFAN1Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(6, 119, 196));
        	circleProgressFAN1.setPainter(circleProgressFAN1Painter);
	     }

        if((61<=FAN1) && (FAN1<=80)){
        	circleProgressFAN1Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xb7, 0xba, 0x00));
        	circleProgressFAN1.setPainter(circleProgressFAN1Painter);
	     }

        if(81<=FAN1){
        	circleProgressFAN1Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xba,0x09, 0));
        	circleProgressFAN1.setPainter(circleProgressFAN1Painter);
	     }
        /*
                         * change color Indicator FAN2
                         */
        	 	if(FAN2<40){
        	 		circleProgressFAN2Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0x00, 0x9E, 0x0D));
        	 		circleProgressFAN2.setPainter(circleProgressFAN2Painter);
                }

                if((40<=FAN2)&&(FAN2 <=60)){
                	circleProgressFAN2Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(6, 119, 196));
                	circleProgressFAN2.setPainter(circleProgressFAN2Painter);
        	     }

                if((61<=FAN2) && (FAN2<=80)){
                	circleProgressFAN2Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xb7, 0xba, 0x00));
                	circleProgressFAN2.setPainter(circleProgressFAN2Painter);
        	     }

                if(81<=FAN2){
                	circleProgressFAN2Painter.setColor(touchgfx::Color::getColorFrom24BitRGB(0xba,0x09, 0));
                	circleProgressFAN2.setPainter(circleProgressFAN2Painter);
        	     }

        }
 void  MainView::FunVolUP()
 {
	 uint8_t ch;
/*
if(Count<80){
	  presenter->SetVolume(++Count);
}
 */
	 ch=3-in;

	 if(ch<3)
	 presenter->SetInput(++ch);
 }

 void  MainView::FunVolDown()
 {
	 uint8_t ch;
	 ch=3-in;
	 /*
	 if(Count>0){
		 presenter->SetVolume(--Count);
	      }
	      */
	 if(ch>0){
		 presenter->SetInput(--ch);
	 }

 }


void MainView::CheckVOL(){

	Count= presenter->GetVol();
		  	 Unicode::snprintf(textVolumeBuffer,TEXTVOLUME_SIZE,"%02d", Count);
		  	 textVolume.invalidate();

}
void MainView::ButPlayEnter(){
	uint8_t s;
	s=presenter->GetState();
if (s!=0){
	     presenter->Stop();
	     buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_PLAY_ID), touchgfx::Bitmap(BITMAP_PLAY_ID));
		 buttonPlay.invalidate();
       }
else{
	if(presenter->PlayRadio()){
		buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_RADIO_ID), touchgfx::Bitmap(BITMAP_RADIO_ID));
		buttonPlay.invalidate();
	}
}
}
void MainView::CheckIkonBat(){
	uint8_t s;
		 s=presenter->GetState();
		    if(s==1){
		    	buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_HDD_ID), touchgfx::Bitmap(BITMAP_HDD_ID));
		    	buttonPlay.invalidate();
		    }
		    if(s==2){
		    	buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_RADIO_ID), touchgfx::Bitmap(BITMAP_RADIO_ID));
		    	buttonPlay.invalidate();
		    }
		    if(s==3){
		      	buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BUD_ID), touchgfx::Bitmap(BITMAP_BUD_ID));
		    	buttonPlay.invalidate();
		    }
		    if(s==0){
		    	 buttonPlay.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_PLAY_ID), touchgfx::Bitmap(BITMAP_PLAY_ID));
		    	 buttonPlay.invalidate();
		    }
}
