

#include "Window.hpp"




#include <cstdint>
#include <cmath>




int main(int argc, char* argv[])
{
                       
    Window window;
    
    // TODO: remove
    std::cout << "start tests from inputbox, in main" << std::endl;
    Inputbox *inputbox = new Inputbox();
    inputbox->SetPosition(0, 0);
    std::cout << "INPUTBOX..." << std::endl;
    inputbox->TestFunc2();
    inputbox->TestFunc();
    std::cout << "finished calling from inputbox, in main" << std::endl;
    std::cin.get();

    std::cout << "calling from main()" << std::endl;
    fc_open(window);

    return 0;
}
