#ifndef GUITEXTOBJECT_HPP
#define GUITEXTOBJECT_HPP


#include "GUIObject.hpp"



class GUITextObject : public GUIObject
{


    public:

    GUITextObject(const FontTextureManager& ftm, const int width, const int height)
        : GUIObject(width, height)
        , _ftm_{ftm}
    {
    }

    virtual ~GUITextObject()
    {
    }


    protected:

    // TODO: remove this, should not require it for all subclasses
    // note: all text based classes inherit from this object, so ok here
    const FontTextureManager& _ftm_;

    private:

    // buffer
    // TODO: which type?
    FileBuffer _buffer_;


};


#endif // GUITEXTOBJECT_HPP