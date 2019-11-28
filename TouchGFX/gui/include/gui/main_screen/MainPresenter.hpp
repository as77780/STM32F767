#ifndef MAIN_PRESENTER_HPP
#define MAIN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainView;

class MainPresenter : public Presenter, public ModelListener
{
public:
    MainPresenter(MainView& v);

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

    virtual ~MainPresenter() {};
    void saveHour(int16_t hour) { model->saveHour(hour);  }
        void saveMinute(int16_t minute){ model->saveMinute(minute);  }
        int16_t getHour(){ return model->getHour(); }
        int16_t getMinute(){ return model->getMinute(); }
        int16_t getSecond(){ return model->getSecond(); }
        float getTempPow(){return model->getTempPow();}
        float getTempSound(){return model->getTempSound();}
        uint8_t getFan1(){return model->getFan1();}
        uint8_t getFan2(){return model->getFan2();}
        void SetVolume(int8_t vol) { model->SetVolume(vol);  }
        uint8_t GetVol(){return (model->GetVol()) ;}
        uint8_t GetStatLogin(){return model->GetStatLogin();}
        uint8_t GetStatInet(){return model->GetStatInet();}
        void SetInput(uint8_t in){model->SetInput(in);}
        uint8_t GetState(){return model->play.GetState();}
        void ResetPowerFlag(){model->ResetPowerFlag();}
        uint8_t CheckPowerFlag(){return model->CheckPowerFlag();}

        uint8_t PlayBUD(){return model->play.PlayBUD();}
        uint8_t PlayRadio(){return  model->play.PlayRadio();}
        uint8_t PlayHDD(){return  model->play.PlayHDD();}
        void Stop(){ model->play.Stop();}

private:
    MainPresenter();

    MainView& view;
};


#endif // MAIN_PRESENTER_HPP
