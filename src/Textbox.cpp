#include "Textbox.hpp"



////////////////////////////////////////////////////////////////////////////
// Rendering helper functions
////////////////////////////////////////////////////////////////////////////


// Convert line number to string
std::string Textbox::line_number_to_string(const int line_number, const int line_number_width)
{
    std::ostringstream ss;
    ss << std::setw(line_number_width) << std::setfill('0') << line_number;
    std::string line_number_str{ss.str()};
    return line_number_str;
}


// TODO: pass dst_rect by reference and modify within function
// TODO: remove _texture_chars_ arguments
void Textbox::print_line_number(const int line_number, const int line_number_width, const unsigned int print_index_y, SDL_Renderer *const renderer)
{
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};

    // pass dst_rect by copy
    // dst_rect must be moved manually by calling function
    // Note: update, dst_rect managed by this function
    const unsigned int y_off{print_index_y * texture_chars_size.at(' ').h};
    SDL_Rect dst_rect{_pos_x_, _pos_y_ + y_off, texture_chars_size.at(' ').w, texture_chars_size.at(' ').h};

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
// TODO: better if _pos_x_ and _pos_y_ were added here rather than adding them included with the dst_rect
// do this by passing by value
void Textbox::print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
                                    const SDL_Rect& src_rect, SDL_Rect& dst_rect)
{

    // space for character already checked
    // do print
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

}


