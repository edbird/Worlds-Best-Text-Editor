#ifndef GUITEXTENTRY_HPP
#define GUITEXTENTRY_HPP


#include "GUITextObject.hpp"
#include "Cursor.hpp"



class GUITextEntry : public GUITextObject
{

    public:

    GUITextEntry(const FontTextureManager& ftm, const int width, const int height)
        : GUITextObject(ftm, width, height)
    {

        //const std::map<const char, SDL_Texture*>& texture_chars{_ftm_.GetCharTexture()};
        const std::map<const char, SDL_Rect>& texture_chars_size{ftm.GetCharSize()};

        int size_x{texture_chars_size.at(' ').w};
        int size_y{texture_chars_size.at(' ').h};
        _cursor_.reset(new Cursor(size_x, size_y/*,*/ /*2 * size_x, 0,*/ /*config*/)); // TODO: config here
        
        // TODO: implement
        _cursor_->SetPos(0, 0); // the cursor is always drawn in the location where the next
                                // character will be inserted: the buffer starts with zero
                                // size however the cursor will be drawn at position 0
                                // even though the buffer is not of size 1
    }

    virtual ~GUITextEntry()
    {
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // CURSOR FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////
    
    // used when opening new file
    void ResetCursor();

    const Cursor& GetCursor() const;

    Cursor& MutableCursor();

    // TODO: should the buffer be responsible for setting the cursor
    // position or should the cursor be responsible for setting its own
    // bounds ?
    void CursorLeft();
    void CursorRight();
    // TODO: remember target line position
    // TODO: config: set rememberlineposition
    void CursorUp();
    void CursorDown();

    //void CursorCR();
    
    void BackspaceAtCursor();

    void InsertAtCursor(const char ch);





    // get const buffer reference
    //const Buffer& GetBuffer() const
    //{
    //    return _buffer_;
    //}
    
    //Buffer& MutableBuffer()
    //{
    //    return _buffer_;
    //}


    private:


    // cursor
    std::unique_ptr<Cursor> _cursor_;
    //std::size_t _cursor_x_;
    //std::size_t _cursor_y_;

};


#endif // GUITEXTENTRY_HPP
