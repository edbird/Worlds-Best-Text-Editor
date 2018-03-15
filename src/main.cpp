

#include "Config.hpp"
#include "Window.hpp"

#include <cstdint>
#include <cmath>


// TODO: implement application


int main(int argc, char* argv[])
{

    Config config;
    const Config &_config_{config};


    // font pointer
    TTF_Font *_font_ = nullptr;
    
    // TODO: should go elsewhere?
    // TODO: error message
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
    }
    else
    {
        // initialize SDL_ttf
        if(TTF_Init() < 0) // TODO check return codes
        {
            std::cout << TTF_GetError() << std::endl;
        }
        else
        {
        
            // open the font
            // TODO move elsewhere
            //TTF_Font *_font_ = nullptr;
            _font_ = TTF_OpenFont("/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf", _config_.GetInt("fontsize"));
            
            if(_font_ == nullptr)
            {
                std::cout << TTF_GetError() << std::endl;
            }
            else
            {
                
            }

            // pass the font to the window
            Window window(config, _font_);
            window.Run();

        }

    } // SDL_Init() failed


    // cleanup
    
    
    // close font
    TTF_CloseFont(_font_);

    TTF_Quit();
    SDL_Quit();


    return 0;
}
