#ifndef CURSOR_HPP
#define CURSOR_HPP


#include "Config.hpp"

#include <SDL.h>

#include <iostream>


enum class CursorType
{
    BLOCK,
    IBEAM,
    UNDERSCORE
};


class Cursor
{
    
    public:

    typedef uint32_t CursorPos_t;


    Cursor(const int size_x, const int size_y, /*const int buffer_origin_x, const int buffer_origin_y,*/ const Config& config)
        : _config_{config}
        , _line_{0}
        , _col_{0}
        , _remember_col_{0}
        , _cursor_size_x_{size_x}
        , _cursor_size_y_{size_y}
        //, _cursor_origin_x_{buffer_origin_x}
        //, _cursor_origin_y_{buffer_origin_y}
        , _cursor_type_{CursorType::BLOCK}
        , _last_blink_time_{0} // TODO
        , _blink_delay_time_{500} // default
    {
        // TODO: remove "has key" method - don't need it since a default value
        // is always given
        //if(_config_.HasKey("cursorblinkrate"))
        {
            _blink_delay_time_ = _config_.GetInt("cursorblinkdelay"); 
        }
        
        //init_cursor();
    }
    
    /*
    ~Cursor()
    {
        // free cursor resources
        std::vector<SDL_Texture*>::iterator it{_cursor_texture_.begin()};
        for(; it != _cursor_texture_.end(); ++ it)
        {
            if(*it != nullptr)
            {
                SDL_DestroyTexture(*it);
            }
            
            *it = nullptr;
        }
        _cursor_texture_.clear();
    }
    */

    private:

    void set_pos(const CursorPos_t line, const CursorPos_t col)
    {
        _line_ = line;
        _col_ = col;
        // TODO: added 2018-05-20
        _remember_col_ = col;
        Print();
    }

    public:

    // TODO: what if an invalid value is given?
    void SetPos(const CursorPos_t line, const CursorPos_t col)
    {
        set_pos(line, col);
    }