void Textbox::Draw(SDL_Renderer *const renderer, const Uint32 timer)
{

    // line wrap count, line number count, line number draw boolean, std::vector<std::string>
    
    // _buffer_line_count_ ?
    // _first_line_index_ ?
    
    // cursor_x, cursor_y
    
    // line width
    // line number width
    // line number enabled
    
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};
    
    
    ////////////////////////////////////////////////////////////////////////////
    // CODE FROM UPDATE FUNCTION
    ////////////////////////////////////////////////////////////////////////////
    
    
    /// COMPUTE LINE NUMBER WIDTH ///
    
    // get line count
    unsigned long long buffer_line_count{GetLineCount()};

    // compute line number width required
    unsigned int line_number_width = get_line_number_width();    
    
    /// CURSOR ///
    
    // get cursor position (input)
    Cursor::CursorPos_t cursor_line{_cursor_->GetPosLine()};
    Cursor::CursorPos_t cursor_col{_cursor_->GetPosCol()};
    // translate cursor position (output)
    unsigned int cursor_x = 0;
    unsigned int cursor_y = 0;
    
    
    ////////////////////////////////////////////////////////////////////////////
    // DRAW BUFFER TEXT
    ////////////////////////////////////////////////////////////////////////////

    // TODO: remove pseudovariables
    //SDL_Renderer *_renderer_{renderer};
    //Buffer &_buffer_{*this};
    //const std::map<const char, SDL_Texture*>& _texture_chars_{texture_chars};
    //const std::map<const char, SDL_Rect>& _texture_chars_size_{texture_chars_size};



    ////////////////////////////////////////////////////////////////////////////
    // DRAWING INIT
    ////////////////////////////////////////////////////////////////////////////
    
    // Set rendering space and render to screen
    // size of individual characters
    // position set to origin of screen and character 'a' (first
    // character in the character string)
    //int dst_rect_origin_x{_pos_x_}; // origin x for text, leaving space for line numbers

    // Move dst_rect_origin_x if line numbers are enabled
    // this is the starting x position for text drawing
    // TODO: add 4 pixels to this value and check code still works, need to adjust filled rect positions
    // and add a new one for this 4 pixel strip
    int dst_rect_origin_x{line_number_width * c_w};

    // current line number to print, starts from 1
    //int line_number{1};
    
    // Initialize source rect for character drawing
    // this is always the same, unless the character width and height changes
    // all char textures start at 0,0
    SDL_Rect src_rect{0, 0, c_w, c_h};
    
    // Initialize destination rect for line number printing
    //SDL_Rect dst_rect_line_number{_pos_x_, _pos_y_, texture_chars_size.at(' ').w, texture_chars_size.at(' ').h};

    // Initialize destination rect for character printing
    //SDL_Rect dst_rect{dst_rect_origin_x, _pos_y_, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};

    // Initialize cursor destination rect for cursor drawing
    //SDL_Rect cursor_texture_dst_rect{dst_rect_origin_x + _pos_x_, _pos_y_, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
    
    // Testing purposes
    int overhang{_size_x_ % (texture_chars_size.at(' ').w)};
    int y_overhang{_size_y_ % (texture_chars_size.at(' ').h)};
    SDL_Rect rect1{_pos_x_, _pos_y_, _size_x_ - overhang, _size_y_ - y_overhang};
    SDL_Rect rect2{_pos_x_ + _size_x_ - overhang, _pos_y_, overhang, _size_y_ - y_overhang};
    SDL_Rect rect3{_pos_x_, _pos_y_ + _size_y_ - y_overhang, _size_x_ - overhang, y_overhang};
    SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
    SDL_RenderFillRect(renderer, &rect1);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect3);
    
    // line width - the number of characters which can fit in the textbox in
    // the width direction
    std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
    std::size_t line_count{(_size_y_ / c_h)};
    
    
    ////////////////////////////////////////////////////////////////////////////
    // DRAWING
    ////////////////////////////////////////////////////////////////////////////
    
    /// COMBINATION OF UPDATE AND DRAW CODE ///
    
    
    // Note: if a complete line cannot be drawn it is not drawn
    // Note: Actually I think lines which are split are partially drawn!
    // Note: Need to fix this behaviour
    // TODO

    // TODO what if line wrap count size is 0
    // initial check to see if characters can be drawn
    // break if drawing beyond the end of the textbox drawing area

    
    
    
    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    // substring start position
    std::size_t substr_pos{0};
    std::size_t substr_len{0};
    
    
    // current line number to print, starts from 1
    unsigned int line_number{1 + _scroll_index_};
    
    // line index for printing line numbers and characters
    unsigned int print_index_y{0}; // this variable like line_ix, but increments when lines are wrapped
    // Note: could have used `dst_rect_line_number`, however I have chosen to use a single variable
    // as a counter instead and keep `dst_rect_line_number` within the scope of where it is needed
    // TODO: this is slightly slower, but better code?
    
    // iterate over all lines in buffer
    bool quit_now = false; // TODO: this is a hack
    //std::size_t scroll_index_counter{0};
    std::cout << _scroll_index_ << " " << _scroll_sub_index_ << " <- si, ssi" << std::endl;
    for(std::size_t line_ix{_scroll_index_}; line_ix < buffer_contents.size() && quit_now == false; ++ line_ix)//, ++ scroll_index_counter)
    {

        // only draw line number if the scroll sub index is 0
        // if 0, this line not scrolled
        if(_scroll_sub_index_ == 0 || line_ix > _scroll_index_)
            // TODO: && scroll_index_counter < _scroll_index_ + number of lines that can be drawn
        {
            // draw line number
            // TODO should go outside in case size is zero?
            // Print line number //zero
            if(_line_number_enabled_ == true)
            {
                // TODO: are these needed anymore?
                //if(check_space_y(dst_rect_line_number))
                //{
                //SDL_Rect dst_rect_line_number{_pos_x_ + 0, _pos_y_ + print_index_y * c_h, c_w, c_h};
                //print_line_number(line_number, line_number_width, dst_rect_line_number, renderer);
                // set the line number
                line_number = 1 + line_ix;
                print_line_number(line_number, line_number_width, print_index_y, renderer);
                //++ line_number;
                //dst_rect_line_number.y += dst_rect_line_number.h;
                //}
                
                // TODO: line number at bottom of window is printed even if no text
                // is printed! (due to text being wrapped and entire line not fitting
                // in window)
            }
        }

            
        
            // set substring position to start
            // set length to zero
            substr_pos = 0;
            substr_len = 0;
        
        
            // compute the number of wrapped lines taken up by the current line
            std::size_t current_line_wrap_count{1 + buffer_contents[line_ix].size() / line_width};
            
            // check for each new line number to be printed
            // break if drawing beyond the end of the textbox drawing area
            // if the current line, including the number of wraps, will fix in the remaining textbox space
            //if((line_ix - _scroll_index_) + current_line_wrap_count < line_count) // lt or leq? TODO
            if(print_index_y + current_line_wrap_count < line_count) // lt or leq? TODO
            {
        
                // chop the line up (wrap the line)
                //for(bool exit{false}; exit == false; )
                std::size_t scroll_sub_index_counter{0};
                for(;;)
                {

                    // true if the remaining text in the line needs to be
                    // wrapped again, in other words, this is the escape
                    // condition of the infinite loop
                    //bool wrapped{false};
                    bool exit{true};

                    // compute substr_len size
                    if(buffer_contents[line_ix].size() - substr_pos > line_width)
                    {
                        // remaining contents in line too big to fix, set
                        // substring length to maximum size
                        substr_len = line_width;

                        //wrapped = true;
                        exit = false;

                        // increment wrap count vector last element
                        //++ _line_wrap_count_.back();
                    }
                    else
                    {
                        // remaining contents in line will fit
                        substr_len = buffer_contents[line_ix].size() - substr_pos;
                    }

                    // copy
                    // TODO: remove variable next_line
                    // optimize out
                    //std::string next_line{buffer_contents[line_ix].substr(substr_pos, substr_len)};
                    //substr_pos += substr_len;
                    //std::cout << next_line << std::endl;
                    //_matrix_.emplace_back(next_line);
                    //_line_number_.emplace_back(first);
                    //first = false;
                    
                    // print the substring contained next_line
                    // print characters on this line
                    //for(std::size_t char_ix{0}; char_ix < next_line.size(); ++ char_ix)
                    //for(std::size_t char_ix{substr_pos}; char_ix < substr_pos + substr_len; ++ char_ix)
                    // only print the wrapped subline if the scroll counter is high enough
                    if(scroll_sub_index_counter >= _scroll_sub_index_ || line_ix > _scroll_index_)
                        // TODO: should move this if statement outside and have the counters counting as a priority over
                        // the if statements
                    {
                        //std::cout << "print split line, scroll_sub_index_counter=" << scroll_sub_index_counter << std::endl;
                        // print the wrapped substring / sub part of line
                        for(std::size_t char_ix{0}; char_ix < substr_len; ++ char_ix)
                        {
                        
                            //const int c_w{texture_chars_size.at(' ').w};
                            //const int c_h{texture_chars_size.at(' ').h};
                        
                            //const int x_off{c_w * (char_ix - substr_pos)};
                            const int x_off{c_w * char_ix};
                            //const int y_off{c_h * (line_ix - _scroll_index_)}; // line_ix here is WRONG if scroll != 0 TODO (FIXED)
                            const int y_off{c_h * print_index_y};
                            // TODO: line_ix is also wrong if lines are wrapped
                        
                            // character dst rect
                            SDL_Rect dst_rect{_pos_x_ + dst_rect_origin_x + x_off, _pos_y_ + y_off, c_w, c_h};
                        
                            // character to print
                            //const char ch{next_line[char_ix]};
                            const char ch{buffer_contents[line_ix][char_ix + substr_pos]};
                        
                            // set the texture pointer
                            SDL_Texture *texture{texture_chars.at(ch)};
                            print_char_texture(renderer, texture, src_rect, dst_rect);
                        }
                        
                        // line wrapped, and subline was printed,
                        // finished wrapping, increment the y position index
                        ++ print_index_y;

                    }

                    // increment the counter regardless of whether printing was done
                    ++ scroll_sub_index_counter;
                    
                    // moved to here because this variable needed for for loop index
                    substr_pos += substr_len;
                    
                    // line wrapped, finished wrapping, increment the y position index
                    //++ print_index_y;

                    //int window_line_count{_size_y_ / c_h}; // the number of lines there is space for in the textbox window
                    //if(print_index_y >= window_line_count) break;
                    //if(print_index_y >= line_count) quit_now = true;
                    // TODO: can remove ^^^


                    //if(substr_len == line_width)
                    //if(wrapped == true)
                    //{
                    //    substr_pos += substr_len;
                    //    //substr_len = 0; // TODO: remove?
                    //}
                    //else
                    //{
                    //    // finished this line
                    //    break;
                    //}
                    
                    if(exit == true) break;
                    
                    
                    // TODO: move into for head
                    //++ scroll_index_counter;

                } // chop up the current line (wrap)
                
                // finished chopping and printing line (wrapping and printing)
                
                // TODO: this does not print line numbers correctly!
                // some lines are wrapped!
                // Might be able to migrate this code into the Update function
                // (change Update to draw and draw instead of storing in a container)
                //if(_line_number_enabled_ && line_ix + 1 < _matrix_.size())
                
                // if there are more lines in the buffer to draw, then draw the line number
                // for the next line
                /*
                if(_line_number_enabled_ && line_ix + 1 < buffer_contents.size())
                {
                    //dst_rect_line_number.y += dst_rect_line_number.h;

                    //if(_line_number_[line_ix + 1] == true)
                    //{
                    SDL_Rect dst_rect_line_number{_pos_x_ + 0, _pos_y_ + print_index_y * c_h, c_w, c_h};
                    print_line_number(line_number, _line_number_width_, dst_rect_line_number, renderer);
                    ++ line_number;
                    //}
                }
                */ // this is already done when next loop
        
        
        
        
            } // if the current line, including the number of wraps, will fix in the remaining textbox space
            else
            {
                quit_now = true;
            }
        
        
            // set cursor x and y
            if(line_ix == cursor_line)
            {
                cursor_y += cursor_col / line_width;
                cursor_x += cursor_col % line_width;
            }
            else if(line_ix < cursor_line)
            {
                cursor_y += (buffer_contents[line_ix].size() - 1) / line_width + 1;
            }

        //else
        //{
        //    ++ line_number;
        //    // TODO: move into for loop head
        //}

        //++ scroll_index_counter;
    }


    ////////////////////////////////////////////////////////////////////////
    // Print cursor
    ////////////////////////////////////////////////////////////////////////
    //const int c_h{texture_chars_size.at(' ').h};
    //const int c_w{texture_chars_size.at(' ').w};
    const int x_off{c_w * cursor_x};
    const int y_off{c_h * cursor_y};
    SDL_Rect cursor_texture_dst_rect{dst_rect_origin_x + _pos_x_ + x_off, _pos_y_ + y_off, c_w, c_h};

    // make cursor blink (TODO: use config option)
    //int cursor_blink_rate{500}; // TODO
    //if(_timer_ / 500 % 2 == 0)
    //{
    //    std::cout << "0" << std::endl;
    //    SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0xFF, 0x00, 0xFF));
    //}
    //else if(_timer_ / 500 % 2 == 1)
    //{
    //    std::cout << "1" << std::endl;
    //    SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0x80, 0x00, 0xA0));
    //}

    // TODO: bug when cursor goes past end of buffer drawing
    // TODO: cursor color
    // TODO: what to do when cursor is out of screen range?
    // TODO: MULTIPLE TEXTURES REQUIRED BECAUSE COLOR CHANGE!
    //SDL_Texture* _current_cursor_texture_ptr_{_cursor_texture_.at(_current_cursor_)};
    //if(_timer_ / 500 % 2 == 1)
    //{
        //_current_cursor_texture_ptr_ = _cursor_texture_.at(3); // TODO: FIX THIS SHOULD NOT BE A CONST!
    //SDL_Rect cursor_texture_dst_rect{dst_rect_origin_x + _texture_chars_size_.at(' ').w * _cursor_->GetPosCol(), _pos_y_ + _texture_chars_size_.at(' ').h * _cursor_->GetPosLine(), _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
    _cursor_->Draw(renderer, cursor_texture_dst_rect, timer);
    // TODO: passing timer here is a bit weird, better to do Cursor.Update(timer) somewhere else
    // before this function call?


}



