#include <gui/screenplayer_screen/ScreenPlayerView.hpp>
#include "BitmapDatabase.hpp"

ScreenPlayerView::ScreenPlayerView()
{

}

void ScreenPlayerView::setupScreen()
{
    ScreenPlayerViewBase::setupScreen();
}

void ScreenPlayerView::tearDownScreen()
{
    ScreenPlayerViewBase::tearDownScreen();
}

void ScreenPlayerView::PlayToBack()
  {
	R_Left_eth();
  }

void ScreenPlayerView::PlayToPlay()
  {
      // Override and implement this function in ScreenPlayer
  }

void ScreenPlayerView::PlayToFollow()
  {
	R_Right_eth();
  }

void ScreenPlayerView::PlayToStop()
  {
	R_stop_eth();
	imagePlay.setAlpha(0);
	imagePlay.invalidate();
  }

void ScreenPlayerView::PlayHDD()
  {
	if(presenter->GetStatLogin()){
		prog_pl_eth();
		imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_HDD_ID));
		imagePlay.setAlpha(255);
		imagePlay.invalidate();
		}

  }

void ScreenPlayerView::PlayRadio()
  {
	if(presenter->GetStatInet()){
		prog_eth();
		imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_RADIO_ID));
		imagePlay.setAlpha(255);
		imagePlay.invalidate();
		}
  }

void ScreenPlayerView::PlayBUD()
  {
	if(presenter->GetStatLogin()){
		prog_bud_eth();
		imagePlay.setBitmap(touchgfx::Bitmap(BITMAP_BUD_ID));
		    imagePlay.setAlpha(255);
		    imagePlay.invalidate();
	}

  }
