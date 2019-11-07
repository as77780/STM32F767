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

    void SetGain(int8_t gain) { model->SetGain(gain);  }
    uint8_t GetGain(){return (model->GetGain()) ;}

    void SetBass(int8_t bass) { model->SetBass(bass);  }
    uint8_t GetBass(){return (model->GetBass()) ;}

    void SetMiddle(int8_t middle) { model->SetMiddle(middle);  }
    uint8_t GetMiddle(){return (model->GetMiddle()) ;}

    void SetTrible(int8_t trible) { model->SetTrible(trible);  }
    uint8_t GetTrible(){return (model->GetTrible()) ;}



private:
    EqualizerPresenter();

    EqualizerView& view;
};


#endif // EQUALIZER_PRESENTER_HPP
