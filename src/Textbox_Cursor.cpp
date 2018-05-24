#include "Textbox.hpp"


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
    const std::vector<std::string> &buffer_content{GetContainer()};
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};
    
    // check cursor can go up, not at top of buffer
    if(_cursor_->GetPosLine() > 0)
    {

        // set the cursor position using the target position and current position
        /*std::size_t line_size{buffer_content.at(_cursor_->GetPosLine() - 1).size()};
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
        }*/
        //_cursor_->SetPos(_cursor_->GetPosLine() - 1, cursor_pos);
        
        // get line_width
        unsigned int line_number_width = get_line_number_width();
        const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?

        // TODO: should not need these variables
        // get the cursor x and y position
//        int cursor_pos_x{cursor_pos % line_width};
//        int cursor_pos_y{_cursor_->GetPosLine() - _scroll_index_ + (cursor_pos / line_width) - _scroll_sub_index_};
//        std::cout << "cursor_pos_x=" << cursor_pos_x << " cursor_pos_y=" << cursor_pos_y << std::endl;

        // cursor wants to go up
//        if(cursor_pos_y < 1)
        //int cursor_pos_y{0};
        // check cursor position does not scroll off end of window
        int cursor_screen_pos_y{0}; // count cursor screen position
        // count from scroll position (line index) to cursor line index
        for(std::size_t ix{_scroll_index_}; ix < _cursor_->GetPosLine(); ++ ix)
        {
            cursor_screen_pos_y += wrap_count.at(ix);
        }
        cursor_screen_pos_y += _cursor_->GetPosCol() / line_width - _scroll_sub_index_;
        std::cout << "cursor_screen_pos_y=" << cursor_screen_pos_y << std::endl;
        if(cursor_screen_pos_y < 1)
        {
            // cursor_pos_y must currently be 0
            ScrollUp();
            // option: can choose either depending on desired behaviour
            //SubScrollUp();
        }


        // detect wrapped line
        //if(cursor_pos / line_width > 0)
        if(_cursor_->GetPosCol() / line_width > 0)
        {
            // cursor moves up analagously to sub scroll
            //std::cout << "CursorUp: A" << std::endl;
            //_cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() - line_width);
            if(_cursor_->GetPosCol() - line_width >= 0)
            {
                // this always happens
                _cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() - line_width);
            }
            else
            {
                // this can never happen
                std::cout << "this happened but it isn't supposed to" << std::endl;
                _cursor_->SetPos(_cursor_->GetPosLine(), buffer_content.at(_cursor_->GetPosLine()).size());
            }
        }
        else
        {
            // cursor moves up analagously to scroll
            //_cursor_->SetPos(_cursor_->GetPosLine() - 1, _cursor_->GetPosCol());
            // cursor moves up analagously to scroll
            // TODO: I added % line_width without thinking about it
            if(_cursor_->GetPosCol() % line_width \
                    + (wrap_count.at(_cursor_->GetPosLine() - 1) - 1) * line_width\
                    <= buffer_content.at(_cursor_->GetPosLine() - 1).size()) // TODO changed this to <= what about other function
            {
                std::cout << (wrap_count.at(_cursor_->GetPosLine() - 1) - 1) << " * " << line_width << std::endl;
                std::cout << _cursor_->GetPosCol() % line_width << std::endl;
                std::cout << _cursor_->GetPosCol() % line_width + (wrap_count.at(_cursor_->GetPosLine() - 1) - 1) * line_width << std::endl;
                std::cout << buffer_content.at(_cursor_->GetPosLine() - 1).size() << std::endl;
                //std::cout << "CursorUp: B" << std::endl;
                _cursor_->SetPos(_cursor_->GetPosLine() - 1, _cursor_->GetPosCol() % line_width + (wrap_count.at(_cursor_->GetPosLine() - 1) - 1) * line_width);
            }
            else
            {
                //std::cout << "CursorUp: C" << std::endl;
                _cursor_->SetPos(_cursor_->GetPosLine() - 1, buffer_content.at(_cursor_->GetPosLine() - 1).size());
            }
        }

    }
    else
    {
        // _line_ is 0, do nothing
        std::cout << "cannot go up" << std::endl;
    }
}

