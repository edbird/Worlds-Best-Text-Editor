#ifndef CHARMATRIX_HPP
#define CHARMATRIX_HPP


#include "Buffer.hpp"


#include <string>
#include <sstream>




/** CharMatrix
  *
  * Stores "matrix" of chars to be drawn to screen
  *
  * Used to organize text in a textbox
  *
  */

class CharMatrix
{

    public:

    // TODO: bit weird that the size in pixels is specified and the line width is
    // also specified, should compute line width from size, and also draw "overhang"
    // in different color
    CharMatrix(/*const std::size_t line_width,*/ const Buffer& buffer,
               const int pos_x, const int pos_y, const int size_x, const int size_y,
               const Cursor& cursor,
               const Config& config,
               const FontTextureManager& ftm)
        : _pos_x_{pos_x}
        , _pos_y_{pos_y}
        , _size_x_{size_x}
        , _size_y_{size_y}
        //, _line_width_{line_width}
        , _cursor_{cursor}
        , _config_{config}
        , _ftm_{ftm}
    {
    
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
        std::size_t line_width{(std::size_t)(_size_x_ / texture_chars_size.at(' ').w)};
        _line_width_ = line_width;
        
        init(line_width, buffer);
    }

    ~CharMatrix()
    {

    }
    
    void Draw(SDL_Renderer *const renderer,
              const Uint32 timer);


    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////


    // TODO: update function (call when text changed, and draw required)

    void print_char_texture(SDL_Renderer *const renderer,
                            SDL_Texture* const texture,
                            const SDL_Rect& src_rect,
                            SDL_Rect& dst_rect);

    std::string line_number_to_string(const int line_number,
                                      const int line_number_width);
    
    void print_line_number(const int line_number,
                           const int line_number_width,
                           SDL_Rect /*&*/dst_rect,
                           SDL_Renderer *const renderer);

    

    inline
    void init(const std::size_t line_width, const Buffer& buffer)
    {
    
        _buffer_line_count_ = buffer.GetLineCount();

        _matrix_.clear();
        _matrix_.shrink_to_fit();
        
        // TODO: this does not work if the line numbers are enabled!
        // cursor tracking
        Cursor::CursorPos_t cursor_line{_cursor_.GetPosLine()};
        Cursor::CursorPos_t cursor_col{_cursor_.GetPosCol()};
        Cursor::CursorPos_t current_line{0};
        Cursor::CursorPos_t current_col{0};
        _cursor_x_ = 0;
        _cursor_y_ = 0;

        std::cout << "cursor_col=" << cursor_col << std::endl;

        // x, y index for tracking char entry into _data_
        std::size_t ix{0};
        std::size_t iy{0};

        // const reference to data structure
        const std::vector<std::string> &line_text{buffer.GetContainer()};
        //std::size_t line_ix{0};
        //std::size_t char_ix{0};

        // substring start position
        std::size_t substr_pos{0};
        std::size_t substr_len{0};

        // iterate over all lines in buffer
        for(std::size_t line_ix{0}; line_ix < line_text.size(); ++ line_ix)
        {
            std::cout << "line_ix=" << line_ix << std::endl;
        
            // insert new blank string as we have started a copying new line
            //_matrix_.emplace_back(std::string());
            //std::string next_line;
            // set substring position to start
            // set length to zero
            substr_pos = 0;
            substr_len = 0;

            for(;;)
            {

                // compute substr_len size
                if(line_text[line_ix].size() - substr_pos > line_width)
                {
                    // remaining contents in line too big to fix, set
                    // substring length to maximum size
                    substr_len = line_width;
                }
                else
                {
                    // remaining contents in line will fit
                    substr_len = line_text[line_ix].size() - substr_pos;
                }

                // copy
                // TODO: remove variable next_line
                // optimize out
                std::string next_line{line_text[line_ix].substr(substr_pos, substr_len)};
                std::cout << next_line << std::endl;
                _matrix_.emplace_back(next_line);
                
                // cursor
                if(current_col + substr_len <= cursor_col)
                {
                    // set cursor x position
                    // only if the current line is the cursor line
                    if(current_line == cursor_line)
                    {
                        _cursor_x_ = cursor_col - current_col;
                    }
                }
                else
                {
                    // line was wrapped, increment cursor y position
                    ++ _cursor_y_;
                    std::cout << "INCR" << std::endl;
                }
                
                current_col += substr_len;
                

                if(substr_len == line_width)
                {
                    substr_pos += substr_len;
                    //substr_len = 0; // TODO: remove?
                }
                else
                {
                    // finished this line
                    break;
                }

            }
            
            if(line_ix + 1 < line_text.size())
            {
                // TODO: this always = line_ix
                current_col = 0;
                ++ current_line;
                ++ _cursor_y_;
            }

        }
        
        std::cout << "cursor_y=" << _cursor_y_ << std::endl;
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Data
    ////////////////////////////////////////////////////////////////////////////

    std::vector<std::string> _matrix_;
    //Cursor &_cursor_;
    std::size_t _cursor_x_;
    std::size_t _cursor_y_;
    std::size_t _buffer_line_count_;

    // drawing
    int _pos_x_;
    int _pos_y_;
    int _size_x_;
    int _size_y_;
    
    std::size_t _line_width_;
    
    const Config& _config_;
    const FontTextureManager& _ftm_;
    const Cursor& _cursor_;

};



////////////////////////////////////////////////////////////////////////////
// Rendering helper functions
////////////////////////////////////////////////////////////////////////////


// Convert line number to string
std::string CharMatrix::line_number_to_string(const int line_number, const int line_number_width)
{
    std::ostringstream ss;
    ss << std::setw(line_number_width) << std::setfill('0') << line_number;
    std::string line_number_str{ss.str()};
    return line_number_str;
}


// TODO: pass dst_rect by reference and modify within function
// TODO: remove _texture_chars_ arguments
void CharMatrix::print_line_number(const int line_number, const int line_number_width, SDL_Rect /*&*/dst_rect, SDL_Renderer *const renderer)
{

    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};

