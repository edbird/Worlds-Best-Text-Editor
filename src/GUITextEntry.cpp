#include "GUITextEntry.hpp"


void GUITextEntry::ResetCursor()
{
    _cursor_->SetPos(0, 0);
}


const Cursor& GUITextEntry::GetCursor() const
{
    return _cursor_;
}


Cursor& GUITextEntry::MutableCursor()
{
    return _cursor_;
}


void GUITextEntry::InsertAtCursor(const char ch)
{

    // current line and col
    Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
    Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};

    //std::cout << "c_col=" << c_col << std::endl;
    
    Insert(ch, c_line, c_col);

    // TODO: not all chars increment?
    // incrementing is done by the sdl event loop
    //std::cout << "_line_text_.at(c_line)=" << _line_text_.at(c_line) << std::endl;
}


// Note: motion of cursor was being done by the Window class
// it is now handled by Textbox class
// TODO: other functions

// this function handles backspace in the general case of a buffer with multiple
// lines
void GUITextEntry::BackspaceAtCursor()
{

    // current line and col
    Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
    Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};
    
    std::cout << "c_line=" << c_line << ", c_col=" << c_col << std::endl;
        
    if(c_col > 0)
    {
        if(Delete(c_line, c_col) == true)
        {
            _cursor_->Left(); // TODO: probably always true?
        }
        else
        {
            // ?
        }
    }
    else
    {
        // TODO: move to else if
        if(c_line > 0)
        {
            Cursor::CursorPos_t goto_line{_cursor_->GetPosLine() - 1};
            std::size_t prev_line_length{LineLength(goto_line)};

            std::cout << "goto_line=" << goto_line << " prev_ll=" << prev_line_length << std::endl;

            if(Delete(c_line, c_col) == true)
            {
                _cursor_->SetPos(goto_line, prev_line_length);
            }
            else
            {
                // ?
            }
        }
    }  
    
}