void Textbox::CursorDown()
{

    update_wrap_count();

    // const reference to data structure
    const std::vector<std::string> &buffer_content{GetContainer()};
    
    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // monospace character width and height
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};
    
    std::cout << buffer_content.size() - 1 << std::endl;
    std::cout << _cursor_->GetPosLine() << std::endl;

    // check cursor can go up, not at top of buffer
    if(_cursor_->GetPosLine() + 1 < buffer_content.size() - 1)
    {

        //std::cout << "aA" << std::endl;

        // set the cursor position using the target position and current position
        /*std::size_t line_size{buffer_content.at(_cursor_->GetPosLine() + 1).size()};
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
        }*/
        //_cursor_->SetPos(_cursor_->GetPosLine() - 1, cursor_pos);
        
        // get line_width
        unsigned int line_number_width = get_line_number_width();
        const std::size_t line_width{(std::size_t)(_size_x_ / c_w - line_number_width)}; // TODO remove std::size_t ?

        // TODO: should not need these variables
        // get the cursor x and y position
//        int cursor_pos_x{cursor_pos % line_width};
//        int cursor_pos_y{_cursor_->GetPosLine() - _scroll_index_ + (cursor_pos / line_width) - _scroll_sub_index_};
//        std::cout << "cursor_pos_x=" << cursor_pos_x << " cursor_pos_y=" << cursor_pos_y << std::endl;

        // cursor wants to go down
//        if(cursor_pos_y > _size_y_ / c_h - 2)
        //int cursor_pos_y{0};
        // check cursor position does not scroll off end of window
        int cursor_screen_pos_y{0}; // count cursor screen position
        // count from scroll position (line index) to cursor line index
        for(std::size_t ix{_scroll_index_}; ix < _cursor_->GetPosLine(); ++ ix)
        {
            cursor_screen_pos_y += wrap_count.at(ix);
        }
        cursor_screen_pos_y += _cursor_->GetPosCol() / line_width - _scroll_sub_index_;
        std::cout << "cursor_screen_pos_y=" << cursor_screen_pos_y << std::endl;
        if(cursor_screen_pos_y > _size_y_ / c_h - 2)
        // Note, one of the +1's above is for the fact that the cursor is
        // going to move down, unless ScrollDown is called in which case
        // 1 should be subtracted from cursor_screen_pos_y due to the scroll
        // motion
        {
            //std::cout << "aB" << std::endl;

            // cursor_pos_y must currently be _size_y_ / c_h - 1
            ScrollDown();
            // option: can choose either depending on desired behaviour
            //SubScrollDown();
        }

        //std::cout << cursor_pos << ", " << line_width << ", " << wrap_count.at(_cursor_->GetPosLine()) << std::endl;
        std::cout << _cursor_->GetPosCol() << ", " << line_width << ", " << wrap_count.at(_cursor_->GetPosLine()) << std::endl;
        //if(cursor_pos / line_width < wrap_count.at(_cursor_->GetPosLine()) - 1)
        if(_cursor_->GetPosCol() / line_width < wrap_count.at(_cursor_->GetPosLine()) - 1) 
        {
            //std::cout << "aC2" << std::endl;
            // there is room on the current line to go down at least 1 more sub
            // line

            std::cout << _cursor_->GetPosCol() << " " << line_width << " " << buffer_content.at(_cursor_->GetPosLine()).size() << std::endl;

            // cursor moves down analagously to sub scroll
            //_cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() + line_width);
            if(_cursor_->GetPosCol() + line_width < buffer_content.at(_cursor_->GetPosLine()).size())
            {
                // the cursor can be moved directly down 1 line, because the
                // amount of text extends beyond the current y position of the
                // cursor
                _cursor_->SetPos(_cursor_->GetPosLine(), _cursor_->GetPosCol() + line_width);
            }
            else
            {
                // the cursor cannot be moved directly down because the line
                // does not extend far enough along on the sub line below
                // where it is wrapped
                // not long enough text! so set position to be at end (maximum)
                _cursor_->SetPos(_cursor_->GetPosLine(), buffer_content.at(_cursor_->GetPosLine()).size());
            }

        }
        else
        {
            //std::cout << "PosCol: " << _cursor_->GetPosCol() << std::endl;
            //std::cout << "next line size: " << buffer_content.at(_cursor_->GetPosLine() + 1).size() << std::endl;

            // cursor moves down analagously to scroll
            if(_cursor_->GetPosCol() % line_width <= buffer_content.at(_cursor_->GetPosLine() + 1).size()) // changed to <=
            {
                //std::cout << "aD2" << std::endl;
                //std::cout << _cursor_->GetPosCol() % line_width << std::endl;
                //std::cout << line_width << std::endl;
                _cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_->GetPosCol() % line_width);
            }
            else
            {
                //std::cout << "aE2" << std::endl;
                _cursor_->SetPos(_cursor_->GetPosLine() + 1, buffer_content.at(_cursor_->GetPosLine() + 1).size());
            }
        }

    }
    else
    {
        // _line_ is the maximum line, do nothing
        std::cout << "cannot go down" << std::endl;
    }

#if 0

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
    const std::vector<std::string> &buffer_content{GetContainer()};
    

    std::cout << "cursor down" << std::endl;

    // check that cursor can go down, that it does not run past the end
    // of the buffer
    if(_cursor_->GetPosLine() < buffer_content.size() - 1)
    {

        // get the target cursor position
        std::size_t line_size{buffer_content.at(_cursor_->GetPosLine() + 1).size()};
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
                if(_cursor_->GetPosLine() + 1 < buffer_content.size())
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
                if(_scroll_index_ < buffer_content.size())
                {
                    _scroll_sub_index_ = 0;
                    ++ _scroll_index_;
                    // move cursor
                    // check if cursor can move
                    if(_cursor_->GetPosLine() + 1 < buffer_content.size())
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


#endif

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
            //if(_scroll_index_ < buffer_content.size() - 1)
            if(_scroll_index_ < buffer_content.size() - wrap_count.at(_cursor_->GetPosLine() - _scroll_index_))
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

    //}
    //else
    //{
    //    std::cout << "cannot go down" << std::endl;
    //    // _line_ is the maximum line, cannot go down, do nothing
    //
    //}
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
    const std::vector<std::string> &buffer_content{GetContainer()};
    

    std::cout << "cursor down" << std::endl;
    if(_cursor_->GetPosLine() < buffer_content.size() - 1)
    {
        std::cout << "first if" << std::endl;
        std::size_t line_size{buffer_content.at(_cursor_->GetPosLine() + 1).size()};
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
            //if(_scroll_index_ < buffer_content.size() - 1)
            if(_scroll_index_ < buffer_content.size() - wrap_count.at(_cursor_->GetPosLine() - _scroll_index_))
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

