#ifndef INPUTCONTAINER1_HPP
#define INPUTCONTAINER1_HPP

#include <gui_generated/containers/InputContainer1Base.hpp>

class InputContainer1 : public InputContainer1Base
{
public:
    InputContainer1();
    virtual ~InputContainer1() {}

    virtual void initialize();
    virtual void updateText(int16_t value);

protected:
};

#endif // INPUTCONTAINER1_HPP
