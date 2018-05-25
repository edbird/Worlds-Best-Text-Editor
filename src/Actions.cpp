#include "Actions.hpp"

// TODO: this is the default one, swap code with below
bool operator==(const CurrentKeyboardAction &current_keyboard_action, const ActionKey &action_key)
{
    return action_key == current_keyboard_action;
}


bool operator==(const ActionKey &action_key, const CurrentKeyboardAction &current_keyboard_action)
{
    // check keycode
    //std::cout << "keycode" << std::endl;
    if(action_key._keycode_ != current_keyboard_action._keycode_)
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    // shift
    ////////////////////////////////////////////////////////////////////////////
    std::cout << "shift" << std::endl;
    if(action_key._shift_state_ == SCAModState::NONE)
    {
        //std::cout << "NONE" << std::endl;
        // both must be false
        if(!(current_keyboard_action._left_shift_state_ == Bistate::FALSE && current_keyboard_action._right_shift_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::LEFT_ONLY)
    {
        //std::cout << "LEFT_ONLY" << std::endl;
        // left must be true, right must be false
        if(!(current_keyboard_action._left_shift_state_ == Bistate::TRUE)) return false;
        if(!(current_keyboard_action._right_shift_state_ == Bistate::FALSE)) return false;
    }
    else if(action_key._shift_state_ == SCAModState::LEFT)
    {
        //std::cout << "LEFT" << std::endl;
        // left must be true, don't check right
        if(!(current_keyboard_action._left_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::RIGHT_ONLY)
    {
        //std::cout << "RIGHT_ONLY" << std::endl;
        // left must be false, right must be true
        if(!(current_keyboard_action._left_shift_state_ == Bistate::FALSE)) return false;
        if(!(current_keyboard_action._right_shift_state_ == Bistate::TRUE)) return false;
    }
    else if(action_key._shift_state_ == SCAModState::RIGHT)
    {
        //std::cout << "RIGHT" << std::endl;
        // right must be true, don't check left
        if(!(current_keyboard_action._right_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::BOTH)
    {
        //std::cout << "BOTH" << std::endl;
        // both left and right must be true
        if(!(current_keyboard_action._left_shift_state_ == Bistate::TRUE && current_keyboard_action._right_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::ANY)
    {
        //std::cout << "ANY" << std::endl;
        // either left or right must be true
        if(!(current_keyboard_action._left_shift_state_ == Bistate::TRUE || current_keyboard_action._right_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._shift_state_ == SCAModState::NOT_LEFT)
    {
        //std::cout << "NOT_LEFT" << std::endl;
        // left must be false, don't check right
        if(!(current_keyboard_action._left_shift_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::NOT_RIGHT)
    {
        //std::cout << "NOT_RIGHT" << std::endl;
        // right must be false, don't check left
        if(!(current_keyboard_action._right_shift_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::DONT_CARE)
    {
        // don't check either
    }
        


    ////////////////////////////////////////////////////////////////////////////
    // ctrl
    ////////////////////////////////////////////////////////////////////////////
    std::cout << "ctrl" << std::endl;
    if(action_key._ctrl_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::FALSE && current_keyboard_action._right_ctrl_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::TRUE)) return false;
        if(!(current_keyboard_action._right_ctrl_state_ == Bistate::FALSE)) return false;
    }
    else if(action_key._ctrl_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::FALSE)) return false;
        if(!(current_keyboard_action._right_ctrl_state_ == Bistate::TRUE)) return false;
    }
    else if(action_key._ctrl_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!(current_keyboard_action._right_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::TRUE && current_keyboard_action._right_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::TRUE || current_keyboard_action._right_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._ctrl_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!(current_keyboard_action._left_ctrl_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!(current_keyboard_action._right_ctrl_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::DONT_CARE)
    {
        // don't check either
    }

    

    ////////////////////////////////////////////////////////////////////////////
    // alt
    ////////////////////////////////////////////////////////////////////////////
    std::cout << "alt" << std::endl;
    if(action_key._alt_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(current_keyboard_action._left_alt_state_ == Bistate::FALSE && current_keyboard_action._right_alt_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!((current_keyboard_action._left_alt_state_ == Bistate::TRUE))) return false;
        if(!((current_keyboard_action._right_alt_state_ == Bistate::FALSE))) return false;
    }
    else if(action_key._alt_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!((current_keyboard_action._left_alt_state_ == Bistate::TRUE)))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!((current_keyboard_action._left_alt_state_ == Bistate::FALSE))) return false;
        if(!((current_keyboard_action._right_alt_state_ == Bistate::TRUE))) return false;
    }
    else if(action_key._alt_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!((current_keyboard_action._right_alt_state_ == Bistate::TRUE)))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!((current_keyboard_action._left_alt_state_ == Bistate::TRUE && current_keyboard_action._right_alt_state_ == Bistate::TRUE)))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!((current_keyboard_action._left_alt_state_ == Bistate::TRUE || current_keyboard_action._right_alt_state_ == Bistate::TRUE)))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._alt_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!((current_keyboard_action._left_alt_state_ == Bistate::FALSE)))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!((current_keyboard_action._right_alt_state_ == Bistate::FALSE)))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::DONT_CARE)
    {
        // don't check either
    }



    ////////////////////////////////////////////////////////////////////////////
    // gui
    ////////////////////////////////////////////////////////////////////////////
    std::cout << "gui" << std::endl;
    if(action_key._gui_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!((current_keyboard_action._left_gui_state_ == Bistate::FALSE && current_keyboard_action._right_gui_state_ == Bistate::FALSE)))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!((current_keyboard_action._left_gui_state_ == Bistate::TRUE))) return false;
        if(!((current_keyboard_action._right_gui_state_ == Bistate::FALSE))) return false;
    }
    else if(action_key._gui_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!((current_keyboard_action._left_gui_state_ == Bistate::TRUE)))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!(current_keyboard_action._left_gui_state_ == Bistate::FALSE)) return false;
        if(!(current_keyboard_action._right_gui_state_ == Bistate::TRUE)) return false;
    }
    else if(action_key._gui_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!(current_keyboard_action._right_gui_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(current_keyboard_action._left_gui_state_ == Bistate::TRUE && current_keyboard_action._right_gui_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(current_keyboard_action._left_gui_state_ == Bistate::TRUE || current_keyboard_action._right_gui_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._gui_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!(current_keyboard_action._left_gui_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!(current_keyboard_action._right_gui_state_ == Bistate::FALSE))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::DONT_CARE)
    {
        // don't check either
    }

    // if got to here, then nothing was wrong, continue
    return true;

};
