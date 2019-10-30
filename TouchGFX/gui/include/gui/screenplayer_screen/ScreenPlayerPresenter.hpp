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

private:
    ScreenPlayerPresenter();

    ScreenPlayerView& view;
};


#endif // SCREENPLAYERPRESENTER_HPP
