#ifndef LABEL_HPP
#define LABEL_HPP


#include "SDL.h"


#include <string>
#include <sstream>



enum class LabelAnchor
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT,
    CENTER
};


// label
// can be used for debugging
class Label
{


    public:

    Label(const FontTextureManager& ftm)
        : _ftm_{ftm}
        , _anchor_{LabelAnchor::TOP_LEFT}
    {
    }

    
    Label(const std::string& text, const FontTextureManager& ftm)
        : _text_{text}
        , _ftm_{ftm}
        , _anchor_{LabelAnchor::TOP_LEFT}
    {
    }


    int Width() const
    {
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        //int c_h{texture_chars_size.at(' ').h};

        return c_w * _text_.size();
    }


    // TODO: base class with width and height for drawable objects
    int Height() const
    {
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        //int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};

        return c_h;
    }


    void SetText(const std::string& text)
    {
        _text_ = text;
    }


    ////////////////////////////////////////////////////////////////////////////
    // SET POSITION AND ANCHOR
    ////////////////////////////////////////////////////////////////////////////

    void SetPosition(const int pos_x, const int pos_y)
    {
        _pos_x_ = pos_x;
        _pos_y_ = pos_y;
    }
    
    void SetAnchor(const LabelAnchor anchor)
    {
        _anchor_ = anchor;
    }



    // TODO: duplicated from CharMatrix
    // print char
    // TODO: optimize this, flag might not be required
    // TODO: better if _pos_x_ and _pos_y_ were added here rather than adding them included with the dst_rect
    // do this by passing by value
    void print_char_texture(SDL_Renderer *const renderer, SDL_Texture* const texture,
                                        const SDL_Rect& src_rect, SDL_Rect& dst_rect) const
    {

        // space for character already checked
        // do print
        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

    }


    void Draw(SDL_Renderer *const renderer, const Uint32 timer) const
    {

        // anchor
        int x_off{0};
        int y_off{0};

        if(_anchor_ == LabelAnchor::TOP || _anchor_ == LabelAnchor::BOTTOM || _anchor_ == LabelAnchor::CENTER)
        {
            x_off -= Width() / 2;
        }

        if(_anchor_ == LabelAnchor::RIGHT || _anchor_ == LabelAnchor::TOP_RIGHT || _anchor_ == LabelAnchor::BOTTOM_RIGHT)
        {
            x_off -= Width();
        }

        if(_anchor_ == LabelAnchor::LEFT || _anchor_ == LabelAnchor::RIGHT || _anchor_ == LabelAnchor::CENTER)
        {
            y_off -= Height() / 2;
        }

        if(_anchor_ == LabelAnchor::BOTTOM || _anchor_ == LabelAnchor::BOTTOM_LEFT || _anchor_ == LabelAnchor::BOTTOM_RIGHT)
        {
            y_off -= Height();
        }



        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};

        // compute size
        int size_x{c_w * _text_.size()};
        int size_y{c_h};

        // draw background
        SDL_Rect rect{_pos_x_ + x_off, _pos_y_ + y_off, size_x, size_y};
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &rect);


        ////////////////////////////////////////////////////////////////////////////
        // DRAW BUFFER TEXT
        ////////////////////////////////////////////////////////////////////////////
        
        SDL_Rect dst_rect{_pos_x_ + x_off, _pos_y_ + y_off, c_w, c_h};
        SDL_Rect src_rect{0, 0, c_w, c_h};

        for(std::size_t char_ix{0}; char_ix < _text_.size(); ++ char_ix)
        {
            // character to print
            const char ch{_text_[char_ix]};
        
            // set the texture pointer
            SDL_Texture *texture{texture_chars.at(ch)};
            print_char_texture(renderer, texture, src_rect, dst_rect);
            
            // advance dst
            // TODO: can print off screen
            dst_rect.x += dst_rect.w;
        }
    
    }




    private:

    std::string _text_;
    int _pos_x_;
    int _pos_y_;

    const FontTextureManager& _ftm_;

    // anchor position
    LabelAnchor _anchor_;

};


#endif LABEL_HPP // LABEL_HPP
