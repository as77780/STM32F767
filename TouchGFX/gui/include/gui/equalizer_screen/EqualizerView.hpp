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
protected:
};

#endif // EQUALIZER_VIEW_HPP
