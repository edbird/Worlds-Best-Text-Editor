#include "FunctionCallback.hpp"


#include "Window.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "Inputbox.hpp"


#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// BUFFER OPEN / SAVE
////////////////////////////////////////////////////////////////////////////////




// CTRL-O: open action
void fc_open(Window& current_window)
{

    // TODO: request a filename
    //current_window._ftm_->TestFunc();
    Inputbox *inputbox = new Inputbox();
    inputbox->SetPosition(0, 0);
    std::cout << "INPUTBOX..." << std::endl;
    inputbox->TestFunc2();
    inputbox->TestFunc();
    std::cin.get();

    /*
    std::cout << "Window address: " << &current_window << std::endl;
    std::cout << current_window.TestName() << std::endl;

    SDL_Event event;
    while(SDL_PollEvent(&event) == 0);
    std::cout << "event.type=" << event.type << std::endl;

    Label *l2 = new Label(*current_window._ftm_);
    l2->ProcessEvent(current_window, event, current_window._keyboard_, current_window._timer_);
    
    inputbox->ProcessEvent(current_window, event, current_window._keyboard_, current_window._timer_);
    std::cout << "ProcessEvent" << std::endl;
    inputbox->Draw(current_window._renderer_, current_window._timer_);
    inputbox->Draw(current_window._renderer_, current_window._timer_);

    // TODO: anchor
    //current_window.AddGUIObject(current_window.GenerateName(), inputbox);

    // TODO: check if valid filename returned in fc_inputbox_enter_pressed

    //open_action
    if(static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->NotSaved())
    {
        std::cout << "The buffer is not saved, cannot open" << std::endl;
    }
    else
    {
        // reset cursor position
        static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->ResetCursor();

        //current_window._guiobject_.at("textbox")->Open("buffer.txt");
        static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Open(static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->GetFilename());
        //std::cout << "File " << "buffer.txt" << " read, " << current_window._guiobject_.at("textbox")->Size() << " bytes" << std::endl;
        std::cout << "File " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->GetFilename() << " read, " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Size() << " bytes" << std::endl;
    }

    Label *l = new Label(*current_window._ftm_);
    current_window.AddGUIObject("0", l);
    */

}


