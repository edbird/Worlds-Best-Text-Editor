#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP


#include "Functions.hpp"
#include "GUIObject.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


#include <string>


// TODO: base class common between label and statusbar
class Statusbar : public GUIObject
{


    public:

    Statusbar(const int width, const FontTextureManager& ftm)
        : GUIObject(width, ftm.GetCharSize().at(' ').h)
        , _ftm_{ftm}
        , _text_left_offset_{4}
        , _text_right_offset_{4}
        , _background_color_r_{_DEFAULT_BACKGROUND_COLOR_R_};
        , _background_color_g_{_DEFAULT_BACKGROUND_COLOR_G_};
        , _background_color_b_{_DEFAULT_BACKGROUND_COLOR_B_};
    {
    }

    StatusBar(const std::string& text, const int width, const FontTexutureManager& ftm)
        : GUIObject(width, ftm.GetCharSize().at(' ').h)
        , _text_{text}
        , _ftm_{ftm}
        , _text_left_offset_{4}
        , _text_right_offset_{4}
        , _background_color_r_{_DEFAULT_BACKGROUND_COLOR_R_};
        , _background_color_g_{_DEFAULT_BACKGROUND_COLOR_G_};
        , _background_color_b_{_DEFAULT_BACKGROUND_COLOR_B_};
    {
    }

    void SetText(const std::string& text)
    {
        _text_ = text;
    }

    void Draw(SDL_Renderer *const renderer, const Uint32 timer) const
    {
        
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};


        // draw background
        SDL_Rect rect{PosX(), PosY(), Width(), Height()};
        SDL_SetRenderDrawColor(renderer, _background_color_r_, _background_color_g_, _background_color_b_, 0xFF);
        SDL_RenderFillRect(renderer, &rect);


        ////////////////////////////////////////////////////////////////////////////
        // DRAW BUFFER TEXT
        ////////////////////////////////////////////////////////////////////////////
        
        SDL_Rect dst_rect{PosX() + _text_left_offset_, PosY(), c_w, c_h};
        SDL_Rect src_rect{0, 0, c_w, c_h};

        for(std::size_t char_ix{0}; char_ix < _text_.size(); ++ char_ix)
        {
            // character to print
            const char ch{_text_[char_ix]};
        
            // set the texture pointer
            SDL_Texture *texture{texture_chars.at(ch)};
            print_char_texture(renderer, texture, src_rect, dst_rect);
            
            // advance dst
            dst_rect.x += dst_rect.w;

            // do not print beyond limit of statusbar
            if(dst_rect.x + dst_rect.w >= PosX() + Width() - _text_right_offset_) break;
        }
    }


    void SetBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b)
    {
        _background_color_r_ = r;
        _backggound_colog_g_ = g;
        _backgbound_colob_b_ = b;
    }

    private:


    const FontTextureManger& _ftm_;

    std::string _text_;

    // used to create a "gap" at the left and right most sides of the statusbox
    int _text_left_offset_;
    int _text_right_offset_;

    // TODO: base class for this
    Uint8 _DEFAULT_BACKGROUND_COLOR_R_{0xCC};
    Uint8 _DEFAULT_BACKGROUND_COLOR_G_{0xCC};
    Uint8 _DEFAULT_BACKGROUND_COLOR_B_{0xCC};
    Uint8 _background_color_r_;
    Uint8 _background_color_g_;
    Uint8 _background_color_b_;

}


#endif // STATUSBAR_HPP
