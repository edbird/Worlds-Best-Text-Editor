
#include <SDL.h>
#include <SDL_ttf.h>

#include <memory>

#include <iostream>
#include <fstream>

#include <cstdint>

#include <cmath>



#include "Buffer.hpp"
#include "Cursor.hpp"
#include "KeyMap.hpp"
#include "Keyboard.hpp"



class Window
{

    public:

    Window()
        : _window_(nullptr, SDL_DestroyWindow)
        , _surface_{nullptr}
        //, COLOR_BACKGROUND{0x00000000}
        //, COLOR_TEXT_DEFAULT{0xFFFFFFFF}
        //, COLOR_CURRENT_LINE_BACKGROUND{0xFFFFFF00}
    {
        // init printable characters
        for(unsigned char ch{' '}; ; )
        {
            _texture_chars_.push_back(ch);

            if(ch == '~') break;
            ++ ch;
        }
        
        // TODO: should go elsewhere?
        // TODO: error message
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
        }

        //Initialize SDL_ttf
        if(TTF_Init() < 0) // TODO check return codes
        {
            std::cout << TTF_GetError() << std::endl;
        }


        //_window_ = nullptr;
        //_surface_ = nullptr;

        else
        {
            _window_.reset(SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _WIDTH_, _HEIGHT_, SDL_WINDOW_SHOWN));
            //_window_ = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _WIDTH_, _HEIGHT_, SDL_WINDOW_SHOWN);

