#ifndef SCREEN1_VIEW_HPP
#define SCREEN1_VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include "main.h"
class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void buttonHourUpClicked();
    virtual void buttonHourDownClicked();
    virtual void buttonMinuteUpClicked();
    virtual void buttonMinuteDownClicked();
    virtual void buttonSaveClicked();
    void handleTickEvent();
    //{ presenter->saveHour(hour);}



protected:
    uint8_t hour;
    uint8_t minute;
};
extern  RTC_HandleTypeDef hrtc;
#endif // SCREEN1_VIEW_HPP
