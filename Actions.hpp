#ifndef ACTIONS_HPP
#define ACTIONS_HPP


#include <map>

#include <SDL.h>

// TODO: cleanup this file

// a tristate class for modifier keys
enum class MODSTATE
{
    FALSE,
    TRUE,
    DONT_CARE
};

class ModState
{

    private:

    MODSTATE _modstate_;

};

// mod state of actual keypress
// TODO: use in keyboard classes
class KeyboardModstate
{

    public:

    KeyboardModstate(SDL_Keycode keycode)
        : _keydown_{false}
        , _keycode_{keycode}
    {
        
        // check if keycode is valid

        // list of valid key options
        if(_keycode_ == SDLK_LSHIFT)
        {}
        else if(_keycode_ == SDLK_RSHIFT)
        {}
        else if(_keycode_ == SDLK_LCTRL)
        {}
        else if(_keycode_ == SDLK_RCTRL)
        {}
        else if(_keycode_ == SDLK_LALT)
        {}
        else if(_keycode_ == SDLK_RALT)
        {}
        else if(_keycode_ == SDLK_LGUI)
        {}
        else if(_keycode_ == SDLK_RGUI)
        {}
        /*
        else if(_keycode_ == SDLK_NUMLOCKCLEAR)
        {}
        else if(_keycode_ == SDLK_CAPSLOCK)
        {}
        else if(_keycode_ == SDLK_MODE)
        {}
        */
        else
        {
            throw "key not allowed";
            // TODO: no other better way of doing this?
            // there is no "unspecified" key
            // so forced to check key is valid
        }

    }

    private:

    // is key pressed? true = pressed, false = not pressed
    bool _keydown_;
    // the associated keycode, should be one of
    // CTRL, SHIFT, ALT, etc
    SDL_Keycode _keycode_;

}

enum class Tristate
{
    FALSE,
    TRUE,
    DONT_CARE
};

enum class Bistate
{
    FALSE,
    TRUE
}

// Shift Control Alt Modifier State
enum class SCAModState
{
    NONE, // left OFF, right OFF
    LEFT_ONLY, // left ON, right OFF
    LEFT, // left ON, right DONT_CARE (do not check right)
    RIGHT_ONLY, // left OFF, right ON
    RIGHT, // left DONT_CARE (do not check left), right ON
    BOTH // / AND, // left ON, right ON (AND)
    ANY // / OR, // any of left or right, if 1 or more is on then pass test (OR)
    // weird / unusual combinatiONs which are not typically used:
    NOT_LEFT, // left OFF, right DONT_CARE (do not check right)
    NOT_RIGHT, // left DONT_CARE (do not check left), right OFF
    DONT_CARE
    
    // combinations are:
    //
    // L | R
    // --+--
    // 0 | 0 <+ Boolean only
    // 1 | 0  |
    // 0 | 1  |
    // 1 | 1 <+
    // --+--
    // 0 | X <+ Tri states included
    // X | 0  |
    // 1 | X  |
    // X | 1  |
    // X | X <+ (this is DONT_CARE)
    // --+--
    // X | 1  ||  1 | X <- "any"
    // 0 | 1  ||  1 | 0  ||  1 | 1 <- "== any" (at least one 1)
    // X | 0  ||  0 | X <- "Nany" (at least one 0)

}

class ActionKey
{

    friend
    bool operator==(const ActionKey &action_key, const ActionKey2 &action_key_2);


    public:

    ActionKey(SDL_Keycode keycode, SCAModState shift_state, SCAModState ctrl_state, SCAModState alt_state, SCAModState gui_state)
        : _keycode_{keycode}
        , _shift_state_{shift_state}
        , _ctrl_state_{ctrl_state}
        , _alt_state_{alt_state}
        , _gui_state_{gui_state}
    {
    }

    private:

    // the keycode which is associated with this action
    // NOTE: this can also include the modifier keys as well as
    // regular keys such as SDLK_A
    SDL_Keycode _keycode_;

