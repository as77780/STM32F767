#include <gui/screeninputmenu_screen/ScreenInputMenuView.hpp>
#include <gui/screeninputmenu_screen/ScreenInputMenuPresenter.hpp>

ScreenInputMenuPresenter::ScreenInputMenuPresenter(ScreenInputMenuView& v)
    : view(v)
{
}

void ScreenInputMenuPresenter::activate()
{
	model->ActivEnc();
}

void ScreenInputMenuPresenter::deactivate()
{
	 model->DisativEnc();
}
