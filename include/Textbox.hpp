#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "Textbox.hpp"
#include "CharMatrix.hpp"

class Textbox
{

    public:
    
    Textbox(const Config& config, const FontTextureManager& ftm)
        : _config_{config}
        , _ftm_{ftm}
        , _size_x_{300}
        , _size_y_{300}
        , _pos_x_{100} // TODO: test with/out line number
        , _pos_y_{0}
        , _line_text_first_line_{0}
    {
        
        //const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{ftm.GetCharSize()};
        
        int size_x{texture_chars_size.at(' ').w};
        int size_y{texture_chars_size.at(' ').h};
        _cursor_.reset(new Cursor(size_x, size_y, /*2 * size_x, 0,*/ config));
        
        // TODO: implement
        _cursor_->SetPos(0, 0);  // the cursor is always drawn in the location where the next
                                // character will be inserted: the buffer starts with zero
                                // size however the cursor will be drawn at position 0
                                // even though the buffer is not of size 1
        
    }
    
    
    
    // TODO: change to get cursor not subfunctions of cursor class
    //Cursor::CursorPos_t GetCursorLine() const;
    //Cursor::CursorPos_t GetCursorCol() const;

    const Cursor& GetCursor() const;

    // TODO: should the buffer be responsible for setting the cursor
    // position or should the cursor be responsible for setting its own
    // bounds ?
    void CursorLeft();

    void CursorRight();

    // TODO: remember target line position
    // TODO: config: set rememberlineposition
    void CursorUp();

    void CursorDown();

    void CursorCR();
    
    private:


    /*
    Cursor& MutableCursor()
    */



    // TODO: Do i still want the XAtCursor() functions?
    // Does the cursor "live" in this class? Are the cursor related functions
    // still relevant?

    public:

    void InsertAtCursor(const char ch);

    void ReturnAtCursor();

    void BackspaceAtCursor();


    //void ResetCursorLastBlinkTime();


    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Rendering helper functions
    ////////////////////////////////////////////////////////////////////////////
    
    // Convert line number to string
    std::string line_number_to_string(const int line_number, const int line_number_width);
        

    // TODO: pass dst_rect by reference and modify within function
    // TODO: remove _texture_chars_ arguments
    void print_line_number(const int line_number, const int line_number_width, SDL_Rect /*&*/dst_rect, SDL_Renderer *const _renderer_, const std::map<const char, SDL_Texture*>& _texture_chars_, const std::map<const char, SDL_Rect>& _texture_chars_size_);
    


    // print char
    // TODO: optimize this, flag might not be required
    void print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
                            const SDL_Rect& src_rect, SDL_Rect& dst_rect);

    public:
    
    // TODO: the buffer object should know its own WIDTH and HEIGHT
    void Draw(SDL_Renderer *const renderer, const Uint32 _timer_)
    {
        //const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        // compute line width
        // TODO: can be done inside CharMatrix
        //std::size_t line_width{(std::size_t)(_size_x_ / texture_chars_size.at(' ').w)};
        
        // create character matrix object
        CharMatrix cm(/*line_width,*/ _buffer_, _pos_x_, _pos_y_, _size_x_, _size_y_, *_cursor_, _config_, _ftm_);
        
        cm.Draw(renderer, _timer_);
        
        
    }
    
    
    // get const buffer reference
    const Buffer& GetBuffer() const
    {
        return _buffer_;
    }
    
    Buffer& MutableBuffer()
    {
        return _buffer_;
    }
    

    private:


    std::unique_ptr<Cursor> _cursor_;
    
    
    const Config& _config_;
    const FontTextureManager& _ftm_;

    //CharMatrix;

    Buffer _buffer_;

    // size of buffer on screen (for drawing)
    int _size_x_;
    int _size_y_;

    // position (for drawing)
    int _pos_x_;
    int _pos_y_;
    
    

    // first line of text to print
    std::size_t _line_text_first_line_;

};