    SCAModState _shift_state_;
    SCAModState _ctrl;
    SCAModState _alt_state_;
    SCAModState _gui_state_;

    // set of modifier states
    //KeyboardModState _modstate_;
    //Tristate _any_shift_;
    //Tristate _left_shift_;
    //Tristate _right_shift_;

    //Tristate _any_ctrl_;
    //Tristate _left_ctrl_;
    //Tristate _right_ctrl_;



};

class Actions
{



};


class ActionKey2
{

    friend
    bool operator==(const ActionKey &action_key, const ActionKey2 &action_key_2);

    public:

    ActionKey2(const Keyboard& keyboard)
    {
        // initialize using keyboard,
        // sets the keycode to the last pressed keycode
        // and sets modifier tristates
        // this constructor is used to construct an ActionKey object to
        // compare with predefined ActionKeys
        _keycode_ = keyboard.GetLastPressedKeycode();

        // the shift states must be either pressed or not pressed,
        // set them here
        if(keyboard.LeftShiftState()) _left_shift_state_ = Bistate::TRUE;
        else _left_shift_state_ = Bistate::FALSE;

        if(keyboard.RightShiftState()) _right_shift_state_ = Bistate::TRUE;
        else _right_shift_state_ = Bistate::FALSE;

        if(keyboard.LeftCTRLState()) _left_ctrl_state_ = Bistate::TRUE;
        else _left_ctrl_state_ = Bistate::FALSE;

        if(keyboard.RightCTRLState()) _right_ctrl_state_ = Bistate::TRUE;
        else _right_ctrl_state_ = Bistate::FALSE;

        if(keyboard.LeftShiftState()) _left_alt_state_ = Bistate::TRUE;
        else _left_alt_state_ = Bistate::FALSE;

        if(keyboard.RightShiftState()) _right_alt_state_ = Bistate::TRUE;
        else _right_alt_state_ = Bistate::FALSE;

        if(keyboard.LeftGUIState()) _left_gui_state_ = Bistate::TRUE;
        else _left_gui_state_ = Bistate::FALSE;

        if(keyboard.RightGUIState()) _right_gui_state_ = Bistate::TRUE;
        else _right_gui_state_ = Bistate::FALSE;
    }

    //ActionKey2(const SDL_Keycode keycode, Bistate shift_state, Bistate ctrl_state, Bistate alt_state, Bistate gui_state)
    //{
    //    _keycode_ = keycode;
    //
    //
    //}



    private:

    SDL_Keycode _keycode_;
    Bistate _left_shift_state_;
    Bistate _right_shift_state_;
    Bistate _left_ctrl_state_;
    Bistate _right_ctrl_state_;
    Bistate _left_alt_state_;
    Bistate _right_alt_state_;
    Bistate _left_gui_state_;
    Bistate _right_gui_state_;
    // TODO: locks

};


