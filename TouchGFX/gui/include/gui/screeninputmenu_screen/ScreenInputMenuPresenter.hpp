#ifndef SCREENINPUTMENUPRESENTER_HPP
#define SCREENINPUTMENUPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenInputMenuView;

class ScreenInputMenuPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenInputMenuPresenter(ScreenInputMenuView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenInputMenuPresenter() {};
    void SetVol(int8_t vol) { model->SetVol(vol);  }
    uint8_t GetVolume(){return (model->GetVolume()) ;}
    void SetInput(uint8_t in){model->SetInput(in);}
    void ResetPowerFlag(){model->ResetPowerFlag();}
    uint8_t CheckPowerFlag(){return model->CheckPowerFlag();}

private:
    ScreenInputMenuPresenter();

    ScreenInputMenuView& view;
};


#endif // SCREENINPUTMENUPRESENTER_HPP
