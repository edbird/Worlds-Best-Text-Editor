#ifndef WINDOW_HPP
#define WINDOW_HPP


#include "Actions.hpp"
#include "Config.hpp"
#include "FontTextureManager.hpp"
//#include "Buffer.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "Cursor.hpp"
#include "KeyMap.hpp"
#include "Keyboard.hpp"
#include "ColorPalette.hpp"


#include <SDL.h>
#include <SDL_ttf.h>


#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>


// VIM like editor modes
enum class EditorMode
{
    NORMAL,
    COMMAND,
    //INSERT,
    //REPLACE // TODO: Is this seperate from insert?
    EDIT
};

// strange name but switches between insert and replace modes when in edit mode
enum class EditorEditMode
{
    INSERT,
    REPLACE
};






// TODO: remove Textbox& by accessing textbox through Window&
void fc_enter_edit_mode(Window&);
void fc_exit_edit_mode(Window&);
void fc_quit_request(Window&);
void fc_quit_force(Window&);
void fc_open(Window&);
void fc_save(Window&);

// for debugging, increment / decrement the scroll index
void fc_scroll_inc(Window&);
void fc_scroll_dec(Window&);
void fc_scroll_inc_sub(Window&);
void fc_scroll_dec_sub(Window&);


// debug function
void fc_print_buffer(Window&);

void fc_up(Window&);
void fc_down(Window&);
void fc_left(Window&);
void fc_right(Window&);



class Window
{

    // list of callback functions
    friend void fc_enter_edit_mode(Window&);
    friend void fc_exit_edit_mode(Window&);
    friend void fc_quit_request(Window&);
    friend void fc_quit_force(Window&);
    friend void fc_open(Window&);
    friend void fc_save(Window&);
    friend void fc_print_buffer(Window&);

    friend void fc_scroll_inc(Window&);
    friend void fc_scroll_dec(Window&);
    friend void fc_scroll_inc_sub(Window&);
    friend void fc_scroll_dec_sub(Window&);

    friend void fc_up(Window&);
    friend void fc_down(Window&);
    friend void fc_left(Window&);
    friend void fc_right(Window&);


    public:

    Window(const Config& config, const TTF_Font* const font)
        : _quit_{false}
        , _window_(nullptr, SDL_DestroyWindow)
        , _surface_{nullptr}
        //, _font_{font}
        , _editor_mode_{EditorMode::NORMAL} // TODO: config file default mode
        , _config_{config}
        //, _ftm_{ftm}
        //, COLOR_BACKGROUND{0x00000000}
        //, COLOR_TEXT_DEFAULT{0xFFFFFFFF}
        //, COLOR_CURRENT_LINE_BACKGROUND{0xFFFFFF00}
    {
        
        

            
        _window_.reset(SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _WIDTH_, _HEIGHT_, SDL_WINDOW_SHOWN));

        if(_window_ == nullptr)
        {
            std::cerr << SDL_GetError() << std::endl;
        }
        else
        {
            // not using smart pointer as this is not a managed resource
            //_surface_.reset(SDL_GetWindowSurface(_window_.get()));
            _surface_ = SDL_GetWindowSurface(_window_.get());

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
                // initialize renderer color
                SDL_SetRenderDrawColor(_renderer_, COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);

            }


            // assumes font is valid (not nullptr)
            //init_texture_chars();
            _ftm_ = new FontTextureManager(_renderer_, font, _color_palette_); // TODO make font a member var

            //init_cursor();

            // TODO: do not pass this as an argument, pass a pointer to a resources class
            //_buffer_ptr_ = new Buffer(_config_, _texture_chars_size_);
            _textbox_ptr_ = new Textbox(_config_, *_ftm_);
        
            _status_label_ = new Label("Worlds Best Text Editor", *_ftm_);
            //_status_label_->SetPosition(0, _size_y_);
            _status_label_->SetPosition(0, _HEIGHT_);
            _status_label_->SetAnchor(LabelAnchor::BOTTOM_LEFT);

            // Reset SDL timer after load
            _timer_ = SDL_GetTicks();

            _refresh_delay_ = std::floor((1000.0 / (double)config.GetInt("targetrefreshrate")));
            // TODO: remove, temp hack to make slow
            _refresh_delay_ = 500.0;
            std::cout << "refreshdelay set to: " << _refresh_delay_ << std::endl;
            // TODO: keep track of the rounded part and add it on to the next refresh delay to get
            // a more accurage 60 Hz refresh


