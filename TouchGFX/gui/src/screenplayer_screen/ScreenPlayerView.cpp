#include <gui/screenplayer_screen/ScreenPlayerView.hpp>

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
  }

void ScreenPlayerView::PlayHDD()
  {
	if(presenter->GetStatLogin()){
		prog_pl_eth();
		}

  }

void ScreenPlayerView::PlayRadio()
  {
	if(presenter->GetStatInet()){
		prog_eth();
		}
  }

void ScreenPlayerView::PlayBUD()
  {
	if(presenter->GetStatLogin()){
		prog_bud_eth();
	}

  }
