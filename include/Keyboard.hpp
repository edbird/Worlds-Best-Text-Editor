#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <map>


#include "KeyMap.hpp"
#include "SDLK_KeyMap.hpp"


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

    Keystate Get() const
    {
        return _state_;
    }

    private:

    Keystate _state_;

};

class Keyboard
{

    public:

    Keyboard()
        : _mod_key_
        {
            {SDLK_LSHIFT, false},
            {SDLK_RSHIFT, false},
            {SDLK_LCTRL, false},
            {SDLK_RCTRL, false},
            {SDLK_LALT, false},
            {SDLK_RALT, false},
            {SDLK_LGUI, false},
            {SDLK_RGUI, false},
            {SDLK_NUMLOCKCLEAR, false},
            {SDLK_CAPSLOCK, false},
            {SDLK_MODE, false},
        }
        //, _key_
        //{
            // TODO ?
        //}
    {
        // check if SDL_init TODO
        // check mods
        const SDL_Keymod MOD{SDL_GetModState()};

        // clear anyway
        //clear_modifiers();
        
        if(MOD & KMOD_NONE)
        {
        //    clear_modifiers();
            //_mod_key_.ClearModifiers();
            ModKeyIterator_t it{_mod_key_.begin()};
            for(; it != _mod_key_.end(); ++ it)
            {
                it->second = false;
            }
        }

        //if(MOD & KMOD_LSHIFT)
        //  set_lshift(true);

        set_lshift_state(MOD & KMOD_LSHIFT);
        set_rshift_state(MOD & KMOD_RSHIFT);
        set_lctrl_state(MOD & KMOD_LCTRL);
        set_rctrl_state(MOD & KMOD_RCTRL);
        set_lalt_state(MOD & KMOD_LALT);
        set_ralt_state(MOD & KMOD_RALT);


    }

    // convert current state into ascii char
    // edit: convert SDL_Keycode into ascii char
    // returns false if no ascii char
    // returns true if ascii char is valid
    bool GetChar(SDL_Keycode keycode, char &return_ch) const
    {
        // map the keycode first
        SDL_Keycode mapped_keycode{keycode};
        if(MapSDLKeycode(mapped_keycode))
        {
            char ch{0};
            //if(_keymap_.Find(mapped_keycode, ch, _lshift_state_ || _rshift_state_))
            if(_keymap_.Find(mapped_keycode, ch, ShiftState()))
            {
                // if true, keycode is in the map and the output character is ch
                return_ch = ch;
                return true;
            }
            // _keymap_ does not contain mapped_keycode, second map failed
            return false;
        }
        // mapped_keycode is not valid, first map failed
        return false;
    }

    bool MapSDLKeycode(SDL_Keycode &keycode) const
    {
        SDL_Keycode mapped_keycode;
        if(_sdlk_keymap_.Find(keycode, mapped_keycode))
        {
            keycode = mapped_keycode;
            return true;
        }
        return false;
    }

    int ProcessEvent(const SDL_Event& event)
    {
        // get the state
        KeyboardKey::Keystate current_state{KeyboardKey::Keystate::UNDEFINED};
        if(event.type == SDL_KEYDOWN)
        {
            current_state = KeyboardKey::Keystate::TRUE;
        }
        else if(event.type == SDL_KEYUP)
        {
            current_state = KeyboardKey::Keystate::FALSE;
        }

        // map the key using the keymap, then set the state in the second map
        SDL_Keycode mapped_keysym{event.key.keysym.sym};
        if(MapSDLKeycode(mapped_keysym))
        {
        //if(_sdlk_keymap_.Find(event.key.keysym.sym, mapped_keysym))
        //{
            // mapped_keysym contains the SDL_Keycode to process
            KeyboardKeyIterator_t it{_key_.find(mapped_keysym)};
            if(it != _key_.end())
            {
                it->second.Set(current_state); // TODO: this part might be better in another function
            }
            else
            {
                // error, mapped_keysym is not in the list of keys!
                std::cout << "keysym is not in _key_" << std::endl;
            }

            // set the state variables
            set_mod_state();

        }
        else
        {
            // the map does not map the input key to any output key!
            // therefore this key is ignored
            // mapped_key is not valid!
            // this keysym is not in the map
        }

    }