void Textbox::ScrollDown()
{
    // always need to update the wrap count vector before accessing it
    //update_wrap_count(); // not accessed in this function

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_index_ < buffer_contents.size() - 1)
    {
        ++ _scroll_index_;
    }
    else
    {
        std::cout << "Textbox::ScrollDown(): cannot scroll down" << std::endl;
    }
}


void Textbox::ScrollUp()
{
    // always need to update the wrap count vector before accessing it
    //update_wrap_count(); // not accessed in this function

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_index_ > 0)
    {
        -- _scroll_index_;
    }
    else
    {
        std::cout << "Textbox::ScrollUp(): cannot scroll up" << std::endl;
    }
}


void Textbox::ScrollDownSub()
{
    // always need to update the wrap count vector before accessing it
    update_wrap_count();

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    std::cout << _scroll_index_ << " SI" << std::endl;
    if(_scroll_sub_index_ < wrap_count.at(_scroll_index_) - 1)
    {
        std::cout << "A" << std::endl;
        ++ _scroll_sub_index_;
    }
    else
    {
        std::cout << "B" << std::endl;

        if(_scroll_index_ < buffer_contents.size() - 1)
        {
        std::cout << "C" << std::endl;
            _scroll_sub_index_ = 0;
            ++ _scroll_index_;
        }
        else
        {
            std::cout << "Textbox::ScrollDownSub(): cannot scroll down" << std::endl;
        }
    }
}


