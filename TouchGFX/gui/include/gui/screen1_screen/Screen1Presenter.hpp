#ifndef SCREEN1_PRESENTER_HPP
#define SCREEN1_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen1View;

class Screen1Presenter : public Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);

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

    virtual ~Screen1Presenter() {};
    void saveHour(uint8_t hour) { model->saveHour(hour);  }
     void saveMinute(uint8_t minute){ model->saveMinute(minute);  }
     uint8_t getHour(){ return model->getHour(); }
     uint8_t getMinute(){ return model->getMinute(); }
     uint8_t getSecond(){ return model->getSecond(); }

private:
    Screen1Presenter();

    Screen1View& view;
};


#endif // SCREEN1_PRESENTER_HPP