            init_action_keys();

        }

        
    }

    ~Window()
    {
        delete _ftm_;
    
        //delete _buffer_ptr_;
        delete _textbox_ptr_;

        delete _status_label_;

        // clean action key vectors
        {
            std::vector<std::pair<ActionKey*, EditorMode>>::iterator it{akv_editor_mode_specific.begin()};
            for(; it != akv_editor_mode_specific.end(); ++ it)
            {
                delete it->first;
            }
        }

        // clean action key vectors
        {
            std::vector<ActionKey*>::iterator it{akv.begin()};
            for(; it != akv.end(); ++ it)
            {
                delete *it;
            }
        }


    
        //SDL_DestroyWindow(_window_.get());
        //SDL_DestroyWindow(_window_);

        // Delete cursor textures
        /*
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
        */


        SDL_DestroyRenderer(_renderer_);

    }
    
    
    
    private:
    
    // old function - moved to FontTextureManager
    //void init_texture_chars()
    //{
    //            
    //    // init printable characters
    //    for(char ch{' '}; ; )
    //    {
    //
    //        // render text surface
    //        const char ch_string[2]{ch, '\0'};
    //        
    //        // render text
    //        SDL_Color COLOR_TEXT_DEFAULT = _color_palette_.Get("black");
    //        SDL_Surface* text_surface = TTF_RenderText_Solid((TTF_Font*)_font_, ch_string /*_texture_chars_.c_str()*/, COLOR_TEXT_DEFAULT);
    //        
    //        if(text_surface == nullptr)
    //        {
    //            std::cout << TTF_GetError() << std::endl;
    //        }
    //        else
    //        {
    //            // create texture from surface pixels
    //            SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer_, text_surface);
    //            if(texture == nullptr)
    //            {
    //                std::cout << SDL_GetError() << std::endl;
    //            }
    //            else
    //            {
    //                //Get image dimensions
    //                //_texture_width_ = _text_surface_->w;
    //                //_texture_height_ = _text_surface_->h;
    //
    //                _texture_chars_[ch] = texture;
    //                _texture_chars_size_[ch].w = text_surface->w;
    //                _texture_chars_size_[ch].h = text_surface->h;
    //            }
    //
    //            //Get rid of old surface
    //            SDL_FreeSurface(text_surface);
    //        } 
    //
    //        if(ch == '~') break;
    //        ++ ch;
    //    }
    //}
    
    void init_action_keys()
    {

        ////////////////////////////////////////////////////////////////////////
        // DEFINE ACTION KEYS
        ////////////////////////////////////////////////////////////////////////

        // this ActionKey is the object we check the above against
        // this action for key "e"
        // with NO shift state,
        // with NO ctrl state,
        // with NO alt state,
        // with NO gui state

        // order: shift, ctrl, alt, gui


        ////////////////////////////////////////////////////////////////////////
        // NOT EDITOR MODE SPECIFIC
        ////////////////////////////////////////////////////////////////////////

        // ctrl q
        ActionKey* ak_quit_request = new ActionKey(fc_quit_request, SDLK_q, SCAModState::NONE, SCAModState::ANY);

        // ctrl shift q
        ActionKey* ak_quit_force = new ActionKey(fc_quit_force, SDLK_q, SCAModState::ANY, SCAModState::ANY);

        // ctrl s
        ActionKey* ak_save = new ActionKey(fc_save, SDLK_s, SCAModState::NONE, SCAModState::ANY);

        // ctrl o
        ActionKey* ak_open = new ActionKey(fc_open, SDLK_o, SCAModState::NONE, SCAModState::ANY);

        // F12
        ActionKey* ak_print_buffer = new ActionKey(fc_print_buffer, SDLK_F12, SCAModState::DONT_CARE, SCAModState::DONT_CARE);

        // TODO: PLUS key maps to same as EQUALS, but with different shift state!
        // TODO: some keys have double maps
        ActionKey* ak_scroll_inc = new ActionKey(fc_scroll_inc, SDLK_EQUALS, SCAModState::DONT_CARE, SCAModState::RIGHT_ONLY);
        ActionKey* ak_scroll_dec = new ActionKey(fc_scroll_dec, SDLK_MINUS, SCAModState::DONT_CARE, SCAModState::RIGHT_ONLY);
        ActionKey* ak_scroll_inc_sub = new ActionKey(fc_scroll_inc_sub, SDLK_EQUALS, SCAModState::DONT_CARE, SCAModState::LEFT_ONLY);
        ActionKey* ak_scroll_dec_sub = new ActionKey(fc_scroll_dec_sub, SDLK_MINUS, SCAModState::DONT_CARE, SCAModState::LEFT_ONLY);

        
        ////////////////////////////////////////////////////////////////////////
        // EDITOR MODE SPECIFIC
        ////////////////////////////////////////////////////////////////////////
        
        // e
        ActionKey* ak_enter_edit_mode = new ActionKey(fc_enter_edit_mode, SDLK_e);
        // TODO: change from fixed SDLK_* keys to variables, which can be changed in config?
        // need to check how the key maps worked to figure out what to do here

        // esc
        ActionKey* ak_exit_edit_mode = new ActionKey(fc_exit_edit_mode, SDLK_ESCAPE);
        

        // ijkl left right up down, edit mode
        ActionKey* ak_up_edit = new ActionKey(fc_up, SDLK_i, SCAModState::DONT_CARE, SCAModState::ANY);
        ActionKey* ak_down_edit = new ActionKey(fc_down, SDLK_k, SCAModState::DONT_CARE, SCAModState::ANY);
        ActionKey* ak_left_edit = new ActionKey(fc_left, SDLK_j, SCAModState::DONT_CARE, SCAModState::ANY);
        ActionKey* ak_right_edit = new ActionKey(fc_right, SDLK_l, SCAModState::DONT_CARE, SCAModState::ANY);
        // ijkl left right up down, normal mode
        ActionKey* ak_up_normal = new ActionKey(fc_up, SDLK_i);
        ActionKey* ak_down_normal = new ActionKey(fc_down, SDLK_k);
        ActionKey* ak_left_normal = new ActionKey(fc_left, SDLK_j);
        ActionKey* ak_right_normal = new ActionKey(fc_right, SDLK_l);


        
        ////////////////////////////////////////////////////////////////////////
        // ADD TO VECTORS
        ////////////////////////////////////////////////////////////////////////
        
        // these trigger regardless of editor mode
        // or only for edit mode ?
        akv.push_back(ak_quit_request);
        akv.push_back(ak_quit_force);
        akv.push_back(ak_save);
        akv.push_back(ak_open);
        akv.push_back(ak_print_buffer);
        akv.push_back(ak_scroll_inc);
        akv.push_back(ak_scroll_dec);
        akv.push_back(ak_scroll_inc_sub);
        akv.push_back(ak_scroll_dec_sub);
                    
        
        // this triggers for normal mode only
        akv_editor_mode_specific.push_back({ak_enter_edit_mode, EditorMode::NORMAL});
        // this triggers for editor mode only
        akv_editor_mode_specific.push_back({ak_exit_edit_mode, EditorMode::EDIT});

        akv_editor_mode_specific.push_back({ak_up_edit, EditorMode::EDIT});
        akv_editor_mode_specific.push_back({ak_down_edit, EditorMode::EDIT});
        akv_editor_mode_specific.push_back({ak_left_edit, EditorMode::EDIT});
        akv_editor_mode_specific.push_back({ak_right_edit, EditorMode::EDIT});
        akv_editor_mode_specific.push_back({ak_up_normal, EditorMode::NORMAL});
        akv_editor_mode_specific.push_back({ak_down_normal, EditorMode::NORMAL});
        akv_editor_mode_specific.push_back({ak_left_normal, EditorMode::NORMAL});
        akv_editor_mode_specific.push_back({ak_right_normal, EditorMode::NORMAL});

    }
    



    public:

    int Run()
    {
        //Buffer &_buffer_{*_buffer_ptr_};
        Textbox &textbox{*_textbox_ptr_};
    
        // TODO
        // regarding keyboard input it would be better to have a unified
        // interface using function pointers
        // so that the Keyboard class has a pair of maps
        // which map SDLK_keys to function pointers
        // these functions modify the buffer as they should


        //bool quit{false};
        for(;;)
        {

            // TODO: correct delay time
            //SDL_Delay(50);


            // poll events
            SDL_Event event;

            while(SDL_PollEvent(&event) != 0)
            {
                
                // send data to Keyboard class
                _keyboard_.ProcessEvent(event);

                // TODO: what to do about this thing? it is not a keyboard action!
                // but functionality is the same
                // user request quit
                if(event.type == SDL_QUIT)
                {
                    if(_textbox_ptr_->NotSaved())
                    {
                        std::cout << "cannot quit, not saved" << std::endl;
                    }
                    else
                    {
                        //quit = true;
                        _quit_ = true;
                    }
                }

                // keypress events
                else if((event.type == SDL_KEYDOWN) /*|| (event.type == SDL_KEYUP)*/)
                {

                    // get the modifier state
                    // split into different modifier constants
                    //const SDL_Keymod MOD{SDL_GetModState()};
                    const bool MOD_SHIFT{_keyboard_.ShiftState()};
                    const bool MOD_CTRL{_keyboard_.CTRLState()};
                    const bool MOD_ALT{_keyboard_.AltState()};
                    const bool MOD_GUI{_keyboard_.GUIState()};
                    const bool MOD_LSHIFT{_keyboard_.LeftShiftState()};
                    const bool MOD_NONE{!_keyboard_.ModState()};

                    // store the current key press and keyboard modifier states
                    // in an ActionKey2 object
                    CurrentKeyboardAction current_keyboard_action(_keyboard_);



                    bool fired{false};
                    std::vector<std::pair<ActionKey*, EditorMode>>::iterator it{akv_editor_mode_specific.begin()};
                    for(; it != akv_editor_mode_specific.end(); ++ it)
                    {
                        // check correct editor mode
                        if(it->second == _editor_mode_)
                        {
                            // check keyboard action key matches
                            if(current_keyboard_action == *(it->first))
                            {
                                std::cout << "FIRE" << std::endl;
                                it->first->Fire(*this);
                                fired = true;
                                break;
                            }
                        }
                    }

                    // iterate through akv
                    if(!fired)
                    {
                        std::vector<ActionKey*>::iterator it{akv.begin()};
                        for(; it != akv.end(); ++ it)
                        {
                            if(current_keyboard_action == **it)
                            {
                                std::cout << "FIRE" << std::endl;
                                (*it)->Fire(*this);
                                fired = true;
                                break;
                            }
                        }
                    }

                    
                    if(!fired)
                    {

                        // process any keys which do not care about the editor mode
                        if((MOD_NONE && !MOD_SHIFT) && !MOD_CTRL)
                        {

                            // Note: These keys do not appear anywhere else, in either
                            // of the EditorMode if statements, so they can safely be
                            // processed here (no chance of triggering 2 different
                            // actions)
                            switch(event.key.keysym.sym)
                            {
                                // movement keys
                                case SDLK_UP:
                                    _textbox_ptr_->CursorUp();        
                                    break;


                                case SDLK_DOWN:
                                    _textbox_ptr_->CursorDown();
                                    break;


                                case SDLK_LEFT:
                                    _textbox_ptr_->CursorLeft();
                                    break;


                                case SDLK_RIGHT:
                                    _textbox_ptr_->CursorRight();
                                    break;

                            }
                        }


                        // switch on editor mode first
                        //if(_editor_mode_ == EditorMode::NORMAL)
                        //{

                            // switch on modifier state second
                            // TODO: change to switch on key second, and then
                            // mod state third
                            // NOTE: may not be required as new "action key"
                            // is to be implemented
                            //if(MOD_CTRL)
                                // process control keys
                            // NOTE: done [+]

                            // TODO: this should not depend on the editor mode!
                            // TODO: implement action key before returning to fix this
                            // TODO: need to completely re-think how this text editor should behave
                           
                        //}

                        
                        // process printable characters
                        // these either have shift or no modifier
                        // also process ESC key for swap back to normal mode
                        //else
                        if(_editor_mode_ == EditorMode::EDIT)
                        {

                            if((MOD_NONE && !MOD_SHIFT) && !MOD_CTRL)
                            {
                                switch(event.key.keysym.sym)
                                {

                                    // exit edit mode
                                    //case SDLK_ESCAPE:
                                    //    _editor_mode_ = EditorMode::NORMAL;
                                    //    break;

                                    // insert new line
                                    case SDLK_RETURN:
                                        _textbox_ptr_->ReturnAtCursor();
                                        _textbox_ptr_->CursorCR();
                                        _textbox_ptr_->CursorDown();
                                        break;

                                    default:
                                        break;
                                    
                                }
                            }

                            // backspace only works for NO MOD or SHIFT AND NOT CONTROL
                            // NOTE: I am still not happy about the implementation of these mod keys
                            // this states that we don't care about the WIN key for example!
                            if(MOD_NONE || (MOD_SHIFT && !MOD_CTRL))
                            {
                                switch(event.key.keysym.sym)
                                {

                                    // backspace
                                    case SDLK_BACKSPACE:
                                        // only move if the buffer could execute the backspace
                                        // command; ie if a char was deleted
                                        //if(_textbox_ptr_->BackspaceAtCursor() == true)
                                        //{
                                        //    std::cout << "moving cursor left" << std::endl;
                                        //    _textbox_ptr_->CursorLeft();
                                        //}
                                        _textbox_ptr_->BackspaceAtCursor(); // TODO: change other functions to follow the new format
                                        // see Textbox.hpp for more details (implementation hiding)
                                        break;

                                    default:
                                        break;

                                }
                            }

                            // TODO: does MOD_NONE imply !MOD_CTRL, if not it should?
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
                                    _textbox_ptr_->InsertAtCursor(ch);
                                    _textbox_ptr_->CursorRight();
                                }

                            }
                        }
                    } // if !fired


                    // print buffer for debug
                    //std::cout << "buffer contents:" << std::endl;
                    //std::cout << _buffer_.Get() << std::endl;
                }
            }


            draw_window();
            
                                
            if(_quit_ == true) break;


            //SDL_Delay(500);

        }

        return 0;
    }


    private:

    


    // TODO: remove pseudofunction
    //void draw_buffer_contents()
    //{
    //    //Buffer &_buffer_{*_buffer_ptr_};
    //    //_buffer_.Draw(_renderer_, _texture_chars_, _texture_chars_size_, _timer_);
    //    
    //    _textbox_ptr_->Draw(_renderer_, _timer_);
    //}



    void draw_window()
    {

        // sdl delay until time to draw
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time{current_time - _timer_};
        // 50 ms additional included
        if(elapsed_time /*+ 50*/ < _refresh_delay_)
        {
            SDL_Delay(_refresh_delay_ - elapsed_time);
        }
    
        // reset timer for cursor
        _timer_ = current_time; //SDL_GetTicks();
        
        // do graphics drawing
        //SDL_FillRect(_surface_, nullptr, COLOR_BACKGROUND);
        
        //SDL_UpdateWindowSurface(_window_.get());

        // clear screen
        SDL_Color COLOR_BACKGROUND = _color_palette_.Get(ColorType::BACKGROUND);
        SDL_SetRenderDrawColor(_renderer_, COLOR_BACKGROUND.r, COLOR_BACKGROUND.g, COLOR_BACKGROUND.b, COLOR_BACKGROUND.a);
        SDL_RenderClear(_renderer_);

        // draw the text buffer contents
        //draw_buffer_contents();
        _textbox_ptr_->Draw(_renderer_, _timer_);
    
        // draw the label
        _status_label_->Draw(_renderer_, _timer_);  

        //Update screen
        SDL_RenderPresent(_renderer_);

    }

    bool _quit_;

    // TODO: make default variables as consts
    const int32_t _WIDTH_{600};
    const int32_t _HEIGHT_{400};

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window_;
    //std::unique_ptr<SDL_Surface> _surface_;
    SDL_Surface *_surface_;

    // text renderer
    SDL_Renderer *_renderer_ = nullptr; // TODO smart

    // globally used font
    //const TTF_Font *const _font_ = nullptr;

    // texture
    //SDL_Texture *_texture_;

    // TODO why do I need these?
    //unsigned short _texture_width_;
    //unsigned short _texture_height_; // think short is ok?

    //const std::string _texture_chars_{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "};
    //std::string _texture_chars_;
    //std::map<const char, SDL_Texture*> _texture_chars_;
    //std::map<const char, SDL_Rect> _texture_chars_size_;

    //SDL_Window *_window_;
    //SDL_Surface *_surface_;

    // TODO: change back - this has been done because buffer now requires an argument
    //Buffer _buffer_;
    //Buffer *_buffer_ptr_;
    Textbox *_textbox_ptr_;
    Label *_status_label_;
    
    //Cursor _cursor_;
    // TODO: custom deleter

    Keyboard _keyboard_;
    KeyMap _keymap_; // TODO: what do I do with this now?

    //SDL_Color COLOR_BACKGROUND;
    //SDL_Color COLOR_CURSOR;
    //SDL_Color COLOR_TEXT_DEFAULT;
    //SDL_Color COLOR_CURRENT_LINE_BACKGROUND;
    ColorPalette _color_palette_; // TODO: this is also used by FontTextureManager, move elsewhere?


    Uint32 _timer_;
    Uint32 _refresh_delay_;


    // configuration options
    const Config &_config_;
    FontTextureManager *_ftm_;

    // Editor mode
    EditorMode _editor_mode_;


    // vector of all action keys
    std::vector<ActionKey*> akv;

    // vector of all action keys
    std::vector<std::pair<ActionKey*, EditorMode>> akv_editor_mode_specific;




};