void Textbox::ScrollUpSub()
{
    // always need to update the wrap count vector before accessing it
    update_wrap_count();

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_sub_index_ > 0)
    {
        -- _scroll_sub_index_;
    }
    else
    {
        if(_scroll_index_ > 0)
        {
            _scroll_sub_index_ = wrap_count.at(_scroll_index_ - 1) - 1;
            -- _scroll_index_;
        }
        else
        {
            std::cout << "Textbox::ScrollUpSub(): cannot scroll up" << std::endl;
        }
    }
}



// TODO: should the buffer be responsible for setting the cursor
// position or should the cursor be responsible for setting its own
// bounds ?
void Textbox::CursorLeft()
{
    //_cursor_->Left();
    //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
    //if(_col_ > 0)
    if(_cursor_->GetPosCol() > 0)
        _cursor_->Left();
}


void Textbox::CursorRight()
{
    //const std::vector<std::string>& _line_text_{_buffer_.GetContainer()};
    const std::vector<std::string>& _line_text_{GetContainer()};

    // TODO DEBUG
    //if(_col_ < line_size)
    if(_cursor_->GetPosCol() < _line_text_.at(_cursor_->GetPosLine()).size())
        _cursor_->Right(); // TODO
    //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
}


// TODO: I actually want "cursor down" to move down 1 position, not 1 whole line
// because I want to be able to move within wrapped lines using the up and down
// arrows, this is not how vim works
// however this would not work with simple macro recording. macro recording would
// have to translate cursor positions into delta positions using line/char indices
// rather than the naieve number of up/down keys pressed


