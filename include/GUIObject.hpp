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

    private:

    int _size_x_;
    int _size_y_;

    int _pos_x_;
    int _pos_y_;

};


#endif // GUIOBJECT_HPP
