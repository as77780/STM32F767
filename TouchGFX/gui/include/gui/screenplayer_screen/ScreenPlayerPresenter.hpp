#ifndef SCREENPLAYERPRESENTER_HPP
#define SCREENPLAYERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenPlayerView;

class ScreenPlayerPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenPlayerPresenter(ScreenPlayerView& v);

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

    virtual ~ScreenPlayerPresenter() {};
    uint8_t GetStatLogin(){return model->GetStatLogin();}
    uint8_t GetStatInet(){return model->GetStatInet();}

    uint8_t PlayBUD(){return model->play.PlayBUD();}
    uint8_t PlayRadio(){return  model->play.PlayRadio();}
    uint8_t PlayHDD(){return  model->play.PlayHDD();}
    void Stop(){ model->play.Stop();}
    void FF_Play() {model->play.FF_Play();}
    void Back_Play(){ model->play.Back_Play();}
    uint8_t GetState(){return  model->play.GetState();}
    void ResetPowerFlag(){model->ResetPowerFlag();}
    uint8_t CheckPowerFlag(){return model->CheckPowerFlag();}

private:
    ScreenPlayerPresenter();

    ScreenPlayerView& view;
};


#endif // SCREENPLAYERPRESENTER_HPP
