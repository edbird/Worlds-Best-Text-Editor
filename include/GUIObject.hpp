#ifndef GUIOBJECT_HPP
#define GUIOBJECT_HPP


#include "Keyboard.hpp"
#include "Actions.hpp"


#include <SDL.h>

class Window;

class GUIObject
{

    public:

    GUIObject();
    GUIObject(const int, const int);
    GUIObject(const int, const int, const int, const int);
    virtual ~GUIObject()
    {
    }

    virtual void ProcessEvent(Window& current_window, const SDL_Event& event, const Keyboard& keyboard, /*const CurrentKeyboardAction& ka_current,*/ const Uint32 timer) = 0;

    virtual void Draw(SDL_Renderer *const renderer, const Uint32 timer) const = 0;

    virtual void TestFunc() const = 0;

    virtual int PosX() const;
    virtual int PosY() const;
    virtual int Width() const;
    virtual int Height() const;

    virtual void SetPosition(const int, const int);
    virtual void SetSize(const int, const int);
    virtual void SetWidth(const int);
    virtual void SetHeight(const int);

    // TODO: virtual?
    void SetBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b);


    private:

    ////////////////////////////////////////////////////////////////////////////
    // SIZE AND POSITION
    ////////////////////////////////////////////////////////////////////////////

    int _size_x_;
    int _size_y_;

    int _pos_x_;
    int _pos_y_;



    ////////////////////////////////////////////////////////////////////////////
    // BACKGROUND COLOR
    ////////////////////////////////////////////////////////////////////////////
    protected:

    Uint8 _DEFAULT_BACKGROUND_COLOR_R_{0xCC};
    Uint8 _DEFAULT_BACKGROUND_COLOR_G_{0xCC};
    Uint8 _DEFAULT_BACKGROUND_COLOR_B_{0xCC};
    Uint8 _background_color_r_;
    Uint8 _background_color_g_;
    Uint8 _background_color_b_;

};


#endif // GUIOBJECT_HPP
