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

private:
    MainPresenter();

    MainView& view;
};


#endif // MAIN_PRESENTER_HPP
