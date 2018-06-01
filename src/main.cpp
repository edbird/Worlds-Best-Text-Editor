

#include "Config.hpp"
#include "Window.hpp"


#include "program_arguments.hpp"


#include <cstdint>
#include <cmath>


// TODO: implement application


void print_help(std::ostream& os)
{
    os << "help" << std::endl;
}

int main(int argc, char* argv[])
{ 

    ////////////////////////////////////////////////////////////////////////////
    // PROCESS PROGRAM ARGUMENTS
    ////////////////////////////////////////////////////////////////////////////

    ProgramArguments pa;
    pa.Add("help", "--help", "false");
    pa.Add("filename", "--filename", "buffer.txt");
    pa.Print();
    pa.Process(argc, argv);

    std::string arg_help{pa.Get("help")};
    //std::string arg_filename{pa.Get("filename")};

    if(arg_help == std::string("true"))
    {
        print_help(std::cout);
    }


    ////////////////////////////////////////////////////////////////////////////
    // LOAD CONFIG
    ////////////////////////////////////////////////////////////////////////////

    Config config;
    const Config &_config_{config};


    ////////////////////////////////////////////////////////////////////////////
    // LOAD SDL
    ////////////////////////////////////////////////////////////////////////////

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
            
            // pass the font to the FontTextureManager
            //FontTextureManager ftm(_font_, color_palette);
            // window has a FontTextureManager object, should separate?

            // pass the font to the window
            Window window(config, _font_);


            /*
            std::cout << "ENTERED MAIN" << std::endl;
            Inputbox *box = new Inputbox(window._ftm_);
            box->SetPosition(0, 0);
            box->TestFunc2();
            box->TestFunc();
            box->Draw(window._renderer_, window._timer_);
            */

            if(pa.WasProvided("filename"))
            {
                window.OpenFile(pa.Get("filename"));
            }
            //fc_open(&window);
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