// TODO: remember target line position
// TODO: config: set rememberlineposition
void Textbox::CursorUp()
{

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};
    
    if(_cursor_->GetPosLine() > 0)
    {
        std::size_t line_size{buffer_contents.at(_cursor_->GetPosLine() - 1).size()};
        Cursor::CursorPos_t cursor_pos{_cursor_->GetPosCol()};
        Cursor::CursorPos_t cursor_pos_target{_cursor_->GetTargetCol()};
        if(cursor_pos_target > line_size)
        {
            // target position is too far along the line
            // as the line is too short!
            // check against the current cursor position
            // rather than the target cursor position
            // which is set whenever the user moves left / right
            if(cursor_pos > line_size)
            {
                cursor_pos = line_size;
            }
            else
            {
                // _cursor_pos_ has the correct value
                // don't do anything
            }
        }
        else
        {
            // target position is ok:
            // set the cursor position to be the target position
            cursor_pos = cursor_pos_target;
        }
        //_cursor_->SetPos(_cursor_->GetPosLine() - 1, cursor_pos);

        // check cursor position does not scroll off end of window
        if(_cursor_->GetPosLine() - 1 - _scroll_index_ > 0) // TODO: are these signed? does it matter?
        {
            _cursor_->SetPos(_cursor_->GetPosLine() - 1, cursor_pos);
            // TODO: appears twice, move out of if statement
        }
        else
        {
            std::cout << "trying to scroll up" << std::endl;
            // TODO: this isn't going to work, can scroll off end of screen
            if(_scroll_index_ > 0)
            {
                -- _scroll_index_;
            }
            _cursor_->SetPos(_cursor_->GetPosLine() - 1, cursor_pos);
            // TODO: appears twice, move out of if statement
        }
    }
    else
    {
        // _line_ is 0, do nothing
        std::cout << "cannot go up" << std::endl;
    }
}


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
    std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
    
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


