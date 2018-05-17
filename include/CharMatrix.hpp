#ifndef CHARMATRIX_HPP
#define CHARMATRIX_HPP


//#include "Label.hpp"
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

    // TODO: lifetime of this object is zero (only used to draw)
    // however config is read every time this class is constructed
    // change this so that lifetime is longer

    // TODO: bit weird that the size in pixels is specified and the line width is
    // also specified, should compute line width from size, and also draw "overhang"
    // in different color
    CharMatrix(/*const std::size_t line_width,*/// const Buffer& buffer,
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
        , _line_number_enabled_{false}
        , _line_number_width_{0}
    {
    

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


        // TODO: reorgnize this, constructor contains all LONG LIFE
        // processes, add Update() function which Updates() contents of
        // _matrix_ before drawing
        //init(line_width - _line_number_width_, buffer);
    }

    ~CharMatrix()
    {

    }

    //void Update(const Buffer& buffer, )
    //{
    //    
    //}
    
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

   
    public:


    void Update(const Buffer& buffer, SDL_Renderer *const renderer, const Uint32 timer)
    {

        std::string l1s("TRIG false");
        
        // get line count
        _buffer_line_count_ = buffer.GetLineCount();

        // compute line number width required
        _line_number_width_ = 0;
        if(_line_number_enabled_)
        {
            // set line number character count
            //int line_count{_buffer_.GetLineCount()};
            int line_count{_buffer_line_count_};
            for(;;)
            {
                ++ _line_number_width_;
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
        
        // compute line width (space for characters when line numbers are accounted for)
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        _line_width_ = (std::size_t)(_size_x_ / texture_chars_size.at(' ').w - _line_number_width_);

        // clear matrix contents
        _matrix_.clear();
        _matrix_.shrink_to_fit();
        _line_number_.clear();
        _line_number_.shrink_to_fit();
        _line_wrap_count_.clear();
        _line_wrap_count_.shrink_to_fit();
        
        // cursor tracking
        Cursor::CursorPos_t cursor_line{_cursor_.GetPosLine()};
        Cursor::CursorPos_t cursor_col{_cursor_.GetPosCol()};
        _cursor_x_ = 0;
        _cursor_y_ = 0;

        // x, y index for tracking char entry into _data_
        std::size_t ix{0};
        std::size_t iy{0};

        // const reference to data structure
        const std::vector<std::string> &line_text{buffer.GetContainer()};

        // substring start position
        std::size_t substr_pos{0};
        std::size_t substr_len{0};

        // iterate over all lines in buffer
        for(std::size_t line_ix{0}; line_ix < line_text.size(); ++ line_ix)
        {
        
            // set substring position to start
            // set length to zero
            substr_pos = 0;
            substr_len = 0;

            // add a new wrap count to the wrap count vector
            _line_wrap_count_.push_back(0);

            // used to trigger line number drawing
            // TODO: remove and use _line_wrap_count_ instead?
            bool first{true};
            for(;;)
            {

                // true if the remaining text in the line needs to be
                // wrapped again, in other words, this is the escape
                // condition of the infinite loop
                bool wrapped{false};

                // compute substr_len size
                if(line_text[line_ix].size() - substr_pos > _line_width_)
                {
                    // remaining contents in line too big to fix, set
                    // substring length to maximum size
                    substr_len = _line_width_;

                    wrapped = true;

                    // increment wrap count vector last element
                    ++ _line_wrap_count_.back();
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
                //std::cout << next_line << std::endl;
                _matrix_.emplace_back(next_line);
                _line_number_.emplace_back(first);
                first = false;
                

                //if(substr_len == line_width)
                if(wrapped == true)
                {
                    substr_pos += substr_len;
                //    //substr_len = 0; // TODO: remove?
                }
                else
                {
                    // finished this line
                    break;
                }

            }


            // set cursor x and y
            if(line_ix == cursor_line)
            {
                _cursor_y_ += cursor_col / _line_width_;
                _cursor_x_ += cursor_col % _line_width_;
            }
            else if(line_ix < cursor_line)
            {
                _cursor_y_ += (line_text[line_ix].size() - 1) / _line_width_ + 1;
            }
        }


        // print debug labels
        // note: removed this label, however label needs to be added to
        // the git tracker on my other computer
        //std::stringstream l1ss(l1s);
        //Label l1(l1ss.str(), _ftm_);
        //l1.SetPosition(350, 100);
        //l1.Draw(renderer, timer);


    }


    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Data
    ////////////////////////////////////////////////////////////////////////////

    std::vector<std::string> _matrix_;
    
    // Store bool to signal when the line is "first"
    // ie; a line number should be drawn
    std::vector<int> _line_number_;
    
    // Need to know: how many lines is each line?
    // How many lines is each line split over?
    std::vector<unsigned int> _line_wrap_count_;

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

    bool _line_number_enabled_;
    int _line_number_width_;

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
    int dst_rect_origin_x{_line_number_width_ * texture_chars_size.at(' ').w};

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
    if(_line_number_enabled_ == true)
    {
        // TODO: are these needed anymore?
        //if(check_space_y(dst_rect_line_number))
        //{
        print_line_number(line_number, _line_number_width_, dst_rect_line_number, renderer);
        ++ line_number;
        //dst_rect_line_number.y += dst_rect_line_number.h;
        //}
    }

    
        
    ////////////////////////////////////////////////////////////////////////////
    // DRAWING
    ////////////////////////////////////////////////////////////////////////////

    // Note: if a complete line cannot be drawn it is not drawn
    // Note: Actually I think lines which are split are partially drawn!
    // Note: Need to fix this behaviour
    // TODO

    // TODO what if line wrap count size is 0

    // initial check to see if characters can be drawn
    // break if drawing beyond the end of the textbox drawing area
    const int c_w{texture_chars_size.at(' ').w};
    const int c_h{texture_chars_size.at(' ').h};
    //const int space_for_num_chars{_size_x_ / c_w - _line_number_width_};
    const int this_line_num_lines{_line_wrap_count_.at(0)};
    if(c_h * (0 + this_line_num_lines + 1) >= _pos_y_ + _size_y_)
    {
        //break;
    }
    else
    {
    for(std::size_t line_ix{0}; line_ix < _matrix_.size(); ++ line_ix)
    {


        // check was here

        // print characters on this line
        for(std::size_t char_ix{0}; char_ix < _matrix_[line_ix].size(); ++ char_ix)
        {
        
            //const int c_w{texture_chars_size.at(' ').w};
            //const int c_h{texture_chars_size.at(' ').h};
        
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

        
        // check for each new line number to be printed
        // break if drawing beyond the end of the textbox drawing area
        //const int c_w{texture_chars_size.at(' ').w};
        //const int c_h{texture_chars_size.at(' ').h};
        //const int space_for_num_chars{_size_x_ / c_w - _line_number_width_};
        const int this_line_num_lines{_line_wrap_count_.at(line_ix)};
        if(c_h * ((line_ix + 1) + this_line_num_lines + 1) >= _pos_y_ + _size_y_)
        {
            break;
        }


        // TODO: this does not print line numbers correctly!
        // some lines are wrapped!
        // Might be able to migrate this code into the Update function
        // (change Update to draw and draw instead of storing in a container)
        if(_line_number_enabled_ && line_ix + 1 < _matrix_.size())
        {
            dst_rect_line_number.y += dst_rect_line_number.h;

            if(_line_number_[line_ix + 1] == true)
            {
                print_line_number(line_number, _line_number_width_, dst_rect_line_number, renderer);
                ++ line_number;
            }

        }
        
        
    }
    }

    
    ////////////////////////////////////////////////////////////////////////
    // Print cursor
    ////////////////////////////////////////////////////////////////////////
    //const int c_h{texture_chars_size.at(' ').h};
    //const int c_w{texture_chars_size.at(' ').w};
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
