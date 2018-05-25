#include "Textbox.hpp"



////////////////////////////////////////////////////////////////////////////
// Rendering helper functions
////////////////////////////////////////////////////////////////////////////


// Convert line number to string
std::string Textbox::line_number_to_string(const int line_number, const int line_number_width) const
{
    std::ostringstream ss;
    ss << std::setw(line_number_width) << std::setfill('0') << line_number;
    std::string line_number_str{ss.str()};
    return line_number_str;
}


// TODO: pass dst_rect by reference and modify within function
// TODO: remove _texture_chars_ arguments
void Textbox::print_line_number(const int line_number, const int line_number_width, const unsigned int print_index_y, SDL_Renderer *const renderer) const
{
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};

    // pass dst_rect by copy
    // dst_rect must be moved manually by calling function
    // Note: update, dst_rect managed by this function
    const unsigned int y_off{print_index_y * texture_chars_size.at(' ').h};
    SDL_Rect dst_rect{PosX(), PosY() + y_off, texture_chars_size.at(' ').w, texture_chars_size.at(' ').h};

    std::string line_number_str{line_number_to_string(line_number, line_number_width)};
    //SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
    std::string::const_iterator it{line_number_str.cbegin()};
    for(; it != line_number_str.cend(); ++ it)
    {
        char ch{*it};
        SDL_Texture *texture{texture_chars.at(ch)};
        SDL_Rect src_rect{0, 0, texture_chars_size.at(ch).w, texture_chars_size.at(ch).h};
        dst_rect.w = src_rect.w;
        dst_rect.h = src_rect.h;
        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
        dst_rect.x += src_rect.w;
    }
    
}


// print char
// TODO: optimize this, flag might not be required
// TODO: better if PosX() and PosY() were added here rather than adding them included with the dst_rect
// do this by passing by value
//void Textbox::print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
//                                    const SDL_Rect& src_rect, SDL_Rect& dst_rect) const
//{
//
//    // space for character already checked
//    // do print
//    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
//
//}













// compute line number width
unsigned int Textbox::get_line_number_width() const
{

    // get line count
    unsigned long long buffer_line_count{GetLineCount()};

    // compute line number width required
    unsigned int line_number_width = 0;
    if(_line_number_enabled_)
    {
        // set line number character count
        //int line_count{_buffer_.GetLineCount()};
        int line_count{buffer_line_count};
        for(;;)
        {
            ++ line_number_width;
            line_count = line_count / 10;
            if(line_count > 0)
            {
                continue;
            }
            else
            {
                break;
                // line_number_char_count contains correct value
                // line_count is invalid
            }
        }
    }

    return line_number_width;

}


void Textbox::update_wrap_count()
{
    wrap_count.clear();
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};
    
    
    ////////////////////////////////////////////////////////////////////////////
    // CODE FROM UPDATE FUNCTION
    ////////////////////////////////////////////////////////////////////////////
    
    // compute line number width required
    unsigned int line_number_width = get_line_number_width();

    // line width - the number of characters which can fit in the textbox in
    // the width direction
    std::size_t line_width{(std::size_t)(Width() / c_w - line_number_width)}; // TODO remove std::size_t ?
    
    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    // iterate over all lines in buffer
    //for(std::size_t line_ix{0 + _scroll_index_}; line_ix < buffer_contents.size(); ++ line_ix)
    for(std::size_t line_ix{0}; line_ix < buffer_contents.size(); ++ line_ix)
    {
        // compute the number of wrapped lines taken up by the current line
        std::size_t current_line_wrap_count{1 + buffer_contents[line_ix].size() / line_width};
        wrap_count.push_back(current_line_wrap_count);
    }

    // print the wrap count
    //for(std::size_t i{0}; i < wrap_count.size(); ++ i)
    //    std::cout << wrap_count[i] << ", ";
    //std::cout << std::endl;


}





void Textbox::CursorCR()
{
    _cursor_->CR();
}


void Textbox::InsertAtCursor(const char ch)
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


void Textbox::ReturnAtCursor()
{

    // current line and col
    Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
    Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};

    //std::cout << "c_col=" << c_col << std::endl;
    
    InsertNewLine(c_line, c_col);
}

// Note: motion of cursor was being done by the Window class
// it is now handled by Textbox class
// TODO: other functions
void Textbox::BackspaceAtCursor()
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