    // pass dst_rect by copy
    // dst_rect must be moved manually by calling function

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
void CharMatrix::print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
                                    const SDL_Rect& src_rect, SDL_Rect& dst_rect)
{

    // space for character already checked
    // do print
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

}



// TODO: the buffer object should know its own WIDTH and HEIGHT
void CharMatrix::Draw(SDL_Renderer *const renderer,
    const Uint32 timer
    )
{

    // get reference to texture chars size and texture pointers
    const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
    const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
    
    // Testing purposes
    int overhang{_size_x_ % (texture_chars_size.at(' ').w)};
    SDL_Rect rect1{_pos_x_, _pos_y_, _size_x_ - overhang, _size_y_};
    SDL_Rect rect2{_pos_x_ + _size_x_ - overhang, _pos_y_, overhang, _size_y_};
    SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
    SDL_RenderFillRect(renderer, &rect1);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect2);


    ////////////////////////////////////////////////////////////////////////////
    // DRAW BUFFER TEXT
    ////////////////////////////////////////////////////////////////////////////

    // TODO: remove pseudovariables
    //SDL_Renderer *_renderer_{renderer};
    //Buffer &_buffer_{*this};
    //const std::map<const char, SDL_Texture*>& _texture_chars_{texture_chars};
    //const std::map<const char, SDL_Rect>& _texture_chars_size_{texture_chars_size};


    ////////////////////////////////////////////////////////////////////////////
    // READ CONFIG
    ////////////////////////////////////////////////////////////////////////////

    bool line_number_enabled{false};
    int line_number_width{0};
    // Note: only 1 is true, any other integer is false
    if(_config_.GetInt("linenumber") == 1)
    {
        line_number_enabled = true;

        // set line number character count
        //int line_count{_buffer_.GetLineCount()};
        int line_count{_buffer_line_count_};
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


    ////////////////////////////////////////////////////////////////////////////
    // DRAWING INIT
    ////////////////////////////////////////////////////////////////////////////
    
    // Set rendering space and render to screen
    // size of individual characters
    // position set to origin of screen and character 'a' (first
    // character in the character string)
    //int dst_rect_origin_x{_pos_x_}; // origin x for text, leaving space for line numbers

    // Move dst_rect_origin_x if line numbers are enabled
    // TODO: if not enabled then:
    // is line_number_width == 0?
    // if true, then do not need if statement here, just exec line inside if
    // i have determined this to be true, test with line numbers disabled
    //if(line_number_enabled == true)
    //{
    //    dst_rect_origin_x += line_number_width * _texture_chars_size_.at(' ').w;
    //}
    int dst_rect_origin_x{line_number_width * texture_chars_size.at(' ').w};

    // current line number to print, starts from 1
    int line_number{1};
    
    // Initialize source rect for character drawing
    // this is always the same, unless the character width and height changes
    // all char textures start at 0,0
    SDL_Rect src_rect{0, 0, texture_chars_size.at(' ').w, texture_chars_size.at(' ').h};
    
    // Initialize destination rect for line number printing
    SDL_Rect dst_rect_line_number{_pos_x_, _pos_y_, texture_chars_size.at(' ').w, texture_chars_size.at(' ').h};

    // Initialize destination rect for character printing
    //SDL_Rect dst_rect{dst_rect_origin_x, _pos_y_, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};

    // Initialize cursor destination rect for cursor drawing
    //SDL_Rect cursor_texture_dst_rect{dst_rect_origin_x + _pos_x_, _pos_y_, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};

    // Print line number zero
    if(line_number_enabled == true)
    {
        // TODO: are these needed anymore?
        //if(check_space_y(dst_rect_line_number))
        //{
            print_line_number(line_number, line_number_width, dst_rect_line_number, renderer);
        //}
    }

    
        
    ////////////////////////////////////////////////////////////////////////////
    // DRAWING
    ////////////////////////////////////////////////////////////////////////////

    for(std::size_t line_ix{0}; line_ix < _matrix_.size(); ++ line_ix)
    {
    
        for(std::size_t char_ix{0}; char_ix < _matrix_[line_ix].size(); ++ char_ix)
        {
        
            const int c_h{texture_chars_size.at(' ').h};
            const int c_w{texture_chars_size.at(' ').w};
        
            const int x_off{c_w * char_ix};
            const int y_off{c_h * line_ix};
        
            // character dst rect
            SDL_Rect dst_rect{_pos_x_ + dst_rect_origin_x + x_off, _pos_y_ + y_off, c_w, c_h};
        
            // character to print
            const char ch{_matrix_[line_ix][char_ix]};
        
            // set the texture pointer
            SDL_Texture *texture{texture_chars.at(ch)};
            print_char_texture(renderer, texture, src_rect, dst_rect);
        }
        
        
    }

    
    ////////////////////////////////////////////////////////////////////////
    // Print cursor
    ////////////////////////////////////////////////////////////////////////
    const int c_h{texture_chars_size.at(' ').h};
    const int c_w{texture_chars_size.at(' ').w};
    const int x_off{c_w * _cursor_x_};
    const int y_off{c_h * _cursor_y_};
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
    _cursor_.Draw(renderer, cursor_texture_dst_rect, timer);
    // TODO: passing timer here is a bit weird, better to do Cursor.Update(timer) somewhere else
    // before this function call?
    
}


#endif
