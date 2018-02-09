#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <map>

// This class "wraps" the SDL keyboard input functionality
// It obtains the keys pressed and sets modifier states
// In addition, this class has the ability to re-map
// keys. The SDLK_Key map maps SDLK_Keys from one value
// to another.

class KeyboardKey
{

    public:

    enum class Keystate
    {
        UNDEFINED,
        FALSE, // not pressed
        TRUE // pressed
    };

    KeyboardKey()
        : _state_{Keystate::FALSE}
    {
        
    }

    void Set(const Keystate state)
    {
        _state_ = state;
    }

    private:

    Keystate _state_;

}

class Keyboard
{

    public:

    Keyboard()
        : _sdlk_keymap_
        {
            {, },
            {, }
        }
        , _lshift_state_{false}
        , _rshift_state_{false}
        , _lctrl_state_{false}
        , _rctrl_state_{false}
        , _lalt_state_{false}
        , _ralt_state_{false}
        , _lgui_state_{false}
        , _rgui_state_{false}
        , _nlock_state_{false}
        , _clock_state_{false}
        , _mode_state_{false}
    {
        // check if SDL_init TODO
        // check mods
        const SDL_Keymod MOD{SDL_GetModState()};

        // clear anyway
        //clear_modifiers();
        
        if(MOD & KMOD_NONE)
        {
        //    clear_modifiers();
        }

        //if(MOD & KMOD_LSHIFT)
        //  set_lshift(true);

        set_lshift(MOD & KMOD_LSHIFT);
        set_rshift(MOD & KMOD_RSHIFT);
        set_lctrl(MOD & KMOD_LCTRL);
        set_rctrl(MOD & KMOD_RCTRL);
        set_lalt(MOD & KMOD_LALT);
        set_ralt(MOD & KMOD_RALT);


    }

    // convert current state into ascii char
    // returns false if no ascii char
    // returns true if ascii char is valid
    bool GetChar(char * const return_ch) const
    {
        char ch{0};
        if(find())
        {

        }
        return false;
    }

    int ProcessEvent(const SDL_Event& event)
    {
        // get the state
        KeyboardKey::Keystate current_state{KeyboardKey::Keystate::UNDEFINED);
        if(event.type == SDL_KEYDOWN)
        {
            current_state = KeyboardKey::Keystate::TRUE;
        }
        else if(event.type == SDL_KEYUP)
        {
            current_state = KeyboardKey::Keystate::FALSE;
        }

        //const KeyboardKey *key{nullptr}

        // map the key using the keymap, then set the state in the second map
        const_iterator it{_sdlk_keymap_.find(event.key.keysym.sym)};
        if(it != _key.end())
        {
            const SDL_Keysym mapped_keysym{it->second};
            const_iterator it2{_key_.find(mapped_keysym)};
            if(it2 != _key_.end())
            {
                it2->Set(current_state);
            }
        }
        else
        {
            // this keysym is not in the map
        }


        }

        //if(event.key.keysym.scancode)
        //{
        //    
        //}

    }

    // return the state of any of the modifier keys
    // does not include LOCK keys (nlock, clock, mode)
    bool ModState() const
    {
        return _lshift_state_ || _rshift_state_ || \
               _lctrl_state_  || _rctrl_state_  || \ 
               _lalt_state_   || _ralt_state_   || \
               _lgui_state_   || _rgui_state_;
    }

    bool LockState() const
    {
        return _nlock_state_ || _clock_state_ || _mode_state_;
    }

    bool ShiftState() const
    {
        return _lshift_state_ || _rshift_state_;
    }

    bool CTRLState() const
    {
        return _lctrl_state_ || _rctrl_state_;
    }

    bool AltState() const
    {
        return _lalt_state_ || _ralt_state_;
    }

    bool LShiftState() const
    {

    }

    // TODO

    private:

    void clear_modifiers()
    {
        _lshift_state_ = false;
        _rshift_state_ = false;
        _lctrl_state_ = false;
        _rctrl_state_ = false;
        _lalt_state_ = false;
        _ralt_state_ = false;
        _lgui_state_ = false;
        _rgui_state_ = false;
        _nlock_state_ = false;
        _clock_state_ = false;
        _mode_state_ = false;
    }

    void set_lshift(const bool state)
    {
        _lshift_state_ = state;
        //if(_lshift_state_ || _rshift_state_)
        //{
        //    
        //}
    }

    void set_rshift(const bool state)
    {
        _rshift_state_ = state;
    }

    void set_lctrl_state(const bool state)
    {
        _lctrl_state_ = state;
    }

    void set_rctrl_state(const bool state)
    {
        _rctrl_state_ = state;
    }

    bool _lshift_state_;
    bool _rshift_state_;
    bool _lctrl_state_;
    bool _rctrl_state_;
    bool _lalt_state_;
    bool _ralt_state_;
    bool _lgui_state_;
    bool _rgui_state_;
    bool _nlock_state_;
    bool _clock_state_;
    bool _mode_state_;

    // should use scancodes?
    // this dual-map design allows multiple keys to be mapped to the same key
    
    // input to output map
    // an actual "map". maps keycodes
    std::map<const SDL_Keycode, const SDL_Keycode> _sdlk_keymap_;

    // keycode to KeyboardKey object which holds the "true" / "false"
    // state of the key (pressed or not pressed)
    // the output of the above map is used as the input to this map
    std::map<const SDL_Keycode, const KeyboardKey> _key_; // the keyboard keys

    // keycode to ascii character map
    KeyMap _keymap_; // 

};

#endif // KEYBOARD_HPP
