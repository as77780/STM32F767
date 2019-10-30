#ifndef SCREEN2_VIEW_HPP
#define SCREEN2_VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include "main.h"

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void TimeView();
protected:
    int16_t hour;
    int16_t minute;
    int16_t second;
   };

extern  RTC_HandleTypeDef hrtc;
#endif // SCREEN2_VIEW_HPP