    // return the state of any of the modifier keys
    // does not include LOCK keys (nlock, clock, mode)
    bool ModState() const
    {
        return _mod_key_.at(SDLK_LSHIFT) || _mod_key_.at(SDLK_RSHIFT) || \
               _mod_key_.at(SDLK_LCTRL)  || _mod_key_.at(SDLK_RCTRL)  || \
               _mod_key_.at(SDLK_LALT)   || _mod_key_.at(SDLK_RALT)   || \
               _mod_key_.at(SDLK_LGUI)   || _mod_key_.at(SDLK_RGUI);
    }

    bool LockState() const
    {
        //return _nlock_state_ || _clock_state_ || _mode_state_;
        return _mod_key_.at(SDLK_NUMLOCKCLEAR) || _mod_key_.at(SDLK_CAPSLOCK) || _mod_key_.at(SDLK_MODE);
    }

    bool ShiftState() const
    {
        //return _lshift_state_ || _rshift_state_;
        return _mod_key_.at(SDLK_LSHIFT) || _mod_key_.at(SDLK_RSHIFT);
    }

    bool CTRLState() const
    {
        //return _lctrl_state_ || _rctrl_state_;
        return _mod_key_.at(SDLK_LCTRL)  || _mod_key_.at(SDLK_RCTRL);
    }

    bool AltState() const
    {
        //return _lalt_state_ || _ralt_state_;
        return _mod_key_.at(SDLK_LALT)   || _mod_key_.at(SDLK_RALT);
    }

    bool LShiftState() const
    {
        return _mod_key_.at(SDLK_LSHIFT);
    }

    // TODO

    private:

    /*
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
    */

    // sets the mod keys state map using the regular key map
    void set_mod_state()
    {
        std::map<const SDL_Keycode, bool>::iterator it{_mod_key_.begin()}; // TODO: use typedef'd object
        for(; it != _mod_key_.end(); ++ it)
        {
            KeyboardKeyConstIterator_t it2{_key_.find(it->first)};
            if(it2 != _key_.end())
            {
                if(it2->second.Get() == KeyboardKey::Keystate::TRUE)
                {
                    it->second = true;
                }
                else
                {
                    it->second = false;
                }
            }
            // else: the mod key should be in _key_ but isnt
        }

    }

    void set_lshift_state(const bool state)
    {
        _mod_key_.at(SDLK_LSHIFT) = state;
    }

    void set_rshift_state(const bool state)
    {
        _mod_key_.at(SDLK_RSHIFT) = state;
    }

    void set_lctrl_state(const bool state)
    {
        _mod_key_.at(SDLK_LCTRL) = state;
    }

    void set_rctrl_state(const bool state)
    {
        _mod_key_.at(SDLK_RCTRL) = state;
    }

    void set_lalt_state(const bool state)
    {
        _mod_key_.at(SDLK_LALT) = state;
    }

    void set_ralt_state(const bool state)
    {
        _mod_key_.at(SDLK_RALT) = state;
    }

    /*
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
    */

    // another map, this one just for modifier keys
    std::map<const SDL_Keycode, bool> _mod_key_;
    typedef std::map<const SDL_Keycode, bool>::const_iterator ModKeyConstIterator_t;
    typedef std::map<const SDL_Keycode, bool>::iterator ModKeyIterator_t;


    // should use scancodes?
    // this dual-map design allows multiple keys to be mapped to the same key
    
    // input to output map
    // an actual "map". maps keycodes
    SDLK_KeyMap _sdlk_keymap_;
    //std::map<const SDL_Keycode, const SDL_Keycode> _sdlk_keymap_;

    // keycode to KeyboardKey object which holds the "true" / "false"
    // state of the key (pressed or not pressed)
    // the output of the above map is used as the input to this map
    std::map<const SDL_Keycode, KeyboardKey> _key_; // the keyboard keys
    typedef std::map<const SDL_Keycode, KeyboardKey>::const_iterator KeyboardKeyConstIterator_t; // TODO stupid name
    typedef std::map<const SDL_Keycode, KeyboardKey>::iterator KeyboardKeyIterator_t; // TODO stupid name


    // keycode to ascii character map
    KeyMap _keymap_; // this is not used in this class, but is used by other
                     // classes when testing the key states of this class

};

#endif // KEYBOARD_HPP
