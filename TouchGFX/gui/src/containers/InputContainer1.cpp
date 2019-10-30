#include <gui/containers/InputContainer1.hpp>

InputContainer1::InputContainer1()
{

}

void InputContainer1::initialize()
{
    InputContainer1Base::initialize();
}

void InputContainer1::updateText(int16_t value)
{
    Unicode::snprintf(TextInputBuffer, TEXTINPUT_SIZE, "%d", value);
    TextInput.invalidate();
}

