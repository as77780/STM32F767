#ifndef EQUALIZER_PRESENTER_HPP
#define EQUALIZER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class EqualizerView;

class EqualizerPresenter : public Presenter, public ModelListener
{
public:
    EqualizerPresenter(EqualizerView& v);

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

    virtual ~EqualizerPresenter() {};

private:
    EqualizerPresenter();

    EqualizerView& view;
};


#endif // EQUALIZER_PRESENTER_HPP
