#include "Textbox.hpp"

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