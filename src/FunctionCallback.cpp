#include "FunctionCallback.hpp"


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
    if(current_window._textbox_ptr_->NotSaved())
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
    //save_action
    current_window._textbox_ptr_->Save("buffer.txt");
    std::stringstream status_text;
    status_text << "Save: buffer.txt, " << current_window._textbox_ptr_->Size() << " bytes";
    current_window._status_label_->SetText(status_text.str());
    std::cout << "File " << "buffer.txt" << " written, " << current_window._textbox_ptr_->Size() << " bytes" << std::endl;
}


// CTRL-O: open action
void fc_open(Window& current_window)
{
    //open_action
    if(current_window._textbox_ptr_->NotSaved())
    {
        std::cout << "The buffer is not saved, cannot open" << std::endl;
    }
    else
    {
        // reset cursor position
        current_window._textbox_ptr_->ResetCursor();

        current_window._textbox_ptr_->Open("buffer.txt");
        std::cout << "File " << "buffer.txt" << " read, " << current_window._textbox_ptr_->Size() << " bytes" << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
// DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_print_buffer(Window& current_window)
{
    std::cout << current_window._textbox_ptr_->Get() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// MANUAL SCROLL LINE AND SCROLL SUB LINE, DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_scroll_inc(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollDown();
}

void fc_scroll_dec(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollUp();
}

void fc_scroll_inc_sub(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollDownSub();
}

void fc_scroll_dec_sub(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollUpSub();
}


////////////////////////////////////////////////////////////////////////////////
// up down left right arrow keys
////////////////////////////////////////////////////////////////////////////////

void fc_up(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorUp();
}

void fc_down(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorDown();
}

void fc_left(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorLeft();
}

void fc_right(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorRight();
}
