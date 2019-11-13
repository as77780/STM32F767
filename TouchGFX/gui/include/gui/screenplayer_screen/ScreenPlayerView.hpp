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
    virtual void PlayToBack();
    virtual void PlayToPlay();
    virtual void PlayToFollow();
    virtual void PlayToStop();
    virtual void PlayHDD();
    virtual void PlayRadio();
    virtual void PlayBUD();
};

#endif // SCREENPLAYERVIEW_HPP
