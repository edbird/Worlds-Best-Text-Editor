#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>


#include "Cursor.hpp"


class BufferLine
{
    
    public:

    const std::string& GetString() const
    {
        return _text_;
    }

    void InsertChar(const char ch, CursorPos_t position)
    {
        // decrement position as values run from 1
        -- position;
        std::cout << "position=" << position << std::endl;
        std::cout << "_text_.size()=" << _text_.size() << std::endl;
        
        std::cout << "_text_=" << _text_ << std::endl;
        // TODO: what values of ch are allowed?
        // TODO: what about out of range error: position argument
        //_text_.insert(_text_.begin() + position, ch);
    }

    private:

    std::string _text_;

};

#define DEBUG 1

class Buffer
{

    public:

    Buffer()
        : _line_count_{0}
        , _modified_{true} // ensure first call to create_data works
    {
        // line at index 0 is always blank - there is no line 0 in
        // text editors
        _line_size_.push_back(0);
        BufferLine blank_line;
        _line_.push_back(blank_line); // this blank line is line 0 and does not do anything
        _line_.push_back(blank_line); // start with a blank line
    }

    // get reference 
    const std::string& Get()
    {
        // check done in create_data()
        //if(_modified == true)
        //{
        create_data();
        //}
        return _data_;
    }

    Cursor& MutableCursor()
    {
        return _cursor_;
    }

    void InsertAtCursor(const char ch)
    {
        _modified_ = true;

        // current line and col
        CursorPos_t c_line{_cursor_.GetPosLine()};
        CursorPos_t c_col{_cursor_.GetPosCol()};

        // use at() here in case we did something wrong
        // when updating the cursor
        #if DEBUG
            _line_.at(c_line).InsertChar(ch, c_col);
        #else
            _line_[c_line].InsertChar(ch, c_col);
        #endif

        // TODO: not all chars increment?
        // incrementing is done by the sdl event loop
    }

    private:

    // compile lines into complete buffer object inside std::string
    void create_data()
    {
        if(_modified_ == true)
        {
            _data_.clear();
            for(CursorPos_t line{1}; line < _line_count_; ++ line)
            {
                _data_ += _line_[line].GetString();
                // add new lines except for last line
                if(line < _line_count_ - 1)
                {
                    _data_ += _new_line_string_;
                }
            }
        }
        _modified_ = false;
    }

    Cursor _cursor_;

    CursorPos_t _line_count_; // not maintained yet
    std::vector<CursorPos_t> _line_size_; // not maintained yet
    std::vector<BufferLine> _line_;

    // "raw buffer" data emulation
    std::string _data_;

    // if buffer is modified, the create_data() function must be called
    // before Get()
    bool _modified_;

    // const
    const std::string _new_line_string_{std::string("\n")};
};


class ActionBuffer : Buffer
{
    // TODO: the "actions" are implemented here
    // eg; left key press should move the _cursor_ inside of buffer
};


#endif // BUFFER_HPP
