#pragma once
struct ButtonState
{
    ButtonState()
    {
        m_onOffValue = false;
        m_selectionValue = false;
    }

    bool m_onOffValue;
    bool m_selectionValue;
};

