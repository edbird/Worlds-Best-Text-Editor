#ifndef GUIOBJECT_HPP
#define GUIOBJECT_HPP


#include <SDL.h>


class GUIObject
{

    public:

    GUIObject();
    GUIObject(const int, const int);
    GUIObject(const int, const int, const int, const int);

    virtual void Draw(SDL_Renderer *const renderer, const Uint32 timer) const = 0;

    virtual int PosX() const;
    virtual int PosY() const;
    virtual int Width() const;
    virtual int Height() const;

    virtual void SetPosition(const int, const int);
    virtual void SetSize(const int, const int);
    virtual void SetWidth(const int);
    virtual void SetHeight(const int);

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
