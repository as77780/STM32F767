#include <gui/containers/InputCenterContainer.hpp>

InputCenterContainer::InputCenterContainer()
{

}

void InputCenterContainer::initialize()
{
    InputCenterContainerBase::initialize();
}

void InputCenterContainer::updateText(int16_t value)
{
    Unicode::snprintf(TextInputCenterBuffer, TEXTINPUTCENTER_SIZE, "%d", value);
    TextInputCenter.invalidate();
}
