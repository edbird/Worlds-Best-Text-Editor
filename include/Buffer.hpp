#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>


#include "Cursor.hpp"


#define DEBUG 1


// TODO: seperate size and drawing code to new class BufferWindow
class Buffer
{

    public:

    Buffer(const Config& config, const std::map<const char, SDL_Rect>& _texture_chars_size_);
    
    ~Buffer();

    std::size_t Size() const;

    // save to text file
    void Save(const std::string& filename) const;

    // this open method overwrites the buffer
    // and sets the _not_saved_ flag to FALSE
    void Open(const std::string& filename);

    // 
    bool NotSaved() const;

    // is the buffer in the "default" state - contains nothing, empty
    bool IsEmpty() const;

    // get reference 
    const std::string& Get() const;

    // get number of lines in buffer (line count)
    int GetLineCount() const;

    /*
    Cursor& MutableCursor()
    */


    // TODO: change to get cursor not subfunctions of cursor class
    Cursor::CursorPos_t GetCursorLine() const;

    Cursor::CursorPos_t GetCursorCol() const;

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

    void InsertAtCursor(const char ch);

    void ReturnAtCursor();

    bool BackspaceAtCursor();


    //void ResetCursorLastBlinkTime();


    private:

    bool cursor_check_space_x(const SDL_Rect& cursor_texture_dst_rect);
    bool cursor_check_space_y(const SDL_Rect& cursor_texture_dst_rect);

    // check first line can be drawn (enough space in y direction)
    bool check_space_y(const SDL_Rect& dst_rect);
    bool check_space_x(const SDL_Rect& dst_rect);

    // TODO: remove STEP as STEP = WIDTH/HEIGHT

    void advance_cursor_char(SDL_Rect& cursor_texture_dst_rect,
                             const int cursor_texutre_origin_x,
                             const Cursor::CursorPos_t cursor_line, const Cursor::CursorPos_t cursor_col,
                             Cursor::CursorPos_t& current_line, Cursor::CursorPos_t& current_col);
    
    void advance_cursor_line(SDL_Rect& cursor_texture_dst_rect,
                             const int cursor_texutre_origin_x,
                             const Cursor::CursorPos_t cursor_line, const Cursor::CursorPos_t cursor_col,
                             Cursor::CursorPos_t& current_line, Cursor::CursorPos_t& current_col);

    void advance_cursor_line_wrap(SDL_Rect& cursor_texture_dst_rect,
                                  const int cursor_texutre_origin_x,
                                  const Cursor::CursorPos_t cursor_line, const Cursor::CursorPos_t cursor_col,
                                  Cursor::CursorPos_t& current_line, Cursor::CursorPos_t& current_col);

    void advance_dst_rect_char(SDL_Rect& dst_rect);

    void advance_dst_rect_line(SDL_Rect& dst_rect, const int dst_rect_origin_x);


    // print char
    // TODO: optimize this, flag might not be required
    void print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
                            const SDL_Rect& src_rect, SDL_Rect& dst_rect);

    
    public:
    // TODO: the buffer object should know its own WIDTH and HEIGHT
    void Draw(SDL_Renderer *const renderer, const std::map<const char, SDL_Texture*>& texture_chars, const std::map<const char, SDL_Rect>& texture_chars_size,  const Uint32 _timer_);

    private:
    
    ////////////////////////////////////////////////////////////////////////////
    // Rendering helper functions
    ////////////////////////////////////////////////////////////////////////////
    
    // Convert line number to string
    std::string line_number_to_string(const int line_number, const int line_number_width);
        

    // TODO: pass dst_rect by reference and modify within function
    // TODO: remove _texture_chars_ arguments
    void print_line_number(const int line_number, const int line_number_width, SDL_Rect /*&*/dst_rect, SDL_Renderer *const _renderer_, const std::map<const char, SDL_Texture*>& _texture_chars_, const std::map<const char, SDL_Rect>& _texture_chars_size_);
    
    ////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////

    // compile lines into complete buffer object inside std::string
    void create_data() const;

    // reference to config
    const Config& _config_;

    // size of buffer on screen
    int _size_x_;
    int _size_y_;

    // position
    int _pos_x_;
    int _pos_y_;

    // TODO smart pointer
    std::unique_ptr<Cursor> _cursor_;

    // TODO: don't need this variable, or the one below
    //Cursor::CursorPos_t _line_count_; // not maintained yet
    //std::vector<Cursor::CursorPos_t> _line_size_; // not maintained yet
    //std::vector<BufferLine> _line_;
    std::vector<std::string> _line_text_;

    // first line of text to print
    std::size_t _line_text_first_line_;

    // "raw buffer" data emulation
    mutable std::string _data_;

    // if buffer is modified, the create_data() function must be called
    // before Get()
    mutable bool _modified_;

    // not saved flag
    // this flag is set to true when the buffer is modified but not saved
    // when the buffer is saved, this flag is set to false
    mutable bool _not_saved_;

    // const
    const std::string _new_line_string_{std::string("\n")};

};


class ActionBuffer : Buffer
{
    // TODO: the "actions" are implemented here
    // eg; left key press should move the _cursor_ inside of buffer
};


#endif // BUFFER_HPP
