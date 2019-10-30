#ifndef SCREENPLAYERVIEW_HPP
#define SCREENPLAYERVIEW_HPP

#include <gui_generated/screenplayer_screen/ScreenPlayerViewBase.hpp>
#include <gui/screenplayer_screen/ScreenPlayerPresenter.hpp>

class ScreenPlayerView : public ScreenPlayerViewBase
{
public:
    ScreenPlayerView();
    virtual ~ScreenPlayerView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREENPLAYERVIEW_HPP
