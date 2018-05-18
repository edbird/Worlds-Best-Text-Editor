#ifndef CHARMATRIX_HPP
#define CHARMATRIX_HPP


//#include "Label.hpp"
#include "Buffer.hpp"
#include "FontTextureManager.hpp"


#include <SDL_ttf.h>


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
        : _first_line_index_{0}
        , _pos_x_{pos_x}
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
    
    void SetFirstLine(const std::size_t first_line)
    {
        _first_line_index_ = first_line;
    }
    
    void Draw(SDL_Renderer *const renderer, const Uint32 timer);

    void Update(const Buffer& buffer, SDL_Renderer *const renderer, const Uint32 timer);


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


    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Data
    ////////////////////////////////////////////////////////////////////////////

    std::vector<std::string> _matrix_;
    std::vector<int> _line_number_;
    //Cursor &_cursor_;
    std::size_t _cursor_x_;
    std::size_t _cursor_y_;
    std::size_t _buffer_line_count_;

    // first line to draw from
    std::size_t _first_line_index_;

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



#endif
