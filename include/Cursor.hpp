#ifndef CURSOR_HPP
#define CURSOR_HPP

typedef uint32_t CursorPos_t;

class Cursor
{

    public:

    Cursor()
        : _line_(1)
        , _col_(1)
    {
        
    }

    CursorPos_t GetPosLine() const
    {
        return _line_;
    }

    CursorPos_t GetPosCol() const
    {
        return _col_;
    }

    void Up()
    {
        if(_line_ > 1)
            -- _line_;
    }

    void Down()
    {
        if(_line_ < - 1)
            ++ _line_;
    }

    void Left()
    {
        if(_col_ > 1)
            -- _col_;
    }

    void Right()
    {
        if(_col_ < - 1)
            ++ _col_;
    }

    private:

    CursorPos_t _line_;
    CursorPos_t _col_;

};

#endif // CURSOR_HPP
