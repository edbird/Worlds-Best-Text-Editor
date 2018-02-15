#ifndef CURSOR_HPP
#define CURSOR_HPP


class Cursor
{
    
    public:

    typedef uint32_t CursorPos_t;


    Cursor()
        : _line_{0}
        , _col_{0}
        , _cursor_type_{0}
    {
        
    }

    // TODO: what if an invalid value is given?
    void SetPos(const CursorPos_t line, const CursorPos_t col)
    {
        _line_ = line;
        _col_ = col;
        Print();
    }

    CursorPos_t GetPosLine() const
    {
        return _line_;
    }

    CursorPos_t GetPosCol() const
    {
        return _col_;
    }

    void CR()
    {
        _col_ = 0;
    }

    void Up()
    {
        if(_line_ > 0)
            -- _line_;
        Print();
    }

    void Down(const CursorPos_t line_count)
    {
        // TODO
        if(_line_ < line_count - 1)
            ++ _line_;
        Print();
    }

    void Left()
    {
        if(_col_ > 0)
            -- _col_;
        _remember_col_ = _col_;
        Print();
    }

    void Right(const CursorPos_t line_size)
    {
        // TODO
        if(_col_ < line_size) // cursor can scroll 1 char past the end!
            ++ _col_;
        _remember_col_ = _col_;
        Print();
    }

    void Print() const
    {
        std::cout << _line_ << ',' << _col_ << std::endl;
    }

    CursorPos_t GetTargetCol() const
    {
        return _remember_col_;
    }

    int GetCursorType() const
    {
        return _cursor_type_;
    }

    private:

    CursorPos_t _line_;
    CursorPos_t _col_;

    // remembers the "target" cursor pos col
    CursorPos_t _remember_col_;

    // cursor type (drawing flag)
    int _cursor_type_;


};

#endif // CURSOR_HPP
