#include "FunctionCallback.hpp"


#include "Window.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "Inputbox.hpp"


#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// INPUTBOX CALLBACKS
////////////////////////////////////////////////////////////////////////////////

// TODO: should Window& be replaced with GUIObject& ?
// would require Window to become part of gui unless
// events for window are processed differently
void fc_inputbox_enter_pressed(Window& current_window)
{

    // get filename from inputbox contents
    const std::string filename{static_cast<Inputbox*>(current_window._guiobject_.at("inputbox"))->GetText()};

    // set textbox filename
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->SetFilename(filename);

}






////////////////////////////////////////////////////////////////////////////////
// EDITOR CHANGE MODES
////////////////////////////////////////////////////////////////////////////////

// define a function to call here, because this is a convenient place to put it
// until I find somewhere to move it
// TODO
void fc_enter_edit_mode(Window& current_window)
{
    if(current_window._editor_mode_ == EditorMode::NORMAL)
    {
        std::cout << "EDIT" << std::endl;
        current_window._editor_mode_ = EditorMode::EDIT;
    }
}

void fc_exit_edit_mode(Window& current_window)
{
    if(current_window._editor_mode_ == EditorMode::EDIT)
    {
        std::cout << "NORMAL" << std::endl;
        current_window._editor_mode_ = EditorMode::NORMAL;
    }
}


////////////////////////////////////////////////////////////////////////////////
// PROGRAM QUIT AND REQUEST QUIT
////////////////////////////////////////////////////////////////////////////////

// TODO: this won't work for multiple textboxes,
// in addition, current_textbox is accessable from current_window
void fc_quit_request(Window& current_window)
{
    std::cout << "QUIT" << std::endl;
    //quit_action
    // quit request action
    if(static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->NotSaved())
    {
        std::cout << "The buffer is not saved, cannot quit" << std::endl;
        std::cout << "CTRL+SHIFT+Q to quit anyway" << std::endl;
        // TODO: message above should be a variable, as user may
        // change key combination
        // TODO: better interactive error message here
    }
    else
    {
        current_window._quit_ = true;
    }
}


// CTRL + SHIFT + Q -> immediate quit, without save
void fc_quit_force(Window& current_window)
{
    std::cout << "FORCE QUIT" << std::endl;
    current_window._quit_ = true;
}



////////////////////////////////////////////////////////////////////////////////
// BUFFER OPEN / SAVE
////////////////////////////////////////////////////////////////////////////////

// CTRL-S: save action
void fc_save(Window& current_window)
{
    // TODO: request a filename
    //current_window.AddGUIObject(new Inputbox());

    //save_action
    //current_window._guiobject_.at("textbox")->Save("buffer.txt");
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Save();
    std::stringstream status_text;
    //status_text << "Save: buffer.txt, " << current_window._guiobject_.at("textbox")->Size() << " bytes";
    status_text << "Save: " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->GetFilename() << ", " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Size() << " bytes";
    static_cast<Label*>(current_window._guiobject_.at("status_label"))->SetText(status_text.str());
    //std::cout << "File " << "buffer.txt" << " written, " << current_window._guiobject_.at("textbox")->Size() << " bytes" << std::endl;
    std::cout << "File " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->GetFilename() << " written, " << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Size() << " bytes" << std::endl;
}


// CTRL-O: open action
void fc_open(Window& current_window)
{
    // TODO: request a filename
    Inputbox *inputbox = new Inputbox(*current_window._ftm_);
    inputbox->SetPosition(current_window.Width() / 2, current_window.Height() / 2);
    // TODO: anchor
    current_window.AddGUIObject(current_window.GenerateName(), inputbox);

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
}


////////////////////////////////////////////////////////////////////////////////
// DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_print_buffer(Window& current_window)
{
    std::cout << static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->Get() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// MANUAL SCROLL LINE AND SCROLL SUB LINE, DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_scroll_inc(Window& current_window)
{
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->ScrollDown();
}

void fc_scroll_dec(Window& current_window)
{
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->ScrollUp();
}

void fc_scroll_inc_sub(Window& current_window)
{
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->ScrollDownSub();
}

void fc_scroll_dec_sub(Window& current_window)
{
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->ScrollUpSub();
}


////////////////////////////////////////////////////////////////////////////////
// up down left right arrow keys
////////////////////////////////////////////////////////////////////////////////

void fc_up(Window& current_window)
{ 
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->CursorUp();
}

void fc_down(Window& current_window)
{ 
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->CursorDown();
}

void fc_left(Window& current_window)
{ 
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->CursorLeft();
}

void fc_right(Window& current_window)
{ 
    static_cast<Textbox*>(current_window._guiobject_.at("textbox"))->CursorRight();
}
