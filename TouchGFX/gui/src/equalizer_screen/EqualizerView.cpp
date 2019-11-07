#include <gui/equalizer_screen/EqualizerView.hpp>

EqualizerView::EqualizerView()
{

}

void EqualizerView::setupScreen()
{
    EqualizerViewBase::setupScreen();
    SliderAmplified.setValue(presenter->GetGain());
    SliderBass.setValue(presenter->GetBass());
    SliderMid.setValue(presenter->GetMiddle());
    SliderTreble.setValue(presenter->GetTrible());

}

void EqualizerView::tearDownScreen()
{
    EqualizerViewBase::tearDownScreen();
}
