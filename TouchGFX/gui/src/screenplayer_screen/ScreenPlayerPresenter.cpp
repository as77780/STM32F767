#include <gui/screenplayer_screen/ScreenPlayerView.hpp>
#include <gui/screenplayer_screen/ScreenPlayerPresenter.hpp>

ScreenPlayerPresenter::ScreenPlayerPresenter(ScreenPlayerView& v)
    : view(v)
{
}

void ScreenPlayerPresenter::activate()
{
	model->ActivEnc();
}

void ScreenPlayerPresenter::deactivate()
{
	model->DisativEnc();
}
