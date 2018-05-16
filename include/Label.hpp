#ifndef LABEL_HPP
#define LABEL_HPP


#include "SDL.h"


#include <string>
#include <sstream>


// label
// can be used for debugging
class Label
{


    public:

    Label(const FontTextureManager& ftm)
        : _ftm_{ftm}
    {
    }

    
    Label(const std::string& text, const FontTextureManager& ftm)
        : _text_{text}
        , _ftm_{ftm}
    {
    }


    void SetText(const std::string& text)
    {
        _text_ = text;
    }


    void SetPosition(const int pos_x, const int pos_y)
    {
        _pos_x_ = pos_x;
        _pos_y_ = pos_y;
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

        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};

        // compute size
        int size_x{c_w * _text_.size()};
        int size_y{c_h};

        // draw background
        SDL_Rect rect{_pos_x_, _pos_y_, size_x, size_y};
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &rect);


        ////////////////////////////////////////////////////////////////////////////
        // DRAW BUFFER TEXT
        ////////////////////////////////////////////////////////////////////////////
        
        SDL_Rect dst_rect{_pos_x_, _pos_y_, c_w, c_h};
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

};


#endif LABEL_HPP // LABEL_HPP
