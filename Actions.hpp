#ifndef ACTIONS_HPP
#define ACTIONS_HPP


#include <map>

#include <SDL.h>


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

enum class TriState
{
    FALSE,
    TRUE,
    DONT_CARE
};

// Shift Control Alt Modifier State
enum class SCAModState
{
    NONE, // left OFF, right OFF
    LEFT_ONLY, // left ON, right OFF
    LEFT, // left ON, right DONT_CARE (do not check right)
    RIGHT_ONLY, // left OFF, right ON
    RIGHT, // left DONT_CARE (do not check left), right ON
    BOTH / AND, // left ON, right ON (AND)
    ANY / OR, // any of left or right, if 1 or more is on then pass test (OR)
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

    public:

    ActionKey(SDL_Keycode keycode)
        : _keycode_{keycode}
    {
    }

    private:

    // the keycode which is associated with this action
    // NOTE: this can also include the modifier keys as well as
    // regular keys such as SDLK_A
    SDL_Keycode _keycode_;

    // set of modifier states
    //KeyboardModState _modstate_;
    TriState _any_shift_;
    TriState _left_shift_;
    TriState _right_shift_;

    TriState _any_ctrl_;
    TriState _left_ctrl_;
    TriState _right_ctrl_;



}

class Actions
{



};


#endif
