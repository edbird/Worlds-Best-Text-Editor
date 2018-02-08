
#include "SDL.h"

#include <memory>

#include <iostream>
#include <fstream>

#include <cstdint>

#include <cmath>



#include "Buffer.hpp"
#include "Cursor.hpp"
#include "KeyMap.hpp"



class Window
{

    public:

    Window()
        : _window_(nullptr, SDL_DestroyWindow)
        , _surface_{nullptr}
    {
        // TODO: should go elsewhere?
        // TODO: error message
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
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
                SDL_FillRect(_surface_, nullptr, SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF));
                //SDL_FillRect(_surface_, nullptr, SDL_MapRGB(_surface_->format, 0xFF, 0xFF, 0xFF));

                SDL_UpdateWindowSurface(_window_.get());
                //SDL_UpdateWindowSurface(_window_);

                SDL_Delay(1000);
            }
        }
    }

    ~Window()
    {
        //SDL_DestroyWindow(_window_.get());
        //SDL_DestroyWindow(_window_);

        SDL_Quit();
    }

    int run()
    {
        
        bool quit{false};
        for(;;)
        {
            
            // poll events
            SDL_Event event;

            while(SDL_PollEvent(&event) != 0)
            {
                // user request quit
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }

                // keypress events
                else if(event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                        // TODO: mappings


                        case SDLK_UP:
                            _buffer_.MutableCursor().Up();        
                            break;


                        case SDLK_DOWN:
                            _buffer_.MutableCursor().Down();
                            break;


                        case SDLK_LEFT:
                            _buffer_.MutableCursor().Left();
                            break;


                        case SDLK_RIGHT:
                            _buffer_.MutableCursor().Right();
                            break;
           

                        case SDLK_a:
                            _buffer_.InsertAtCursor('a');
                            _buffer_.MutableCursor().Right();
                            break;

                        default:
                            std::cerr << "Key: " << event.key.keysym.sym << " is not handled!" << std::endl;
                            break;
                    }

                    // print buffer for debug
                    std::cout << "buffer contents:" << std::endl;
                    std::cout << _buffer_.Get() << std::endl;
                }
            }


            if(quit == true) break;
        }

        return 0;
    }


    private:

    const int32_t _WIDTH_{600};
    const int32_t _HEIGHT_{400};

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window_;
    //std::unique_ptr<SDL_Surface> _surface_;
    SDL_Surface* _surface_;

    //SDL_Window *_window_;
    //SDL_Surface *_surface_;

    Buffer _buffer_;
    //Cursor _cursor_;

};


// TODO: implement application


int main(int argc, char* argv[])
{

    Window window;
    window.run();

    return 0;
}