////////////////////////////////////////////////////////////////////////////////
// EDITOR CHANGE MODES
////////////////////////////////////////////////////////////////////////////////

// define a function to call here, because this is a convenient place to put it
// until I find somewhere to move it
// TODO
void fc_enter_edit_mode(Window& current_window)
{
    if(current_window._editor_mode_ == EditorMode::NORMAL)
    {
        std::cout << "EDIT" << std::endl;
        current_window._editor_mode_ = EditorMode::EDIT;
    }
}

void fc_exit_edit_mode(Window& current_window)
{
    if(current_window._editor_mode_ == EditorMode::EDIT)
    {
        std::cout << "NORMAL" << std::endl;
        current_window._editor_mode_ = EditorMode::NORMAL;
    }
}


////////////////////////////////////////////////////////////////////////////////
// PROGRAM QUIT AND REQUEST QUIT
////////////////////////////////////////////////////////////////////////////////

// TODO: this won't work for multiple textboxes,
// in addition, current_textbox is accessable from current_window
void fc_quit_request(Window& current_window)
{
    std::cout << "QUIT" << std::endl;
    //quit_action
    // quit request action
    if(current_window._textbox_ptr_->NotSaved())
    {
        std::cout << "The buffer is not saved, cannot quit" << std::endl;
        std::cout << "CTRL+SHIFT+Q to quit anyway" << std::endl;
        // TODO: message above should be a variable, as user may
        // change key combination
        // TODO: better interactive error message here
    }
    else
    {
        current_window._quit_ = true;
    }
}


