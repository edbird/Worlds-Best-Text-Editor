#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP


#include "Keyboard.hpp"
#include "Actions.hpp"
#include "FunctionCallback.hpp"
#include "Label.hpp"


#include <SDL.h>


#include <vector>


// a single line input box
class Inputbox : public Label
{


    public:

    Inputbox(const FontTextureManager& ftm)
        : Label(ftm)
    {
        //init_action_keys();

        //SetText("This is an inputbox");
    }

    virtual ~Inputbox()
    {
    }



    virtual int ProcessEvent(Window& current_window, const SDL_Event& event, const Keyboard& keyboard, /*const CurrentKeyboardAction& ka_current,*/ Uint32 timer) override
    {
        std::cout << "Inputbox::ProcessEvent()" << std::endl;

        bool fired{false};

        if(event.type == SDL_KEYDOWN)
        {
            std::cout << "Event type was SDL_KEYDOWN" << std::endl;

            // get the modifier state
            // split into different modifier constants
            //const SDL_Keymod MOD{SDL_GetModState()};
            const bool MOD_SHIFT{keyboard.ShiftState()};
            const bool MOD_CTRL{keyboard.CTRLState()};
            const bool MOD_ALT{keyboard.AltState()};
            const bool MOD_GUI{keyboard.GUIState()};
            const bool MOD_LSHIFT{keyboard.LeftShiftState()};
            const bool MOD_NONE{!keyboard.ModState()};

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
                    fired = true;
                    break;
                }
            }


            // TODO: put in AKV
            if(!fired)
            {
                
                if((MOD_NONE || MOD_SHIFT) && !MOD_CTRL)
                {
                    char ch;
                    if(keyboard.GetChar(ch))
                    {
                        // TODO: class Inputbox does not have methods InsertAtCursor and CursorRight, but it SHOULD have these
                        //this->InsertAtCursor(ch);
                        //this->CursorRight();
                        // TODO: working here
                        std::string str(this->GetText());
                        str.push_back(ch);
                        this->SetText(str);
                        // TODO: settext should be only in label class,
                        // and Inputbox should NOT inherit from label

                        fired = true;
                    }
                }
                
            }

        }

        if(fired == true) return 1;
        else return 0;
    }

    virtual void Draw(SDL_Renderer *const renderer, const Uint32 timer) const //override
    {
        //std::cout << "Inputbox::Draw()" << std::endl;
        //std::cout << PosX() << " " << PosY() << " " << Width() << " " << Height() << std::endl;

        Label::Draw(renderer, timer);
        
        // TODO: Input box header, border
    }


    void Action(ActionKey::FunctionPointer_t function_pointer,
                SDL_Keycode keycode,
                SCAModState shift_state = SCAModState::NONE,
                SCAModState ctrl_state = SCAModState::NONE,
                SCAModState alt_state = SCAModState::NONE,
                SCAModState gui_state = SCAModState::NONE)
    {

        // add new action key
        _akv_.push_back(new ActionKey(function_pointer, keycode, shift_state, ctrl_state, alt_state, gui_state));

    }



    private:

    /*
    void init_action_keys()
    {

        // press RETURN, causes inputbox to send entered data back to host
        // not sure how this will be done yet
        ActionKey* ak_enter_pressed = new ActionKey(fc_inputbox_enter_pressed, SDLK_RETURN, SCAModState::NONE, SCAModState::NONE);

        _akv_.push_back(ak_enter_pressed);
        
    }
    */



    std::vector<ActionKey*> _akv_;


};


#endif
