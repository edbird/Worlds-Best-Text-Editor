#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "GUIObject.hpp"
#include "Buffer.hpp"
#include "FontTextureManager.hpp"
//#include "CharMatrix.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


#include <string>
#include <sstream>


class Textbox : public Buffer, public GUIObject
{

    public:
    
    Textbox(const Config& config, const FontTextureManager& ftm, const int width, const int height)
        : _config_{config}
        , _ftm_{ftm}
        //, _size_x_{width}
        //, _size_y_{height}
        //, _pos_x_{0} // TODO: test with/out line number
        //, _pos_y_{0}
        //, _line_text_first_line_{0}
        , _scroll_index_{0}
        , _scroll_sub_index_{0}
        , _line_number_enabled_{false}
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
       
        //_charmatrix_ptr_ = new CharMatrix(_pos_x_, _pos_y_, _size_x_, _size_y_, *_cursor_, _config_, _ftm_);
        
        
        ////////////////////////////////////////////////////////////////////////////
        // READ CONFIG
        ////////////////////////////////////////////////////////////////////////////

        //bool line_number_enabled{false};
        //int line_number_width{0};
        // Note: only 1 is true, any other integer is false
        if(_config_.GetInt("linenumber") == 1)
        {
            _line_number_enabled_ = true;
        }

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
    std::string line_number_to_string(const int line_number, const int line_number_width) const;

    // compute line number width
    unsigned int get_line_number_width() const;        

    // TODO: pass dst_rect by reference and modify within function
    // TODO: remove _texture_chars_ arguments
    void print_line_number(const int line_number, const int line_number_width, const unsigned int print_index_y, SDL_Renderer *const _renderer_) const;
    


    // print char
    // TODO: optimize this, flag might not be required
    //void print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
    //                        const SDL_Rect& src_rect, SDL_Rect& dst_rect) const;

    public:
    
    // TODO: the buffer object should know its own WIDTH and HEIGHT
    void Draw(SDL_Renderer *const renderer, const Uint32 timer) const;
    
    //{
        //const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    //    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        // compute line width
        // TODO: can be done inside CharMatrix
        //std::size_t line_width{(std::size_t)(_size_x_ / texture_chars_size.at(' ').w)};
        
        // create character matrix object
        // TODO: this should be a long life object
        //CharMatrix cm(/*line_width,*/ _buffer_, _pos_x_, _pos_y_, _size_x_, _size_y_, *_cursor_, _config_, _ftm_);
        //_charmatrix_ptr_->Update(_buffer_, renderer, _timer_);

        // set first line to draw from
        //_charmatrix_ptr_->SetFirstLine(_line_text_first_line_);

        //cm.Draw(renderer, _timer_);
        //_charmatrix_ptr_->Draw(renderer, _timer_);
        
    //}
    
    
    // get const buffer reference
    //const Buffer& GetBuffer() const
    //{
    //    return _buffer_;
    //}
    
    //Buffer& MutableBuffer()
    //{
    //    return _buffer_;
    //}
    

    void ResetCursor() // used when opening new file
    {
        _cursor_->SetPos(0, 0);
    }

    void ScrollDown();
    void ScrollUp();
    void ScrollDownSub();
    void ScrollUpSub();

    private:

    void update_wrap_count();

    ////////////////////////////////////////////////////////////////////////////
    // DATA MEMBERS
    ////////////////////////////////////////////////////////////////////////////


    // cursor
    std::unique_ptr<Cursor> _cursor_;
    //std::size_t _cursor_x_;
    //std::size_t _cursor_y_;

    
    
    const Config& _config_;
    const FontTextureManager& _ftm_;
    
    

    //CharMatrix *_charmatrix_ptr_;

    //Buffer _buffer_;

    // size of buffer on screen (for drawing)
    //int _size_x_;
    //int _size_y_;

    // position (for drawing)
    //int _pos_x_;
    //int _pos_y_;
    
    
    /// Drawing specific members ///
    
    // break lines (wrap)
    //std::vector<std::string> _matrix;
    
    //std::vector<int> _line_number_;
    
    //std::vector<unsigned int> _line_wrap_count_;
    
    

    // first line of text to print
    // first line to start printing from
    //std::size_t _line_text_first_line_;
    // Note: new system: this index counts the number of wrapped lines
    // a line wrapped 3 times is counted through 3 times
    std::size_t _scroll_index_;
    std::size_t _scroll_sub_index_;
    
    bool _line_number_enabled_;

    std::vector<unsigned int> wrap_count;

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








//void ResetCursorLastBlinkTime();















#endif