#if 0
void Textbox::CursorDown()
{

    // TODO: find some way of avoiding running this every time, cannot have
    // a modified flag in this class because the class it inherits from
    // does the modifying with Insert() etc - could overload these functions
    // however
    update_wrap_count();
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};


    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};
    

    std::cout << "cursor down" << std::endl;
    if(_cursor_->GetPosLine() < buffer_contents.size() - 1)
    {
        std::cout << "first if" << std::endl;
        std::size_t line_size{buffer_contents.at(_cursor_->GetPosLine() + 1).size()};
        Cursor::CursorPos_t cursor_pos{_cursor_->GetPosCol()};
        Cursor::CursorPos_t cursor_pos_target{_cursor_->GetTargetCol()};
        if(cursor_pos_target > line_size)
        {
            // target position is too far along the line
            // as the line is too short!
            // check against the current cursor position
            // rather than the target cursor position
            // which is set whenever the user moves left / right
            if(cursor_pos > line_size)
            {
                cursor_pos = line_size;
            }
            else
            {
                // _cursor_pos_ has the correct value
                // don't do anything
            }
        }
        else
        {
            // target position is ok:
            // set the cursor position to be the target position
            cursor_pos = cursor_pos_target;
        }

        // check cursor position does not scroll off end of window
        int window_line_count{_size_y_ / c_h}; // the number of lines there is space for in the textbox window
        std::cout << "size_y=" << _size_y_ << " c_h=" << c_h << std::endl;
        std::cout << "window_line_count=" << window_line_count << std::endl;
        int delta_cursor_position{_cursor_->GetPosLine() - _scroll_index_};
        int window_cursor_position{0}; // starts at index 0, cursor index starts at 0
        // TODO: still confused by above ^, delta index starts from 1 but this var is not used
        // TODO: cursor index starts from zero!
        std::cout << "scroll index: " << _scroll_index_ << " GetPosLine: " << _cursor_->GetPosLine() << std::endl;
        // add 1 to cursor position in for loop because we want to test the location at which the cursor
        // will be IF it is to be moved
        // TODO but cursor->GetPosLine() index starts from 1?
        //for(std::size_t count_ix{_scroll_index_}; count_ix < _cursor_->GetPosLine() + 1; ++ count_ix)
        for(std::size_t count_ix{0}; count_ix < _cursor_->GetPosLine() + 1 - _scroll_index_; ++ count_ix)
        {
            std::cout << "count_ix=" << count_ix << std::endl;
            std::cout << "add " << wrap_count.at(count_ix) << " to ";
            window_cursor_position += wrap_count.at(count_ix);
            std::cout << "window_cursor_position -> " << window_cursor_position << std::endl;
        }
        if(window_cursor_position >= window_line_count)
        {
            std::cout << "scroll down, index=" << _cursor_->GetPosLine() - _scroll_index_ << " count=" << wrap_count.at(_cursor_->GetPosLine() - _scroll_index_) << std::endl;
            //if(_scroll_index_ < buffer_contents.size() - 1)
            if(_scroll_index_ < buffer_contents.size() - wrap_count.at(_cursor_->GetPosLine() - _scroll_index_))
            {
                _scroll_index_ += wrap_count.at(_cursor_->GetPosLine() - _scroll_index_);
            }
        }
        // TODO an if statement is needed here to make sure this doesn't overflow
        _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
      //  if(_cursor_->GetPosLine() + 1 - _scroll_index_ < the number of lines that can fit in a window, but they are wrapped!)
            // TODO: might be better to have a function that can translate cursor position into x,y screen position
            // would need an update function which tracks the number of wraps per line
      //  {
      //      _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
            // TODO: appears twice, move out of if statement
      //  }
      //  else
      //  {
      //      std::cout << "trying to scroll down" << std::endl;
      //      {
      //          ++ _scroll_index_;
      //      }
      //      _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
            // TODO: appears twice, move out of if statement
      //  }
    }
    else
    {
        std::cout << "cannot go down" << std::endl;
        // _line_ is the maximum line, cannot go down, do nothing

    }
}
#endif
void Textbox::CursorDown()
{

    // TODO: find some way of avoiding running this every time, cannot have
    // a modified flag in this class because the class it inherits from
    // does the modifying with Insert() etc - could overload these functions
    // however
    update_wrap_count();
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};


    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};
    

    std::cout << "cursor down" << std::endl;

    // check that cursor can go down, that it does not run past the end
    // of the buffer
    if(_cursor_->GetPosLine() < buffer_contents.size() - 1)
    {

        // get the target cursor position
        std::size_t line_size{buffer_contents.at(_cursor_->GetPosLine() + 1).size()};
        Cursor::CursorPos_t cursor_pos{_cursor_->GetPosCol()};
        Cursor::CursorPos_t cursor_pos_target{_cursor_->GetTargetCol()};
        if(cursor_pos_target > line_size)
        {
            // target position is too far along the line
            // as the line is too short!
            // check against the current cursor position
            // rather than the target cursor position
            // which is set whenever the user moves left / right
            if(cursor_pos > line_size)
            {
                cursor_pos = line_size;
            }
            else
            {
                // _cursor_pos_ has the correct value
                // don't do anything
            }
        }
        else
        {
            // target position is ok:
            // set the cursor position to be the target position
            cursor_pos = cursor_pos_target;
        }

        
        // TODO: move elsewhere?
        unsigned int line_number_width = get_line_number_width();
        const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?

        // check cursor position does not scroll off end of window
        //int cursor_screen_pos_y{_cursor_->GetPosLine() + 1 - _scroll_index_ - _scroll_sub_index_}; // weird but should work
        int cursor_screen_pos_y{0}; // count cursor screen position
        // count from scroll position (line index) to cursor line index
        for(std::size_t ix{_scroll_index_}; ix < _cursor_->GetPosLine(); ++ ix)
        {
            cursor_screen_pos_y += wrap_count.at(ix);
        }
        // subtract the sub scroll counter
        // as this part of a wrapped line is not drawn
        cursor_screen_pos_y -= _scroll_sub_index_;
        const int screen_line_count{_size_y_ / c_h};
        if(cursor_screen_pos_y < screen_line_count)
        {
            std::cout << "cursor_screen_pos_y < screen_line_count" << std::endl;
            // not at end of window, move cursor
            //_cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_->GetPosCol()
            std::cout << "wc: " << wrap_count.at(_cursor_->GetPosLine()) << " param: " << _cursor_->GetPosCol() << " " << line_width << std::endl;
            if(_cursor_->GetPosCol() / line_width < wrap_count.at(_cursor_->GetPosLine()) - 1)
            {
                std::cout << "sub scroll" << std::endl;
                // move cursor
                //unsigned int line_number_width = get_line_number_width();
                //const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
                _cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() + line_width); // no check required here
            }
            else
            {
                std::cout << "cannot sub scroll" << std::endl;
                // move cursor
                if(_cursor_->GetPosLine() + 1 < buffer_contents.size())
                {
                    //unsigned int line_number_width = get_line_number_width();
                    //const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
                    _cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_->GetPosCol() % line_width);
                }
            }
        }
        else
        {
            std::cout << "cursor at end of window" << std::endl;
            // cursor at end of window
            // try to scroll down
            if(_scroll_sub_index_ < wrap_count.at(_cursor_->GetPosLine()) - 1) // TODO: change _scroll_sub_index_ as above?
            {
                ++ _scroll_sub_index_;
                // move cursor
                //unsigned int line_number_width = get_line_number_width();
                //const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
                _cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() + line_width); // no check required here
            }
            else
            {
                if(_scroll_index_ < buffer_contents.size())
                {
                    _scroll_sub_index_ = 0;
                    ++ _scroll_index_;
                    // move cursor
                    // check if cursor can move
                    if(_cursor_->GetPosLine() + 1 < buffer_contents.size())
                    {
                        //unsigned int line_number_width = get_line_number_width();
                        //const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?
                        _cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_->GetPosCol() % line_width);
                    }
                }
                else
                {
                    std::cout << "HIT END OF BUFFER, NO SCROLL" << std::endl;
                }
            }
        }




