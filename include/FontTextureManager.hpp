#ifndef FONTTEXTUREMANAGER_HPP
#define FONTTEXTUREMANAGER_HPP


#include "ColorPalette.hpp"


#include <SDL_ttf.h>



// store things such as the texture chars
class FontTextureManager
{


    public:

    FontTextureManager(SDL_Renderer * const _renderer_, const TTF_Font * const font, const ColorPalette& color_palette)
        //: _font_{nullptr}
        //: _color_palette_{color_palette}
    {
        init(_renderer_, font, color_palette);
    }


    void init(SDL_Renderer * const _renderer_, const TTF_Font * const font, const ColorPalette& color_palette)
    {
        // Create / render text
        
        // init printable characters
        for(char ch{' '}; ; )
        {
    
            // render text surface
            const char ch_string[2]{ch, '\0'};
            
            // render text
            SDL_Color COLOR_TEXT_DEFAULT = color_palette.Get("black");
            SDL_Surface* text_surface = TTF_RenderText_Solid((TTF_Font*)font, ch_string /*_texture_chars_.c_str()*/, COLOR_TEXT_DEFAULT);
            
            if(text_surface == nullptr)
            {
                std::cout << TTF_GetError() << std::endl;
            }
            else
            {
                // create texture from surface pixels
                SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer_, text_surface);
                if(texture == nullptr)
                {
                    std::cout << SDL_GetError() << std::endl;
                }
                else
                {
                    //Get image dimensions
                    //_texture_width_ = _text_surface_->w;
                    //_texture_height_ = _text_surface_->h;
                    
                    _texture_chars_[ch] = texture;
                    _texture_chars_size_[ch].w = text_surface->w;
                    _texture_chars_size_[ch].h = text_surface->h;
                }

                //Get rid of old surface
                SDL_FreeSurface(text_surface);
            } 
    
            if(ch == '~') break;
            ++ ch;
        }
    }
    

    /*
    void init_old_version()
    {
        //Open the font
        //_font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", 11);
        _font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", _config_.GetInt("fontsize"));
        if(_font_ == nullptr)
        {
            std::cout << TTF_GetError() << std::endl;
        }
        else
        {
            // Create / render text
        
            // init printable characters
            for(char ch{' '}; ; )
            {
        
                // render text surface
                const char ch_string[2]{ch, '\0'};
                //SDL_Surface* text_surface = TTF_RenderText_Solid(_font_, _texture_chars_.c_str(), COLOR_TEXT_DEFAULT);
                SDL_Surface* text_surface = TTF_RenderText_Solid(_font_, ch_string, COLOR_TEXT_DEFAULT);
                if(text_surface == nullptr)
                {
                    std::cout << TTF_GetError() << std::endl;
                }
                else
                {
                    // create texture from surface pixels
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer_, text_surface);
                    if(texture == nullptr)
                    {
                        std::cout << SDL_GetError() << std::endl;
                    }
                    else
                    {
                        _texture_chars_[ch] = texture;
                        //Get image dimensions
                        _texture_chars_size_[ch].w = text_surface->w;
                        _texture_chars_size_[ch].h = text_surface->h;
                    }

                    //Get rid of old surface
                    SDL_FreeSurface(text_surface);
                } 
        
                if(ch == '~') break;
                ++ ch;
            }
        }
    }
    */

    ~FontTextureManager()
    {
        
        // Delete cursor textures
        std::map<const char, SDL_Texture*>::iterator it{_texture_chars_.begin()};
        for(; it != _texture_chars_.end(); ++ it)
        {
            SDL_Texture *texture{it->second};

            // TODO this check elsewhere
            if(texture != nullptr)
            {
                SDL_DestroyTexture(texture);
                //_texture_ = nullptr;
                //_texture_width_ = 0;
                //_texture_height_ = 0;
            }

            it->second = nullptr;
        }

        // Cleanup
        //TTF_CloseFont(_font_);


        // TODO: should all TTF and SDL init / quit go in one SDLManger class?

    }

    // call after initialization of SDL
    // TODO: would be better to put all SDL stuff in one class
    // an "SDL initializer" class
    //void Init()
    //{
    //TODO
    //}

    void PrintFontChar(const char ch/*, location, window/surface object*/)
    {
    //TODO
    }

    const std::map<const char, SDL_Texture*>& GetCharTexture() const
    {
        return _texture_chars_;
    }

    const std::map<const char, SDL_Rect>& GetCharSize() const
    {
        return _texture_chars_size_;
    }

/*
    const SDL_Rect& GetCharSize(const char ch) const
    {
        return _texture_chars_size_.at(ch);
    }
    
    const SDL_Texture* const GetCharTexture(const char ch) const
    {
        return _texture_chars_.at(ch);
    }
*/
    private:

    // globally used font
    //TTF_Font *_font_ = nullptr;

    // texture
    //SDL_Texture *_texture_;

    // TODO why do I need these?
    //unsigned short _texture_width_;
    //unsigned short _texture_height_; // think short is ok?

    //const std::string _texture_chars_{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "};
    //std::string _texture_chars_;
    std::map<const char, SDL_Texture*> _texture_chars_;
    std::map<const char, SDL_Rect> _texture_chars_size_;
    
    //const ColorPalette &_color_palette_;




};


#endif // FONTTEXTUREMANAGER_HPP