// CTRL + SHIFT + Q -> immediate quit, without save
void fc_quit_force(Window& current_window)
{
    std::cout << "FORCE QUIT" << std::endl;
    current_window._quit_ = true;
}



////////////////////////////////////////////////////////////////////////////////
// BUFFER OPEN / SAVE
////////////////////////////////////////////////////////////////////////////////

// CTRL-S: save action
void fc_save(Window& current_window)
{
    //save_action
    current_window._textbox_ptr_->Save("buffer.txt");
    std::cout << "File " << "buffer.txt" << " written, " << current_window._textbox_ptr_->Size() << " bytes" << std::endl;
}


// CTRL-O: open action
void fc_open(Window& current_window)
{
    //open_action
    if(current_window._textbox_ptr_->NotSaved())
    {
        std::cout << "The buffer is not saved, cannot open" << std::endl;
    }
    else
    {
        // reset cursor position
        current_window._textbox_ptr_->ResetCursor();

        current_window._textbox_ptr_->Open("buffer.txt");
        std::cout << "File " << "buffer.txt" << " read, " << current_window._textbox_ptr_->Size() << " bytes" << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
// DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_print_buffer(Window& current_window)
{
    std::cout << current_window._textbox_ptr_->Get() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// MANUAL SCROLL LINE AND SCROLL SUB LINE, DEBUG
////////////////////////////////////////////////////////////////////////////////

void fc_scroll_inc(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollDown();
}

void fc_scroll_dec(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollUp();
}

void fc_scroll_inc_sub(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollDownSub();
}

void fc_scroll_dec_sub(Window& current_window)
{
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.ScrollUpSub();
}


////////////////////////////////////////////////////////////////////////////////
// up down left right arrow keys
////////////////////////////////////////////////////////////////////////////////

void fc_up(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorUp();
}

void fc_down(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorDown();
}

void fc_left(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorLeft();
}

void fc_right(Window& current_window)
{ 
    Textbox &textbox{*current_window._textbox_ptr_};
    textbox.CursorRight();
}


#endif