// TODO: this is now implemented by Textbox class
class ActionBuffer : Buffer
{
    // TODO: the "actions" are implemented here
    // eg; left key press should move the _cursor_ inside of buffer
};


/*
Cursor& MutableCursor()
{
    return _cursor_;

}
*/


/*
Cursor::CursorPos_t Textbox::GetCursorLine() const
{
    return _cursor_->GetPosLine();
}

Cursor::CursorPos_t Textbox::GetCursorCol() const
{
    return _cursor_->GetPosCol();
}
*/


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
    const std::vector<std::string>& _line_text_{_buffer_.GetContainer()};

    // TODO DEBUG
    //if(_col_ < line_size)
    if(_cursor_->GetPosCol() < _line_text_.at(_cursor_->GetPosLine()).size())
        _cursor_->Right(); // TODO
    //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
}


// TODO: remember target line position
// TODO: config: set rememberlineposition
void Textbox::CursorUp()
{
    const std::vector<std::string>& _line_text_{_buffer_.GetContainer()};
    
    if(_cursor_->GetPosLine() > 0)
    {
        std::size_t _line_size_{_line_text_.at(_cursor_->GetPosLine() - 1).size()};
        Cursor::CursorPos_t _cursor_pos_{_cursor_->GetPosCol()};
        Cursor::CursorPos_t _cursor_pos_target_{_cursor_->GetTargetCol()};
        if(_cursor_pos_target_ > _line_size_)
        {
            // target position is too far along the line
            // as the line is too short!
            // check against the current cursor position
            // rather than the target cursor position
            // which is set whenever the user moves left / right
            if(_cursor_pos_ > _line_size_)
            {
                _cursor_pos_ = _line_size_;
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
            _cursor_pos_ = _cursor_pos_target_;
        }
        _cursor_->SetPos(_cursor_->GetPosLine() - 1, _cursor_pos_);
    }
    else
    {
        // _line_ is 0, do nothing
    }
    //_cursor_.Up();
}


void Textbox::CursorDown()
{
    const std::vector<std::string>& _line_text_{_buffer_.GetContainer()};
    
    std::cout << "cursor down" << std::endl;
    if(_cursor_->GetPosLine() < _line_text_.size() - 1)
    {
        std::cout << "first if" << std::endl;
        std::size_t _line_size_{_line_text_.at(_cursor_->GetPosLine() + 1).size()};
        Cursor::CursorPos_t _cursor_pos_{_cursor_->GetPosCol()};
        Cursor::CursorPos_t _cursor_pos_target_{_cursor_->GetTargetCol()};
        if(_cursor_pos_target_ > _line_size_)
        {
            // target position is too far along the line
            // as the line is too short!
            // check against the current cursor position
            // rather than the target cursor position
            // which is set whenever the user moves left / right
            if(_cursor_pos_ > _line_size_)
            {
                _cursor_pos_ = _line_size_;
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
            _cursor_pos_ = _cursor_pos_target_;
        }
        _cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_pos_);
    }
    else
    {
        std::cout << "cannot go down" << std::endl;
        // _line_ is the maximum line, cannot go down, do nothing
    }

    //if(_cursor_.GetPosLine() < _line_text_.size())
    //{
    //_cursor_.Down(_line_text_.size()); // TODO
    //}
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
    
    _buffer_.Insert(ch, c_line, c_col);

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
    
    _buffer_.InsertNewLine(c_line, c_col);
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
        
    // TODO: change to void if possible
    if(_buffer_.Delete(c_line, c_col) == true)
    {
        if(c_col > 0)
        {
            _cursor_->Left();
        }
        else
        {
            //_cursor_.Up(); // TODO: test this
            
            Cursor::CursorPos_t goto_line{_cursor_->GetPosLine() - 1};
            _cursor_->SetPos(goto_line, _buffer_.LineLength(goto_line));
        }
    }
    
    
}





//void ResetCursorLastBlinkTime();















#endif
