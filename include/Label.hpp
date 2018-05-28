#ifndef LABEL_HPP
#define LABEL_HPP


#include "Functions.hpp"
#include "GUIObject.hpp"
#include "FunctionCallback.hpp"



#include <string>
#include <sstream>





// label
// can be used for debugging
class Label : public GUIObject
{


    public:

    Label()
        : GUIObject()
    {
        set_size();
    }

    
    Label(const std::string& text)
        : GUIObject()
        , _text_{text}
    {
        set_size();
    }

    virtual ~Label()
    {
    }

    

    void SetText(const std::string& text)
    {
        _text_ = text;

        set_size();
    }

    std::string GetText() const
    {
        return _text_;
    }


    private:

    // call after doing anything to object that changes the size,
    // at the moment, only changing the text contents changes the size
    // so this function must be called after the contents of _text_ is changed
    void set_size()
    {
    
        SetSize(0, 0);
    }

    public:


    ////////////////////////////////////////////////////////////////////////////
    // SET POSITION AND ANCHOR
    ////////////////////////////////////////////////////////////////////////////

    /*
    void SetPosition(const int pos_x, const int pos_y)
    {
        _pos_x_ = pos_x;
        _pos_y_ = pos_y;
    }
    */
    
    virtual void ProcessEvent(Window& current_window) override
    {
        // do nothing

        std::cout << "Label::ProcessEvent()" << std::endl;

        // TODO: make label blink ?
    }

    virtual void Draw() const override
    {

        std::cout << "Label::Draw()" << std::endl;
    }

    virtual void TestFunc() const override
    {
        std::cout << "Label::TestFunc()" << std::endl;
    }




    private:

    std::string _text_;
    //int _pos_x_;
    //int _pos_y_;


};


#endif LABEL_HPP // LABEL_HPP
