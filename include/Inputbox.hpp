#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP


#include "Keyboard.hpp"
#include "Actions.hpp"
#include "FunctionCallback.hpp"
#include "Label.hpp"




#include <vector>


// a single line input box
class Inputbox : public Label
{


    public:

    Inputbox()
        : Label()
    {
    }

    virtual ~Inputbox()
    {
    }



    virtual void ProcessEvent(Window& current_window) override
    {
        std::cout << "Inputbox::ProcessEvent()" << std::endl;

    }

    virtual void Draw() const override
    {
        std::cout << "Inputbox::Draw()" << std::endl;
    }

    /*virtual*/ void TestFunc() const //override
    {
        std::cout << "Inputbox::TestFunc()" << std::endl;
    }

    void TestFunc2() const
    {
        std::cout << "Inputbox::TestFunc2()" << std::endl;
    }


};


#endif