            if(_window_ == nullptr)
            {
                std::cerr << SDL_GetError() << std::endl;
            }
            else
            {
                //_surface_.reset(SDL_GetWindowSurface(_window_.get()));
                _surface_ = SDL_GetWindowSurface(_window_.get());
                //_surface_ = SDL_GetWindowSurface(_window_);

                //SDL_FillRect(_surface_.get(), nullptr, SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF));
                //SDL_FillRect(_surface_, nullptr, SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF));
                //SDL_FillRect(_surface_, nullptr, SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF));

                //SDL_UpdateWindowSurface(_window_.get());
                //SDL_UpdateWindowSurface(_window_);

                //SDL_Delay(1000);


                // map colors
                //COLOR_BACKGROUND = SDL_MapRGB(_surface_->format, 0x00, 0x00, 0x00);
                //COLOR_TEXT_DEFAULT = SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF);
                //COLOR_CURRENT_LINE_BACKGROUND = SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0x00);
                COLOR_BACKGROUND = {0xFF, 0xFF, 0xFF};
                COLOR_TEXT_DEFAULT = {0x00, 0x00, 0x00};
                COLOR_CURRENT_LINE_BACKGROUND = {0xFF, 0xFF, 0x00};
                
                
                // create renderer object
                _renderer_ = SDL_CreateRenderer(_window_.get(), -1, SDL_RENDERER_ACCELERATED); // TODO vsync
                if(_renderer_ == nullptr)
                {
                    std::cout << SDL_GetError() << std::endl;
                }
                else
                {
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(_renderer_, COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);

                    //Initialize PNG loading
                    /*int imgFlags = IMG_INIT_PNG;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        std::cout << SDL_GetError() << std::endl;
                    }*/
                }


                //Open the font
                _font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", 10);
                if(_font_ == nullptr)
                {
                    std::cout << TTF_GetError() << std::endl;
                }
                else
                {
                    //Render text
                
                    // render text surface
                    SDL_Surface* _text_surface_ = TTF_RenderText_Solid(_font_, _texture_chars_.c_str(), COLOR_TEXT_DEFAULT);
                    if(_text_surface_ == nullptr)
                    {
                        std::cout << TTF_GetError() << std::endl;
                    }
                    else
                    {
                        // create texture from surface pixels
                        _texture_ = SDL_CreateTextureFromSurface(_renderer_, _text_surface_);
                        if(_texture_ == nullptr)
                        {
                            std::cout << SDL_GetError() << std::endl;
                        }
                        else
                        {
                            //Get image dimensions
                            _texture_width_ = _text_surface_->w;
                            _texture_height_ = _text_surface_->h;
                        }

                        //Get rid of old surface
                        SDL_FreeSurface(_text_surface_);
                    } 
                
                }







            }
        }
    }

    ~Window()
    {
        //SDL_DestroyWindow(_window_.get());
        //SDL_DestroyWindow(_window_);

        // TODO this check elsewhere
        if(_texture_ != nullptr)
        {
            SDL_DestroyTexture(_texture_);
            _texture_ = nullptr;
            _texture_width_ = 0;
            _texture_height_ = 0;
        }

        TTF_CloseFont(_font_);

        SDL_DestroyRenderer(_renderer_);

        TTF_Quit();
 
        SDL_Quit();
    }

    int run()
    {
        // TODO
        // regarding keyboard input it would be better to have a unified
        // interface using function pointers
        // so that the Keyboard class has a pair of maps
        // which map SDLK_keys to function pointers
        // these functions modify the buffer as they should


        bool quit{false};
        for(;;)
        {
            
            // poll events
            SDL_Event event;

            while(SDL_PollEvent(&event) != 0)
            {
                
                // send data to Keyboard class
                _keyboard_.ProcessEvent(event);

                // user request quit
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }

                // keypress events
                else if((event.type == SDL_KEYDOWN) /*|| (event.type == SDL_KEYUP)*/)
                {

                    // get the modifier state
                    // split into different modifier constants
                    //const SDL_Keymod MOD{SDL_GetModState()};
                    //if(MOD & KMOD_SHIFT == KMOD_SHIFT)
                    //std::cout << (int)MOD << std::endl;
                    //const bool MOD_SHIFT{MOD & KMOD_SHIFT == KMOD_SHIFT};
                    //const bool MOD_CTRL{MOD & KMOD_CTRL == KMOD_CTRL};
                    //const bool MOD_ALT{MOD & KMOD_ALT == KMOD_ALT};
                    //const bool MOD_LSHIFT{MOD & KMOD_LSHIFT == KMOD_LSHIFT};
                    //const bool MOD_NONE{MOD == KMOD_NONE};
                    const bool MOD_SHIFT{_keyboard_.ShiftState()};
                    const bool MOD_CTRL{_keyboard_.CTRLState()};
                    const bool MOD_ALT{_keyboard_.AltState()};
                    const bool MOD_GUI{_keyboard_.GUIState()};
                    const bool MOD_LSHIFT{_keyboard_.LShiftState()};
                    const bool MOD_NONE{!_keyboard_.ModState()};
                    if(MOD_SHIFT) std::cout << "SHIFT" << std::endl;
                    if(MOD_CTRL) std::cout << "CTRL" << std::endl;
                    if(MOD_ALT) std::cout << "ALT" << std::endl;
                    if(MOD_GUI) std::cout << "GUI" << std::endl;
                    if(MOD_NONE) std::cout << "NONE" << std::endl;


                    // TODO: this is a hack!
                    // check if any "detected" modifier is pressed, if so then
                    // invert to get MOD_NONE
                    //const bool MOD_NONE{!(MOD_SHIFT || MOD_CTRL || MOD_ALT)};
                    //if(MOD_NONE) std::cout << "NONE" << std::endl;

                    if(MOD_CTRL)
                    {
                        // process control keys

                        switch(event.key.keysym.sym)
                        {
                            // TODO: mappings

                            #define SDLK_KEY_CASE_MACRO(X, Y) \
                            {\
                            case X: \
                                _buffer_.InsertAtCursor(Y); \
                                _buffer_.CursorRight(); \
                                break;\
                            }

                            // CTRL-S: save action
                            case SDLK_s:
                                if(MOD_CTRL)
                                {
                                    //save_action
                                    _buffer_.Save("buffer.txt");
                                    std::cout << "File " << "buffer.txt" << " written, " << _buffer_.Size() << " bytes" << std::endl;
                                }
                                break;
               
                            /*
                            case SDLK_a:
                                _buffer_.InsertAtCursor('a');
                                _buffer_.MutableCursor().Right();
                                break;

                            SDLK_KEY_CASE_MACRO(SDLK_b, 'b');
                            SDLK_KEY_CASE_MACRO(SDLK_c, 'c');
                            SDLK_KEY_CASE_MACRO(SDLK_d, 'd');
                            SDLK_KEY_CASE_MACRO(SDLK_e, 'e');
                            SDLK_KEY_CASE_MACRO(SDLK_f, 'f');
                            SDLK_KEY_CASE_MACRO(SDLK_g, 'g');
                            SDLK_KEY_CASE_MACRO(SDLK_h, 'h');
                            SDLK_KEY_CASE_MACRO(SDLK_i, 'i');
                            SDLK_KEY_CASE_MACRO(SDLK_j, 'j');
                            SDLK_KEY_CASE_MACRO(SDLK_k, 'k');
                            SDLK_KEY_CASE_MACRO(SDLK_l, 'l');
                            SDLK_KEY_CASE_MACRO(SDLK_m, 'm');
                            SDLK_KEY_CASE_MACRO(SDLK_n, 'n');
                            SDLK_KEY_CASE_MACRO(SDLK_o, 'o');
                            SDLK_KEY_CASE_MACRO(SDLK_p, 'p');
                            SDLK_KEY_CASE_MACRO(SDLK_q, 'q');
                            SDLK_KEY_CASE_MACRO(SDLK_r, 'r');
                            SDLK_KEY_CASE_MACRO(SDLK_s, 's');
                            SDLK_KEY_CASE_MACRO(SDLK_t, 't');
                            SDLK_KEY_CASE_MACRO(SDLK_u, 'u');
                            SDLK_KEY_CASE_MACRO(SDLK_v, 'v');
                            SDLK_KEY_CASE_MACRO(SDLK_w, 'w');
                            SDLK_KEY_CASE_MACRO(SDLK_x, 'x');
                            SDLK_KEY_CASE_MACRO(SDLK_y, 'y');
                            SDLK_KEY_CASE_MACRO(SDLK_z, 'z');
                            SDLK_KEY_CASE_MACRO(SDLK_SPACE, ' ');
                            */


                            //default:
                            //    std::cerr << "Key: " << event.key.keysym.sym << " is not handled!" << std::endl;
                            //    break;
                        }
                    }
                    else if(MOD_NONE && !MOD_SHIFT)
                    {
                        // movement keys
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_UP:
                                _buffer_.CursorUp();        
                                break;


                            case SDLK_DOWN:
                                _buffer_.CursorDown();
                                break;


                            case SDLK_LEFT:
                                _buffer_.CursorLeft();
                                break;


                            case SDLK_RIGHT:
                                _buffer_.CursorRight();
                                break;
                        
                            case SDLK_BACKSPACE:
                                std::cout << "BACK SPACE" << std::endl;
                                // only move if the buffer could execute the backspace
                                // command; ie if a char was deleted
                                if(_buffer_.BackspaceAtCursor() == true)
                                {
                                    std::cout << "moving cursor left" << std::endl;
                                    _buffer_.CursorLeft();
                                }
                                break;

                        
                        }
                    }
                    
                    
                    // process printable characters
                    // these either have shift or no modifier
                    
                    if(MOD_NONE || MOD_SHIFT)
                    {
                        // how the event loop works:
                        // the most recently pressed/released key is always stored
                        // in event.key.keysym.sym
                        // The Keyboard class maintains the current state of the
                        // keyboard, which could include several pressed keys.
                        // The Keyboard class is also used to map a SDL_Keycode
                        // to a printable character
                        char ch;
                        //if(_keyboard_.GetChar(event.key.keysym.sym, ch))
                        if(_keyboard_.GetChar(ch))

                        // use map to process printable characters
                        // (insertable characters - any character
                        // which can be put into the buffer)
                        //char ch;
                        //if(_keymap_.Find(event.key.keysym.sym, ch))
                        {
                            std::cout << "found char " << ch << std::endl;
                            // anything here is "buffer insertable"
                            //if(('a' <= ch) && (ch <= 'z'))
                            //{
                                // detect shift press
                            //    if(MOD_SHIFT)
                            //    {
                            //        std::cout << "shift" << std::endl;

                                    // shift all caps letters
                            //        const char CHAR_SHIFT_DIFF{'a' - 'A'};
                            //        std::cout << (int)CHAR_SHIFT_DIFF << std::endl;
                            //        const char shift_ch{ch - CHAR_SHIFT_DIFF};
                            //        _buffer_.InsertAtCursor(shift_ch);
                            //        _buffer_.InsertAtCursor(ch);
                            //        _buffer_.CursorRight();
                            //    }
                            //    else if(MOD_NONE)
                            //    {
                            //        std::cout << "no shift" << std::endl;

                                    // no modifier: insert unchanged character ch
                                    _buffer_.InsertAtCursor(ch);
                                    _buffer_.CursorRight();
                            //    }
                            //}
                            //else
                            //{
                                // if not an a-z character, do not shift, just insert
                            //    _buffer_.InsertAtCursor(ch);
                            //    _buffer_.CursorRight();
                            //}
                        }
                    }



                    // print buffer for debug
                    std::cout << "buffer contents:" << std::endl;
                    std::cout << _buffer_.Get() << std::endl;
                }
            }


            // do graphics drawing
            //SDL_FillRect(_surface_, nullptr, COLOR_BACKGROUND);
            
            //SDL_UpdateWindowSurface(_window_.get());

            //Clear screen
            SDL_RenderClear(_renderer_);

            //Set rendering space and render to screen
            // size of individual characters
            // position set to origin of screen and character 'a' (first
            // character in the character string)
            SDL_Rect ds_rect{0, 0, _texture_width_ / _texture_chars_.size(), _texture_height_};
            SDL_Rect src_rect{0, 0, _texture_width_ / _texture_chars_.size(), _texture_height_};

            //std::cout << "texture_chars: " << _texture_chars_ << " size=" << _texture_chars_.size() << std::endl;
            //std::cout << "src_rect.w=" << src_rect.w << std::endl;

            std::string::const_iterator it{_buffer_.Get().cbegin()};
            for(; it != _buffer_.Get().cend(); ++ it)
            {

                const char ch{*it};
                //std::cout << "ch=" << (int)ch << " offset=" << (int)(ch - _texture_chars_.at(0)) * src_rect.w << std::endl;
                src_rect.x = (ch - _texture_chars_.at(0)) * src_rect.w;

                //Render current frame
	        

                //Set clip rendering dimensions
                //SDL_Rect *clip{nullptr};
                //if(clip != nullptr)
                //{
                //    rect.w = clip->w;
                //    rect.h = clip->h;
                //}

                //Render to screen
                //SDL_RenderCopyEx(_renderer_, _texture_, clip, &rect, 0.0, nullptr, SDL_FLIP_NONE);

                //Render texture to screen
                SDL_RenderCopy(_renderer_, _texture_, &src_rect, &ds_rect);

                ds_rect.x += ds_rect.w;
            }


            //Update screen
            SDL_RenderPresent(_renderer_);
            
                                
            if(quit == true) break;
        }

        return 0;
    }


    private:

    const int32_t _WIDTH_{600};
    const int32_t _HEIGHT_{400};

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window_;
    //std::unique_ptr<SDL_Surface> _surface_;
    SDL_Surface *_surface_;

    // text renderer
    SDL_Renderer *_renderer_ = nullptr; // TODO smart

    // globally used font
    TTF_Font *_font_ = nullptr;

    // texture
    SDL_Texture *_texture_;

    // TODO why do I need these?
    unsigned short _texture_width_;
    unsigned short _texture_height_; // think short is ok?

    //const std::string _texture_chars_{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "};
    std::string _texture_chars_;

    //SDL_Window *_window_;
    //SDL_Surface *_surface_;

    Buffer _buffer_;
    //Cursor _cursor_;

    Keyboard _keyboard_;
    KeyMap _keymap_; // TODO: what do I do with this now?

    /*
    uint32_t COLOR_BACKGROUND;
    uint32_t COLOR_TEXT_DEFAULT;
    uint32_t COLOR_CURRENT_LINE_BACKGROUND;
    */

    SDL_Color COLOR_BACKGROUND;
    SDL_Color COLOR_TEXT_DEFAULT;
    SDL_Color COLOR_CURRENT_LINE_BACKGROUND;

};


// TODO: implement application


int main(int argc, char* argv[])
{

    Window window;
    window.run();

    return 0;
}
