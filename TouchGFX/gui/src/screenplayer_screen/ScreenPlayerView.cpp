#include <gui/screenplayer_screen/ScreenPlayerView.hpp>
#include "BitmapDatabase.hpp"

ScreenPlayerView::ScreenPlayerView()
{

}

void ScreenPlayerView::setupScreen()
{
    ScreenPlayerViewBase::setupScreen();

}
void ScreenPlayerView::handleTickEvent(){
	CheckIkonBat();
	if((presenter->CheckPowerFlag())==1){
		application().gotoMainScreenSlideTransitionSouth();
		   	}
}

void ScreenPlayerView::CheckIkonBat(){
	uint8_t s=presenter->GetState();
	    if(s==1){
	    	imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_HDD_ID));
	    	imagePlay.setAlpha(255);
	    	imagePlay.invalidate();
	    }
	    if(s==2){
	    	imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_RADIO_ID));
	    	imagePlay.setAlpha(255);
	    	imagePlay.invalidate();
	    }
	    if(s==3){
	    	imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_BUD_ID));
	    	imagePlay.setAlpha(255);
	    	imagePlay.invalidate();
	    }
	    if(s==0){
	    	imagePlay.setAlpha(0);
	    	imagePlay.invalidate();
	    }
}
void ScreenPlayerView::tearDownScreen()
{
    ScreenPlayerViewBase::tearDownScreen();
}

void ScreenPlayerView::PlayToBack()
  {
	presenter->Back_Play();
  }

void ScreenPlayerView::PlayToPlay()
  {
      // Override and implement this function in ScreenPlayer
  }

void ScreenPlayerView::PlayToFollow()
  {
	presenter->FF_Play();
  }

void ScreenPlayerView::PlayToStop()
  {
	presenter->Stop();
	imagePlay.setAlpha(0);
	imagePlay.invalidate();
  }

void ScreenPlayerView::PlayHDD()
  {
	if(presenter->PlayHDD()){
		imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_HDD_ID));
		imagePlay.setAlpha(255);
		imagePlay.invalidate();
		}

  }

void ScreenPlayerView::PlayRadio()
  {
	if(presenter->PlayRadio()){
		imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_RADIO_ID));
		imagePlay.setAlpha(255);
		imagePlay.invalidate();
		}
  }

void ScreenPlayerView::PlayBUD()
  {
	if(presenter->PlayBUD()){
			imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_BUD_ID));
		    imagePlay.setAlpha(255);
		    imagePlay.invalidate();
	}

  }
