#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <SDL.h>
#include <SDL_ttf.h>




#include "Config.hpp"
#include "Buffer.hpp"
#include "Cursor.hpp"
#include "KeyMap.hpp"
#include "Keyboard.hpp"
#include "ColorPalette.hpp"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>


// VIM like editor modes
enum class EditorMode
{
    NORMAL,
    COMMAND,
    INSERT,
    REPLACE // TODO: Is this seperate from insert?
};


class Window
{

    public:

    Window()
        : _window_(nullptr, SDL_DestroyWindow)
        , _surface_{nullptr}
        , _editor_mode_{EditorMode::NORMAL} // TODO: config file default mode
        //, COLOR_BACKGROUND{0x00000000}
        //, COLOR_TEXT_DEFAULT{0xFFFFFFFF}
        //, COLOR_CURRENT_LINE_BACKGROUND{0xFFFFFF00}
    {
        
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
                //COLOR_BACKGROUND = {0xFF, 0xFF, 0xFF};
                //COLOR_CURSOR = {0x00, 0xFF, 0x00};
                //COLOR_TEXT_DEFAULT = {0x00, 0x00, 0x00};
                //COLOR_CURRENT_LINE_BACKGROUND = {0xFF, 0xFF, 0x00};
                SDL_Color COLOR_BACKGROUND = _color_palette_.Get(ColorType::BACKGROUND);
                SDL_Color COLOR_CURSOR = _color_palette_.Get("green"); // TODO
                SDL_Color COLOR_TEXT_DEFAULT = _color_palette_.Get("black");
                SDL_Color COLOR_CURRENT_LINE_BACKGROUND = _color_palette_.Get("yellow");
                
                
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
                //_font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", 11);
                _font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", _config_.GetInt("fontsize"));
                if(_font_ == nullptr)
                {
                    std::cout << TTF_GetError() << std::endl;
                }
                else
                {
                    //Render text
                
                    // init printable characters
                    for(char ch{' '}; ; )
                    {
                
                        // render text surface
                        const char ch_string[2]{ch, '\0'};
                        SDL_Surface* text_surface = TTF_RenderText_Solid(_font_, ch_string /*_texture_chars_.c_str()*/, COLOR_TEXT_DEFAULT);
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
                
                        //_texture_chars_.push_back(ch);

                        if(ch == '~') break;
                        ++ ch;
                    }
                }







            }
        }

        // init cursor
        //219
        //const char cursor_string_normal[2]{(char), 0};
        // Font characters for cursor types
        const char cursor_string_normal[2]{' ', 0};
        const char cursor_string_insert[2]{'|', 0};
        const char cursor_string_replace[2]{'_', 0};

        //SDL_Surface* cursor_surface_normal = TTF_RenderText_Solid(_font_, cursor_string_normal, COLOR_CURSOR);
        //SDL_FillRect(cursor_surface_normal,
        //Uint32 width{_texture_chars_size_[' '].w};
        //Uint32 height{_texture_chars_size_[' '].h};
        //SDL_Surface* cursor_surface_normal = \
        //    SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        
        // Create cursor surface (blank rect)
        SDL_Surface* cursor_surface_normal = SDL_CreateRGBSurface(0, _texture_chars_size_[' '].w, _texture_chars_size_[' '].h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        if(cursor_surface_normal == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
        }
        
        // Create cursor surface (blank rect)
        SDL_Surface* cursor_surface_normal_1 = SDL_CreateRGBSurface(0, _texture_chars_size_[' '].w, _texture_chars_size_[' '].h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        if(cursor_surface_normal_1 == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
        }
        
        // Create cursor surfaces (blank rect)
        SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0xFF, 0x00, 0xFF));
        SDL_FillRect(cursor_surface_normal_1, nullptr, SDL_MapRGBA(cursor_surface_normal_1->format, 0x00, 0x80, 0x00, 0xFF));

        const SDL_Color COLOR_TEXT_DEFAULT{_color_palette_.Get("black")};

        // Create cursor surfaces from font
        SDL_Surface* cursor_surface_insert = TTF_RenderText_Solid(_font_, cursor_string_insert, COLOR_TEXT_DEFAULT);
        SDL_Surface* cursor_surface_replace = TTF_RenderText_Solid(_font_, cursor_string_replace, COLOR_TEXT_DEFAULT);

        // Create cursor textures from surfaces
        SDL_Texture* cursor_texture_normal = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_normal);
        SDL_Texture* cursor_texture_normal_1 = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_normal_1);
        SDL_Texture* cursor_texture_insert = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_insert);
        SDL_Texture* cursor_texture_replace = SDL_CreateTextureFromSurface(_renderer_, cursor_surface_replace);

        // Free cursor surfaces
        SDL_FreeSurface(cursor_surface_normal);
        SDL_FreeSurface(cursor_surface_normal_1);
        SDL_FreeSurface(cursor_surface_insert);
        SDL_FreeSurface(cursor_surface_replace);
        
        // Add texture pointers to cursor texture pointer vector
        _cursor_texture_.push_back(cursor_texture_normal);
        _cursor_texture_.push_back(cursor_texture_insert);
        _cursor_texture_.push_back(cursor_texture_replace);
        _cursor_texture_.push_back(cursor_texture_normal_1);
        //cursor_texture.push_back(cursor_texture_)
        
        // Set current cursor properties
        _current_cursor_ = 0;
        _cursor_texture_src_rect_.x = 0;
        _cursor_texture_src_rect_.y = 0;
        _cursor_texture_src_rect_.w = cursor_surface_normal->w;
        _cursor_texture_src_rect_.h = cursor_surface_normal->h;
        
        // Reset SDL timer after load
        _timer_ = SDL_GetTicks();
    }

    ~Window()
    {
        //SDL_DestroyWindow(_window_.get());
        //SDL_DestroyWindow(_window_);

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
                    if(_buffer_.NotSaved())
                    {
                        std::cout << "cannot quit, not saved" << std::endl;
                    }
                    else
                    {
                        quit = true;
                    }
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
                    /*
                    if(MOD_SHIFT) std::cout << "SHIFT" << std::endl;
                    if(MOD_CTRL) std::cout << "CTRL" << std::endl;
                    if(MOD_ALT) std::cout << "ALT" << std::endl;
                    if(MOD_GUI) std::cout << "GUI" << std::endl;
                    if(MOD_NONE) std::cout << "NONE" << std::endl;
                    */


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

                            // CTRL-Q: quit action
                            case SDLK_q:
                                if(MOD_CTRL && MOD_SHIFT)
                                {
                                    quit = true;
                                    break;
                                }
                                if(MOD_CTRL) // not needed
                                {
                                    //quit_action
                                    if(_buffer_.NotSaved())
                                    {
                                        std::cout << "The buffer is not saved, cannot quit" << std::endl;
                                        std::cout << "CTRL+SHIFT+Q to quit anyway" << std::endl;
                                        // TODO: better interactive error message here
                                    }
                                    else
                                    {
                                        quit = true;
                                    }
                                }
                                break;

                            // CTRL-S: save action
                            case SDLK_s:
                                if(MOD_CTRL) // not needed
                                {
                                    //save_action
                                    _buffer_.Save("buffer.txt");
                                    std::cout << "File " << "buffer.txt" << " written, " << _buffer_.Size() << " bytes" << std::endl;
                                }
                                break;

                            case SDLK_o:
                                if(MOD_CTRL) // not needed
                                {
                                    //open_action
                                    _buffer_.Open("buffer.txt");
                                    std::cout << "File " << "buffer.txt" << " read, " << _buffer_.Size() << " bytes" << std::endl;
                                }
                                break;

                            //default:
                            //    std::cerr << "Key: " << event.key.keysym.sym << " is not handled!" << std::endl;
                            //    break;
                        }
                
                    }
                    
                    if((MOD_NONE || MOD_SHIFT) && !MOD_CTRL)
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_BACKSPACE:
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
                    
                    if((MOD_NONE && !MOD_SHIFT) && !MOD_CTRL)
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
                        

                            case SDLK_RETURN:
                                _buffer_.ReturnAtCursor();
                                _buffer_.CursorCR();
                                _buffer_.CursorDown();
                        
                        }
                    }
                    
                    
                    // process printable characters
                    // these either have shift or no modifier
                    
                    if((MOD_NONE || MOD_SHIFT) && !MOD_CTRL)
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
                        {
                            _buffer_.InsertAtCursor(ch);
                            _buffer_.CursorRight();
                        }

                        // use map to process printable characters
                        // (insertable characters - any character
                        // which can be put into the buffer)
                        //char ch;
                        //if(_keymap_.Find(event.key.keysym.sym, ch))
                        //{
                            //std::cout << "found char " << ch << std::endl;
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
                                    //_buffer_.InsertAtCursor(ch);
                                    //_buffer_.CursorRight();
                            //    }
                            //}
                            //else
                            //{
                                // if not an a-z character, do not shift, just insert
                            //    _buffer_.InsertAtCursor(ch);
                            //    _buffer_.CursorRight();
                            //}
                        //}
                    }



                    // print buffer for debug
                    //std::cout << "buffer contents:" << std::endl;
                    //std::cout << _buffer_.Get() << std::endl;
                }
            }


            draw_window();
            
                                
            if(quit == true) break;


            //SDL_Delay(500);

        }

        return 0;
    }


    private:

    // Convert line number to string
    std::string line_number_to_string(const int line_number, const int line_number_width)
    {
        std::ostringstream ss;
        ss << std::setw(line_number_width) << std::setfill('0') << line_number;
        std::string line_number_str{ss.str()};
        return line_number_str;
    }
        

    // TODO: pass dst_rect by reference and modify within function
    void print_line_number(const int line_number, const int line_number_width, SDL_Rect &dst_rect)
    {
        std::string line_number_str{line_number_to_string(line_number, line_number_width)};
        //SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        std::string::const_iterator it{line_number_str.cbegin()};
        for(; it != line_number_str.cend(); ++ it)
        {
            char ch{*it};
            SDL_Texture *texture{_texture_chars_.at(ch)};
            SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
            dst_rect.w = src_rect.w;
            dst_rect.h = src_rect.h;
            SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);
            dst_rect.x += src_rect.w;
        }
    }


    ////////////////////////////////////////////////////////////////////////////
    // DRAW BUFFER TEXT
    ////////////////////////////////////////////////////////////////////////////
    void draw_buffer_contents()
    {


        bool line_number_enabled{false};
        int line_number_width{0};
        // Note: only 1 is true, any other integer is false
        if(_config_.GetInt("linenumber") == 1)
        {
            line_number_enabled = true;

            // set line number character count
            int line_count{_buffer_.GetLineCount()};
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


        // Draw line numbers
        // Create vector of line numbers
        //std::vector<std::string> line_number_str_vec;
        //const int line_count{_buffer_.GetLineCount()};
        //SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //for(int line{1}; line < line_count; ++ line)
        //{
        //    std::ostringstream ss;
        //    ss << std::setw(line_number_char_count) << std::setfill('0') << line;
        //    std::string line_number_str{ss.str()};
        //    //line_number_str_vec.push_back(line_number_str);
        //    std::string::const_iterator it{line_number_str.cbegin()};
        //    for(; it != line_number_str.cend(); ++ it)
        //    {
        //        char ch{*it};
        //        SDL_Texture *texture{_texture_chars_.at(ch)};
        //        SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
        //        dst_rect.w = src_rect.w;
        //        dst_rect.h = src_rect.h;
        //        SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);
        //        dst_rect.x += src_rect.w;
        //
        //    }
        //    dst_rect.y += dst_rect.h;
        //    dst_rect.x = 0;
        //}

        // Set rendering space and render to screen
        // size of individual characters
        // position set to origin of screen and character 'a' (first
        // character in the character string)
        int dst_rect_origin_x{0};
        // Move dst_rect_origin_x if line numbers are enabled
        if(line_number_enabled == true)
        {
            dst_rect_origin_x += line_number_width * _texture_chars_size_.at(' ').w;
        }

        int line_number{1};
        
        // Initialize destination rect for character printing
        SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //dst_rect = {0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //dst_rect.x = dst_rect_origin_x;
        //SDL_Rect src_rect{0, 0, _texture_width_ / _texture_chars_.size(), _texture_height_};

        // Print line number zero
        print_line_number(line_number, line_number_width, dst_rect);
        //dst_rect.x = dst_rect_origin_x; // move dst_rect to correct startpoint


        //std::cout << "texture_chars: " << _texture_chars_ << " size=" << _texture_chars_.size() << std::endl;
        //std::cout << "src_rect.w=" << src_rect.w << std::endl;


        // cursor position
        Cursor::CursorPos_t cursor_line{_buffer_.GetCursorLine()};
        Cursor::CursorPos_t cursor_col{_buffer_.GetCursorCol()};
        Cursor::CursorPos_t current_line{0};
        Cursor::CursorPos_t current_col{0};

        //std::cout << "cursor_line=" << cursor_line << " cursor_col=" << cursor_col << std::endl;

        // TODO: rather than using this we should be using the cursor pos variables
        // or add new variables to specify the position of the cursor on the screen
        // as well as the position of the cursor in the buffer
        SDL_Rect cursor_texture_dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount

        std::string::const_iterator it{_buffer_.Get().cbegin()};
        //for(; it != _buffer_.Get().cend(); ++ it)
        for(; it != _buffer_.Get().cend(); ++ it)
        {

            //std::cout << "line=" << current_line << " col=" << current_col << std::endl;

            // print char if not at end
            const char ch{*it};
             
            if(ch == '\n')
            {

                // TODO: ERROR IF h CHANGES!!!
                dst_rect.y += dst_rect.h;
                dst_rect.x = 0;
                //dst_rect.x = dst_rect_origin_x;
                
                ++ line_number;
                print_line_number(line_number, line_number_width, dst_rect);
                //dst_rect.x = dst_rect_origin.x; // below

                current_col = 0;
                ++ current_line;

                // only advance cursor y position if the cursor line
                // is below the current line!
                // without this the cursor ALWAYS sits on the final line
                if(cursor_line >= current_line)
                    cursor_texture_dst_rect.y += dst_rect.h;

            }
            else
            {
                
                // source rect origin always 0, 0
                // set the width and height on a per character basis using the map _texture_chars_size_
                SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
                // set the dst_rect to have same size
                dst_rect.w = src_rect.w;
                dst_rect.h = src_rect.h;
                // set the texture pointer
                SDL_Texture *texture{_texture_chars_.at(ch)};

                //std::cout << "ch=" << (int)ch << " offset=" << (int)(ch - _texture_chars_.at(0)) * src_rect.w << std::endl;
                //src_rect.x = (ch - _texture_chars_.at(0)) * src_rect.w;

                //Render current frame
                if(dst_rect.x + dst_rect.w >= _WIDTH_)
                {
                    // TODO: ERROR IF h CHANGES!!!
                    dst_rect.y += dst_rect.h;
                    dst_rect.x = 0;
                    dst_rect.x = dst_rect_origin_x; // this is only needed here because the line is split across multiple lines


                    if(cursor_line == current_line)
                    {
                        // advance position of where cursor is to be drawn
                        cursor_texture_dst_rect.x = 0;
                        cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount
                        cursor_texture_dst_rect.y += dst_rect.h;
                    }
                }

                // render texture to screen
                SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);

                //dst_rect.x += dst_rect.w;
                dst_rect.x += src_rect.w;

                //std::cout << "current_line=" << current_line << std::endl;
                //std::cout << "current_col=" << current_line << std::endl;
                //std::cout << "cursor_line=" << cursor_line << std::endl;
                if(cursor_line == current_line)
                {
                    if(current_col < cursor_col)
                    {
                        // advance position of where cursor is to be drawn
                        cursor_texture_dst_rect.x += src_rect.w;
                        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << " " << cursor_texture_dst_rect.w << " " << cursor_texture_dst_rect.h << std::endl;
                
                    }
                }

                
                ++ current_col;
            }


            //if(it == _buffer_.Get().cend())
            //    break;

        }
        //std::cin.get();
        
        ////////////////////////////////////////////////////////////////////////
        // Print cursor
        ////////////////////////////////////////////////////////////////////////

        
        // print cursor
        _current_cursor_ = 0;
        //SDL_RenderCopy(_renderer_, cursor_texture.at(current_cursor), &cursor_texture_src_rect, &cursor_texture_dst_rect);
        SDL_Rect src_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        cursor_texture_dst_rect.w = src_rect.w;
        cursor_texture_dst_rect.h = src_rect.h;

        //cursor_texture_dst_rect = dst_rect;


        // make cursor blink (TODO: use config option)
        int cursor_blink_rate{500}; // TODO
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

        // TODO: MULTIPLE TEXTURES REQUIRED BECAUSE COLOR CHANGE!
        SDL_Texture* _current_cursor_texture_ptr_{_cursor_texture_.at(_current_cursor_)};
        if(_timer_ / 500 % 2 == 1)
        {
            _current_cursor_texture_ptr_ = _cursor_texture_.at(3); // TODO: FIX THIS SHOULD NOT BE A CONST!
        }
        
        //std::cout << "PRINT" << std::endl;
        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << " " << cursor_texture_dst_rect.w << " " << cursor_texture_dst_rect.h << std::endl;
        //std::cout << src_rect.x << " " << src_rect.y << " " << src_rect.w << " " << src_rect.h << std::endl;
        //if(SDL_RenderCopy(_renderer_, _texture_chars_.at('x'), &src_rect, &cursor_texture_dst_rect) != 0)
        if(SDL_RenderCopy(_renderer_, _current_cursor_texture_ptr_, &src_rect, &cursor_texture_dst_rect) != 0)
        {
            std::cout << SDL_GetError() << std::endl;
        }
        //std::cout << "Cursor Position: " << cursor_line << ":" << cursor_col << std::endl;
        //SDL_RenderCopy(_renderer_, _texture_chars_.at('x'), &src_rect, &dst_rect);
        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << std::endl;


        // when buffer empty print cursor at end
        //if(_buffer_.Get().size() == 0)
        //{

        //}
    }



    void draw_window()
    {
    
        // reset timer for cursor
        _timer_ = SDL_GetTicks();
        
        // do graphics drawing
        //SDL_FillRect(_surface_, nullptr, COLOR_BACKGROUND);
        
        //SDL_UpdateWindowSurface(_window_.get());

        //Clear screen
        SDL_RenderClear(_renderer_);

        // draw the text buffer contents
        draw_buffer_contents();
    

        //Update screen
        SDL_RenderPresent(_renderer_);
    }

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
    //SDL_Texture *_texture_;

    // TODO why do I need these?
    //unsigned short _texture_width_;
    //unsigned short _texture_height_; // think short is ok?

    //const std::string _texture_chars_{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "};
    //std::string _texture_chars_;
    std::map<const char, SDL_Texture*> _texture_chars_;
    std::map<const char, SDL_Rect> _texture_chars_size_;

    //SDL_Window *_window_;
    //SDL_Surface *_surface_;

    Buffer _buffer_;
    //Cursor _cursor_;
    // TODO: move into cursor class
    // TODO: custom deleter
    std::vector<SDL_Texture*> _cursor_texture_;
    std::size_t _current_cursor_;
    SDL_Rect _cursor_texture_src_rect_;

    Keyboard _keyboard_;
    KeyMap _keymap_; // TODO: what do I do with this now?

    /*
    uint32_t COLOR_BACKGROUND;
    uint32_t COLOR_TEXT_DEFAULT;
    uint32_t COLOR_CURRENT_LINE_BACKGROUND;
    */

    //SDL_Color COLOR_BACKGROUND;
    //SDL_Color COLOR_CURSOR;
    //SDL_Color COLOR_TEXT_DEFAULT;
    //SDL_Color COLOR_CURRENT_LINE_BACKGROUND;
    ColorPalette _color_palette_;


    Uint32 _timer_;


    // configuration options
    Config _config_;

    // Editor mode
    EditorMode _editor_mode_;

};

#endif