bool operator==(const ActionKey &action_key, const ActionKey2 &action_key_2);
{

    ////////////////////////////////////////////////////////////////////////////
    // shift
    ////////////////////////////////////////////////////////////////////////////
    if(action_key._shift_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(action_key_2._left_shift_state_ == Bistate::FALSE && action_key_2._right_shift_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!action_key_2._left_shift_state_ == Bistate::TRUE) return false;
        if(!action_key_2._right_shift_state_ == Bistate::FALSE) return false;
    }
    else if(action_key._shift_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!action_key_2._left_shift_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!action_key_2._left_shift_state_ == Bistate::FALSE) return false;
        if(!action_key_2._right_shift_state_ == Bistate::TRUE) return false;
    }
    else if(action_key._shift_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!action_key_2._right_shift_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(action_key_2._left_shift_state_ == Bistate::TRUE && action_key_2._right_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(action_key_2._left_shift_state_ == Bistate::TRUE || action_key_2._right_shift_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._shift_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!action_key_2._left_shift_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._shift_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!action_key_2._right_shift_state_ == Bistate::FALSE)
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
    if(action_key._ctrl_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(action_key_2._left_ctrl_state_ == Bistate::FALSE && action_key_2._right_ctrl_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!action_key_2._left_ctrl_state_ == Bistate::TRUE) return false;
        if(!action_key_2._right_ctrl_state_ == Bistate::FALSE) return false;
    }
    else if(action_key._ctrl_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!action_key_2._left_ctrl_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!action_key_2._left_ctrl_state_ == Bistate::FALSE) return false;
        if(!action_key_2._right_ctrl_state_ == Bistate::TRUE) return false;
    }
    else if(action_key._ctrl_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!action_key_2._right_ctrl_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(action_key_2._left_ctrl_state_ == Bistate::TRUE && action_key_2._right_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(action_key_2._left_ctrl_state_ == Bistate::TRUE || action_key_2._right_ctrl_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._ctrl_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!action_key_2._left_ctrl_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._ctrl_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!action_key_2._right_ctrl_state_ == Bistate::FALSE)
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
    if(action_key._alt_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(action_key_2._left_alt_state_ == Bistate::FALSE && action_key_2._right_alt_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!action_key_2._left_alt_state_ == Bistate::TRUE) return false;
        if(!action_key_2._right_alt_state_ == Bistate::FALSE) return false;
    }
    else if(action_key._alt_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!action_key_2._left_alt_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!action_key_2._left_alt_state_ == Bistate::FALSE) return false;
        if(!action_key_2._right_alt_state_ == Bistate::TRUE) return false;
    }
    else if(action_key._alt_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!action_key_2._right_alt_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(action_key_2._left_alt_state_ == Bistate::TRUE && action_key_2._right_alt_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(action_key_2._left_alt_state_ == Bistate::TRUE || action_key_2._right_alt_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._alt_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!action_key_2._left_alt_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._alt_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!action_key_2._right_alt_state_ == Bistate::FALSE)
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
    if(action_key._gui_state_ == SCAModState::NONE)
    {
        // both must be false
        if(!(action_key_2._left_gui_state_ == Bistate::FALSE && action_key_2._right_gui_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::LEFT_ONLY)
    {
        // left must be true, right must be false
        if(!action_key_2._left_gui_state_ == Bistate::TRUE) return false;
        if(!action_key_2._right_gui_state_ == Bistate::FALSE) return false;
    }
    else if(action_key._gui_state_ == SCAModState::LEFT)
    {
        // left must be true, don't check right
        if(!action_key_2._left_gui_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::RIGHT_ONLY)
    {
        // left must be false, right must be true
        if(!action_key_2._left_gui_state_ == Bistate::FALSE) return false;
        if(!action_key_2._right_gui_state_ == Bistate::TRUE) return false;
    }
    else if(action_key._gui_state_ == SCAModState::RIGHT)
    {
        // right must be true, don't check left
        if(!action_key_2._right_gui_state_ == Bistate::TRUE)
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::BOTH)
    {
        // both left and right must be true
        if(!(action_key_2._left_gui_state_ == Bistate::TRUE && action_key_2._right_gui_state_ == Bistate::TRUE))
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::ANY)
    {
        // either left or right must be true
        if(!(action_key_2._left_gui_state_ == Bistate::TRUE || action_key_2._right_gui_state_ == Bistate::TRUE))
        {
            return false;
        }
        // Note: there is no version for:
        // FALSE || FALSE
    }
    else if(action_key._gui_state_ == SCAModState::NOT_LEFT)
    {
        // left must be false, don't check right
        if(!action_key_2._left_gui_state_ == Bistate::FALSE)
        {
            return false;
        }
    }
    else if(action_key._gui_state_ == SCAModState::NOT_RIGHT)
    {
        // right must be false, don't check left
        if(!action_key_2._right_gui_state_ == Bistate::FALSE)
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

#endif
