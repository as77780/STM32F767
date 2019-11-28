#ifndef EQUALIZER_VIEW_HPP
#define EQUALIZER_VIEW_HPP

#include <gui_generated/equalizer_screen/EqualizerViewBase.hpp>
#include <gui/equalizer_screen/EqualizerPresenter.hpp>

class EqualizerView : public EqualizerViewBase
{
public:
    EqualizerView();
    virtual ~EqualizerView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void handleTickEvent();
protected:
    virtual void ChangeAmpl(int value)
       {
          presenter->SetGain(value);
       }

       virtual void ChangeBass(int value)
       {
    	   presenter->SetBass(value);
       }

       virtual void ChangeMid(int value)
       {
    	   presenter->SetMiddle(value);
       }

       virtual void ChangeTreble(int value)
       {
    	   presenter->SetTrible(value);
       }
};

#endif // EQUALIZER_VIEW_HPP
