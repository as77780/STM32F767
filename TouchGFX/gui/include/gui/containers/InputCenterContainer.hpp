#ifndef INPUTCENTERCONTAINER_HPP
#define INPUTCENTERCONTAINER_HPP

#include <gui_generated/containers/InputCenterContainerBase.hpp>

class InputCenterContainer : public InputCenterContainerBase
{
public:
    InputCenterContainer();
    virtual ~InputCenterContainer() {}

    virtual void initialize();
    void updateText(int16_t value);

protected:
};

#endif // INPUTCENTERCONTAINER_HPP