    void SetPos(const CursorPos_t line, const CursorPos_t col, const Uint32 timer, const bool reset_blink_time = true)
    {
        set_pos(line, col);

        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    void ResetBlinkTime(const Uint32 timer)
    {
        _last_blink_time_ = timer;
    }

    CursorPos_t GetPosLine() const
    {
        return _line_;
    }

    CursorPos_t GetPosCol() const
    {
        return _col_;
    }

    private:

    void cr()
    {
        _col_ = 0;
    }

    void up()
    {
        //if(_line_ > 0)
        -- _line_;
        Print();
    }

    void down()
    {
        // TODO
        //if(_line_ < line_count - 1)
        ++ _line_;
        Print();
    }

    void left()
    {
        //if(_col_ > 0)
        -- _col_;
        _remember_col_ = _col_;
        Print();
    }

    void right()
    {
        // TODO
        //if(_col_ < line_size) // cursor can scroll 1 char past the end!
        ++ _col_;
        _remember_col_ = _col_;
        Print();
    }

    public:

    void CR()
    {
        cr();
    }

    void CR(const Uint32 timer, const bool reset_blink_time = true)
    {
        cr();

        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    // TODO: removing all checks on cursor here!
    // TODO: implement checks in buffer / window
    void Up()
    {
        up();
    }

    void Down()
    {
        down();
    }

    void Left()
    {
        left();
    }

    void Right()
    {
        right();
    }
    
    void Up(const Uint32 timer, const bool reset_blink_time = true)
    {
        up();

        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    void Down(const Uint32 timer, const bool reset_blink_time = true)
    {
        down();
        
        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    void Left(const Uint32 timer, const bool reset_blink_time = true)
    {
        left();
        
        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    void Right(const Uint32 timer, const bool reset_blink_time = true)
    {
        right();
        
        if(reset_blink_time == true)
        {
            ResetBlinkTime(timer);
        }
    }

    void Print() const
    {
        std::cout << _line_ << ',' << _col_ << std::endl;
    }

    CursorPos_t GetTargetCol() const
    {
        return _remember_col_;
    }

    CursorType GetCursorType() const
    {
        return _cursor_type_;
    }
    

    // reset the last blink time when the cursor moves
    void ResetLastBlinkTime(const Uint32 timer)
    {
        _last_blink_time_ = timer;
    }

    // NOTE
    // TODO
    // can pass in dimensions of cursor in the cursor_pos argument rather than setting
    // the cursor size in the constructor?
    void Draw(SDL_Renderer *renderer, const SDL_Rect& cursor_pos, const Uint32 _timer_) const
    {
        // TODO:
        // different blink modes
        // this blink mode draws a cursor and then draws nothing, but it is reset
        // when the cursor is moved to prevent the user not being able to see 
        // where the cursor is
        // another mode is to change the color of the cursor, which maybe should
        // change color as the cursor moves, or maybe the timer should be
        // reset when the cursor moves as in the below implemented example


        // Process the timing information
        if((_timer_ - _last_blink_time_) / _blink_delay_time_ == 1)
        {
            // don't draw just return
            return;
        }
        // NOTE: this is essentially 
        // if (_timer - _last_blink_time_) / _blink_delay_time_ == 2
        // but use > 1 incase there is a long delay which causes the value
        // 2 to be skipped!
        else if((_timer_ - _last_blink_time_) / _blink_delay_time_ > 1)
        {
            //reset the last blink time
            _last_blink_time_ = _timer_;
            // and now print the cursor
        }

        // Draw the cursor
        // TODO: color
        //SDL_Color BLACK{0x00, 0x00, 0x00, 0xFF}; // TODO: get from config
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        if(_cursor_type_ == CursorType::BLOCK)
        {
            //int x0{_cursor_origin_x_ + _cursor_size_x_ * (int)_col_};
            //int y0{_cursor_origin_y_ + _cursor_size_y_ * (int)_line_};
            int x0{cursor_pos.x};
            int y0{cursor_pos.y};
            //SDL_FillRect(renderer, x0, y0, x0 + _cursor_size_x_, y0 + _cursor_size_y_);
            SDL_Rect rect{x0, y0, _cursor_size_x_, _cursor_size_y_};
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(_cursor_type_ == CursorType::IBEAM)
        {
            //int x0{_cursor_origin_x_ + _cursor_size_x_ * (int)_col_};
            //int y0{_cursor_origin_y_ + _cursor_size_y_ * (int)_line_};
            int x0{cursor_pos.x};
            int y0{cursor_pos.y};
            SDL_RenderDrawLine(renderer, x0 + _cursor_size_x_, y0, x0 + _cursor_size_x_, y0 + _cursor_size_y_);
        }
        else if(_cursor_type_ == CursorType::UNDERSCORE)
        {
            //int x0{_cursor_origin_x_ + _cursor_size_x_ * (int)_col_};
            //int y0{_cursor_origin_y_ + _cursor_size_y_ * (int)_line_};
            int x0{cursor_pos.x};
            int y0{cursor_pos.y};
            SDL_RenderDrawLine(renderer, x0, y0 + _cursor_size_y_, x0 + _cursor_size_x_, y0 + _cursor_size_y_);
        }
        else
        {
            std::cerr << "Unrecognized cursor type" << std::endl;
        }

    }
    
    ////////////////////////////////////////////////////////////////////////////
    // Cursor init, like constructor call
    ////////////////////////////////////////////////////////////////////////////
    
    // TODO: move to constructor
    // Notes:
    // this function requires access to the _font_ object and the _texture_chars_size_ object
    // but only because this is poorly written code
    // TODO: change to not using fonts to initialize the cursor, but use instead an X, Y size of
    // cursor
    /*
    private:
    void init_cursor()
    {
        // init cursor
        //219
        //const char cursor_string_normal[2]{(char), 0};
        // Font characters for cursor types
        //const char cursor_string_normal[2]{' ', 0};
        //const char cursor_string_insert[2]{'|', 0};
        //const char cursor_string_replace[2]{'_', 0}; // TODO: working here

        //SDL_Surface* cursor_surface_normal = TTF_RenderText_Solid(_font_, cursor_string_normal, COLOR_CURSOR);
        //SDL_FillRect(cursor_surface_normal,
        //Uint32 width{_texture_chars_size_[' '].w};
        //Uint32 height{_texture_chars_size_[' '].h};
        //SDL_Surface* cursor_surface_normal = \
        //    SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        
        // Create cursor surface (blank rect)
        //SDL_Surface* cursor_surface_normal = SDL_CreateRGBSurface(0, _texture_chars_size_[' '].w, _texture_chars_size_[' '].h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        //if(cursor_surface_normal == nullptr)
        //{
        //    std::cout << SDL_GetError() << std::endl;
        //}
        
        // Create cursor surface (blank rect)
        //SDL_Surface* cursor_surface_normal_1 = SDL_CreateRGBSurface(0, _texture_chars_size_[' '].w, _texture_chars_size_[' '].h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        //if(cursor_surface_normal_1 == nullptr)
        //{
        //    std::cout << SDL_GetError() << std::endl;
        //}
        
        // Create cursor surfaces (blank rect)
        //SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0xFF, 0x00, 0xFF));
        //SDL_FillRect(cursor_surface_normal_1, nullptr, SDL_MapRGBA(cursor_surface_normal_1->format, 0x00, 0x80, 0xFF, 0xFF));

        //const SDL_Color COLOR_TEXT_DEFAULT{_color_palette_.Get("black")};

        // Create cursor surfaces from font
        //SDL_Surface* cursor_surface_insert = TTF_RenderText_Solid(_font_, cursor_string_insert, COLOR_TEXT_DEFAULT);
        //SDL_Surface* cursor_surface_replace = TTF_RenderText_Solid(_font_, cursor_string_replace, COLOR_TEXT_DEFAULT);

        // Create cursor textures from surfaces
        //SDL_Texture* cursor_texture_normal = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_normal);
        //SDL_Texture* cursor_texture_normal_1 = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_normal_1);
        //SDL_Texture* cursor_texture_insert = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_insert);
        //SDL_Texture* cursor_texture_replace = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_replace);

        // Free cursor surfaces
        //SDL_FreeSurface(cursor_surface_normal);
        //SDL_FreeSurface(cursor_surface_normal_1);
        //SDL_FreeSurface(cursor_surface_insert);
        //SDL_FreeSurface(cursor_surface_replace);
        
        // Add texture pointers to cursor texture pointer vector
        //_cursor_texture_.push_back(cursor_texture_normal);
        //_cursor_texture_.push_back(cursor_texture_insert);
        //_cursor_texture_.push_back(cursor_texture_replace);
        //_cursor_texture_.push_back(cursor_texture_normal_1);
        //cursor_texture.push_back(cursor_texture_)
        
        // Set current cursor properties
        //_current_cursor_ = 0;
        //_cursor_texture_src_rect_.x = 0;
        //_cursor_texture_src_rect_.y = 0;
        //_cursor_texture_src_rect_.w = cursor_surface_normal->w;
        //_cursor_texture_src_rect_.h = cursor_surface_normal->h;
    }
    */

    private:
    
    // reference to config
    const Config& _config_;

    CursorPos_t _line_;
    CursorPos_t _col_;
    
    // remembers the "target" cursor pos col
    CursorPos_t _remember_col_;
    
    // size of cursor
    int _cursor_size_x_;
    int _cursor_size_y_;
    
    // TODO: currently do not have an origin coordinate, so if buffer origin changes
    // cursor is not drawn in correct place
    //int _cursor_origin_x_; // 2018-04-15 removed
    //int _cursor_origin_y_;

    
    // textures for cursor drawing
    //std::vector<SDL_Texture*> _cursor_texture_;
    //SDL_Rect _cursor_texture_src_rect_;
    // cursor type (drawing flag)
    //int _cursor_type_;
    //std::size_t _current_cursor_;
    CursorType _cursor_type_;
    // changed from using textures to represent cursor, and copying these using
    // renderer to drawing cursor directly to screen using line drawing functions
    
    // TODO: color!

    mutable Uint32 _last_blink_time_;
    Uint32 _blink_delay_time_;

};

#endif // CURSOR_HPP
