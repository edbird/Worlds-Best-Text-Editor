#include "GUIObject.hpp"

GUIObject::GUIObject()
    : _pos_x_{0}
    , _pos_y_{0}
    , _size_x_{0}
    , _size_y_{0}
    , _background_color_r_{_DEFAULT_BACKGROUND_COLOR_R_}
    , _background_color_g_{_DEFAULT_BACKGROUND_COLOR_G_}
    , _background_color_b_{_DEFAULT_BACKGROUND_COLOR_B_}
{
}

GUIObject::GUIObject(const int size_x, const int size_y)
    : _pos_x_{0}
    , _pos_y_{0}
    , _size_x_{size_x}
    , _size_y_{size_y}
    , _background_color_r_{_DEFAULT_BACKGROUND_COLOR_R_}
    , _background_color_g_{_DEFAULT_BACKGROUND_COLOR_G_}
    , _background_color_b_{_DEFAULT_BACKGROUND_COLOR_B_}
{
}

GUIObject::GUIObject(const int pos_x, const int pos_y, const int size_x, const int size_y)
    : _pos_x_{pos_x}
    , _pos_y_{pos_y}
    , _size_x_{size_x}
    , _size_y_{size_y}
    , _background_color_r_{_DEFAULT_BACKGROUND_COLOR_R_}
    , _background_color_g_{_DEFAULT_BACKGROUND_COLOR_G_}
    , _background_color_b_{_DEFAULT_BACKGROUND_COLOR_B_}
{
}

int GUIObject::PosX() const
{
    return _pos_x_;
}

int GUIObject::PosY() const
{
    return _pos_y_;
}

int GUIObject::Width() const
{
    return _size_x_;
}

int GUIObject::Height() const
{
    return _size_y_;
}

void GUIObject::SetPosition(const int pos_x, const int pos_y)
{
    std::cout << "SetPosition(" << pos_x << ", " << pos_y << ")" << std::endl;
    _pos_x_ = pos_x;
    _pos_y_ = pos_y;
}

void GUIObject::SetSize(const int size_x, const int size_y)
{
    _size_x_ = size_x;
    _size_y_ = size_y;
}

void GUIObject::SetWidth(const int size_x)
{
    _size_x_ = size_x;
}

void GUIObject::SetHeight(const int size_y)
{
    _size_y_ = size_y;
}

void GUIObject::SetBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b)
{
    _background_color_r_ = r;
    _background_color_g_ = g;
    _background_color_b_ = b;
}

/*
void GUIObject::TestFunc2() const
{
    std::cout << "GUIObject::TestFunc2()" << std::endl;
}
*/

