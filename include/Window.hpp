#ifndef WINDOW_HPP
#define WINDOW_HPP

// TODO; remove
#include "Inputbox.hpp"


#include "FunctionCallback.hpp"
#include "Textbox.hpp"
#include "Label.hpp"



#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <map>







class Window
{

    friend void fc_open(Window&);

    public:

    std::string TestName() const
    {
        return std::string("Window 0");
    }

    Window()
        : _quit_{false}
    {
            // TODO: remove
            Inputbox *box = new Inputbox();
            box->SetPosition(0, 0);
            box->TestFunc2();
            box->TestFunc();
            box->Draw();
            std::cin.get();

    }

    ~Window()
    {
    }
    

    private:

    
    bool _quit_;

};






#endif
