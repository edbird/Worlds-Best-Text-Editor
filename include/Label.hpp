#ifndef LABEL_HPP
#define LABEL_HPP


#include "FontTextureManager.hpp"
#include "Functions.hpp"
#include "GUIObject.hpp"
#include "FunctionCallback.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


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
class Label : public GUIObject
{


    public:

    Label(const FontTextureManager& ftm)
        : GUIObject()
        , _ftm_{ftm}
        , _anchor_{LabelAnchor::TOP_LEFT}
    {
        set_size();
    }

    
    Label(const std::string& text, const FontTextureManager& ftm)
        : GUIObject()
        , _text_{text}
        , _ftm_{ftm}
        , _anchor_{LabelAnchor::TOP_LEFT}
    {
        set_size();
    }

    virtual ~Label()
    {
    }

    /*
    int Width() const
    {
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        //int c_h{texture_chars_size.at(' ').h};

        return c_w * _text_.size();
    }
    */


    // TODO: base class with width and height for drawable objects
    /*
    int Height() const
    {
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        //int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};

        return c_h;
    }
    */


    void SetText(const std::string& text)
    {
        _text_ = text;

        set_size();
    }

    std::string GetText() const
    {
        return _text_;
    }


    private:

    // call after doing anything to object that changes the size,
    // at the moment, only changing the text contents changes the size
    // so this function must be called after the contents of _text_ is changed
    void set_size()
    {
    
        // get reference to texture chars size and texture pointers
        const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{_ftm_.GetCharSize()};
        
        int c_w{texture_chars_size.at(' ').w};
        int c_h{texture_chars_size.at(' ').h};

        //GUIObject::_size_x_ = c_w * _text_.size();
        //GUIObject::_size_y_ = c_h;

        SetSize(c_w * _text_.size(), c_h);
    }

    public:


    ////////////////////////////////////////////////////////////////////////////
    // SET POSITION AND ANCHOR
    ////////////////////////////////////////////////////////////////////////////

    /*
    void SetPosition(const int pos_x, const int pos_y)
    {
        _pos_x_ = pos_x;
        _pos_y_ = pos_y;
    }
    */
    
    void SetAnchor(const LabelAnchor anchor)
    {
        _anchor_ = anchor;
    }

    virtual void ProcessEvent(Window& current_window, const SDL_Event& event, const Keyboard& keyboard, /*const CurrentKeyboardAction& ka_current,*/ Uint32 timer) override
    {
        // do nothing

        std::cout << "Label::ProcessEvent()" << std::endl;

        // TODO: make label blink ?
    }

    virtual void Draw(SDL_Renderer *const renderer, const Uint32 timer) const override
    {

        std::cout << "Label::Draw()" << std::endl;

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
        //int size_x{c_w * _text_.size()};
        //int size_y{c_h};

        // draw background
        SDL_Rect rect{PosX() + x_off, PosY() + y_off, Width(), Height()};
        SDL_SetRenderDrawColor(renderer, _background_color_r_, _background_color_g_, _background_color_b_, 0xFF);
        SDL_RenderFillRect(renderer, &rect);


        ////////////////////////////////////////////////////////////////////////////
        // DRAW BUFFER TEXT
        ////////////////////////////////////////////////////////////////////////////
        
        SDL_Rect dst_rect{PosX() + x_off, PosY() + y_off, c_w, c_h};
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
    //int _pos_x_;
    //int _pos_y_;

    const FontTextureManager& _ftm_;

    // anchor position
    LabelAnchor _anchor_;

};


#endif LABEL_HPP // LABEL_HPP