#if 0
        int window_line_count{_size_y_ / c_h}; // the number of lines there is space for in the textbox window
        std::cout << "size_y=" << _size_y_ << " c_h=" << c_h << std::endl;
        std::cout << "window_line_count=" << window_line_count << std::endl;
        int delta_cursor_position{_cursor_->GetPosLine() - _scroll_index_};
        int window_cursor_position{0}; // starts at index 0, cursor index starts at 0
        // TODO: still confused by above ^, delta index starts from 1 but this var is not used
        // TODO: cursor index starts from zero!
        std::cout << "scroll index: " << _scroll_index_ << " GetPosLine: " << _cursor_->GetPosLine() << std::endl;
        // add 1 to cursor position in for loop because we want to test the location at which the cursor
        // will be IF it is to be moved
        // TODO but cursor->GetPosLine() index starts from 1?
        //for(std::size_t count_ix{_scroll_index_}; count_ix < _cursor_->GetPosLine() + 1; ++ count_ix)
        for(std::size_t count_ix{0}; count_ix < _cursor_->GetPosLine() + 1 - _scroll_index_; ++ count_ix)
        {
            std::cout << "count_ix=" << count_ix << std::endl;
            std::cout << "add " << wrap_count.at(count_ix) << " to ";
            window_cursor_position += wrap_count.at(count_ix);
            std::cout << "window_cursor_position -> " << window_cursor_position << std::endl;
        }
        if(window_cursor_position >= window_line_count)
        {
            std::cout << "scroll down, index=" << _cursor_->GetPosLine() - _scroll_index_ << " count=" << wrap_count.at(_cursor_->GetPosLine() - _scroll_index_) << std::endl;
            //if(_scroll_index_ < buffer_contents.size() - 1)
            if(_scroll_index_ < buffer_contents.size() - wrap_count.at(_cursor_->GetPosLine() - _scroll_index_))
            {
                _scroll_index_ += wrap_count.at(_cursor_->GetPosLine() - _scroll_index_);
            }
        }
        // TODO an if statement is needed here to make sure this doesn't overflow
        _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
      //  if(_cursor_->GetPosLine() + 1 - _scroll_index_ < the number of lines that can fit in a window, but they are wrapped!)
            // TODO: might be better to have a function that can translate cursor position into x,y screen position
            // would need an update function which tracks the number of wraps per line
      //  {
      //      _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
            // TODO: appears twice, move out of if statement
      //  }
      //  else
      //  {
      //      std::cout << "trying to scroll down" << std::endl;
      //      {
      //          ++ _scroll_index_;
      //      }
      //      _cursor_->SetPos(_cursor_->GetPosLine() + 1, cursor_pos);
            // TODO: appears twice, move out of if statement
      //  }
#endif

    }
    else
    {
        std::cout << "cannot go down" << std::endl;
        // _line_ is the maximum line, cannot go down, do nothing

    }
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
