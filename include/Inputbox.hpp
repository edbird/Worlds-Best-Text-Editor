#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP


#include "Keyboard.hpp"
#include "Actions.hpp"
#include "FunctionCallback.hpp"


#include <SDL.h>



// a single line input box
class Inputbox : public Label
{


    public:

    Inputbox(const FontTextureManager& ftm)
        : Label(ftm)
    {
        init_action_keys();
    }



    void ProcessEvent(Window& current_window, const SDL_Event& event, const Keyboard& keyboard, /*const CurrentKeyboardAction& ka_current,*/ Uint32 timer)
    {

        if(event.type == SDL_KEYDOWN)
        {
            // store the current key press and keyboard modifier states
            // in an ActionKey2 object
            CurrentKeyboardAction current_keyboard_action(keyboard);
            // TODO: this is slow, need the SDL_Keydown check but can pass as arg
            
            std::vector<ActionKey*>::iterator it{_akv_.begin()};
            for(; it != _akv_.end(); ++ it)
            {
                if(current_keyboard_action == **it)
                {
                    std::cout << "Inputbox::ProcessEvent() -> FIRE" << std::endl;
                    (*it)->Fire(current_window);
                    //fired = true;
                    break;
                }
            }

        }


    }


    private:

    void init_action_keys()
    {

        // press RETURN, causes inputbox to send entered data back to host
        // not sure how this will be done yet
        ActionKey* ak_enter_pressed = new ActionKey(fc_inputbox_enter_pressed, SDLK_RETURN, SCAModState::NONE, SCAModState::NONE);

        _akv_.push_back(ak_enter_pressed);
        
    }



    std::vector<ActionKey*> _akv_;


};


#endif
