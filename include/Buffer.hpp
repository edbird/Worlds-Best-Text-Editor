#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>
#include <fstream>


#include "Cursor.hpp"

/*
class BufferLine
{
    
    public:

    const std::string& GetString() const
    {
        return _text_;
    }

    void InsertChar(const char ch, Cursor::CursorPos_t position)
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
*/


#define DEBUG 1

class Buffer
{

    public:

    Buffer()
        //: _line_count_{0}
        : _modified_{true} // ensure first call to create_data works
        , _not_saved_{true}
    {
        std::string blank_string;
        _line_text_.push_back(blank_string); // this is left here to ensure first
        // call to insert works without having to insert a blank line

        // line at index 0 is always blank - there is no line 0 in
        // text editors
        //_line_size_.push_back(0);
        //BufferLine blank_line;
        //_line_.push_back(blank_line); // this blank line is line 0 and does not do anything
        //_line_.push_back(blank_line); // start with a blank line
        //_line_size_.push_back(0); // start with a blank line
        //_line_text_.push_back(blank_string);

        // TODO: now NO zeroth line! we move the cursor index by -1
        // TODO: NO LONGER MOVE BY MINUS 1, SIMPLY ADJUST LINE NUMBERS

        // TODO: implement
        _cursor_.SetPos(0, 0);  // the cursor is always drawn in the location where the next
                                // character will be inserted: the buffer starts with zero
                                // size however the cursor will be drawn at position 0
                                // even though the buffer is not of size 1
    }

    std::size_t Size() const
    {
        return Get().size();
    }

    // save to text file
    void Save(const std::string& filename) const
    {
        std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
        const std::string stream_data{Get()};
        ofs.write(stream_data.data(), stream_data.size());
        ofs.flush();
        ofs.close();

        // set the saved flag
        _not_saved_ = false;
    }

    // 
    bool NotSaved() const
    {
        return _not_saved_;
    }

    // is the buffer in the "default" state - contains nothing, empty
    bool IsEmpty() const
    {
        if(_line_text_.size() == 1)
        {
            if(_line_text_[0].size() == 0)
            {
                // contains only a single blank line
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(_line_text_.size() == 0)
        {
            throw "Error in IsEmpty()";
        }
        return false;
    }

    // get reference 
    const std::string& Get() const
    {
        // check done in create_data()
        //if(_modified == true)
        //{
        create_data();
        //}
        return _data_;
    }

    // get number of lines in buffer (line count)
    int GetLineCount() const
    {
        return _line_text_.size();
    }

    /*
    Cursor& MutableCursor()
    {
        return _cursor_;
    
    }
    */

    Cursor::CursorPos_t GetCursorLine() const
    {
        return _cursor_.GetPosLine();
    }

    Cursor::CursorPos_t GetCursorCol() const
    {
        return _cursor_.GetPosCol();
    }

    // TODO: should the buffer be responsible for setting the cursor
    // position or should the cursor be responsible for setting its own
    // bounds ?
    void CursorLeft()
    {
        _cursor_.Left();
        //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
    }

    void CursorRight()
    {
        // TODO DEBUG
        _cursor_.Right(_line_text_.at(_cursor_.GetPosLine()).size()); // TODO
        //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
    }

    // TODO: remember target line position
    // TODO: config: set rememberlineposition
    void CursorUp()
    {
        if(_cursor_.GetPosLine() > 0)
        {
            std::size_t _line_size_{_line_text_.at(_cursor_.GetPosLine() - 1).size()};
            Cursor::CursorPos_t _cursor_pos_{_cursor_.GetPosCol()};
            Cursor::CursorPos_t _cursor_pos_target_{_cursor_.GetTargetCol()};
            if(_cursor_pos_target_ > _line_size_)
            {
                // target position is too far along the line
                // as the line is too short!
                // check against the current cursor position
                // rather than the target cursor position
                // which is set whenever the user moves left / right
                if(_cursor_pos_ > _line_size_)
                {
                    _cursor_pos_ = _line_size_;
                }
                else
                {
                    // _cursor_pos_ has the correct value
                    // don't do anything
                }
            }
            else
            {
                // target position is ok:
                // set the cursor position to be the target position
                _cursor_pos_ = _cursor_pos_target_;
            }
            _cursor_.SetPos(_cursor_.GetPosLine() - 1, _cursor_pos_);
        }
        else
        {
            // _line_ is 0, do nothing
        }
        //_cursor_.Up();
    }

    void CursorDown()
    {
        std::cout << "cursor down" << std::endl;
        if(_cursor_.GetPosLine() < _line_text_.size() - 1)
        {
            std::cout << "first if" << std::endl;
            std::size_t _line_size_{_line_text_.at(_cursor_.GetPosLine() + 1).size()};
            Cursor::CursorPos_t _cursor_pos_{_cursor_.GetPosCol()};
            Cursor::CursorPos_t _cursor_pos_target_{_cursor_.GetTargetCol()};
            if(_cursor_pos_target_ > _line_size_)
            {
                // target position is too far along the line
                // as the line is too short!
                // check against the current cursor position
                // rather than the target cursor position
                // which is set whenever the user moves left / right
                if(_cursor_pos_ > _line_size_)
                {
                    _cursor_pos_ = _line_size_;
                }
                else
                {
                    // _cursor_pos_ has the correct value
                    // don't do anything
                }
            }
            else
            {
                // target position is ok:
                // set the cursor position to be the target position
                _cursor_pos_ = _cursor_pos_target_;
            }
            _cursor_.SetPos(_cursor_.GetPosLine() + 1, _cursor_pos_);
        }
        else
        {
            std::cout << "cannot go down" << std::endl;
            // _line_ is the maximum line, cannot go down, do nothing
        }

        //if(_cursor_.GetPosLine() < _line_text_.size())
        //{
        //_cursor_.Down(_line_text_.size()); // TODO
        //}
    }

    void CursorCR()
    {
        _cursor_.CR();
    }

    void InsertAtCursor(const char ch)
    {
        _modified_ = true;
        _not_saved_ = true;

        // current line and col
        Cursor::CursorPos_t c_line{_cursor_.GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_.GetPosCol()};

        //std::cout << "c_col=" << c_col << std::endl;
        // use at() here in case we did something wrong
        // when updating the cursor
        #if DEBUG
            //_line_.at(c_line).InsertChar(ch, c_col);
            _line_text_.at(c_line).insert(_line_text_.at(c_line).begin() + c_col, ch);
        #else
            //_line_[c_line].InsertChar(ch, c_col);
            _line_text_[c_line].insert(_line_text_[c_line].begin() + c_col, ch);
        #endif

        // TODO: not all chars increment?
        // incrementing is done by the sdl event loop
        //std::cout << "_line_text_.at(c_line)=" << _line_text_.at(c_line) << std::endl;
    }

    void ReturnAtCursor()
    {
        _modified_ = true;
        _not_saved_ = true;

        // current line and col
        Cursor::CursorPos_t c_line{_cursor_.GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_.GetPosCol()};

        //std::cout << "c_col=" << c_col << std::endl;
        // use at() here in case we did something wrong
        // when updating the cursor
        #if DEBUG
            //_line_.at(c_line).InsertChar(ch, c_col);
            //_line_text_.at(c_line).insert(_line_text_.at(c_line).begin() + c_col, '\n');
        #else
            //_line_[c_line].InsertChar(ch, c_col);
            //_line_text_[c_line].insert(_line_text_[c_line].begin() + c_col, '\n');
        #endif

        // push back a new line (string)
        _line_text_.push_back(std::string{});
    }

    bool BackspaceAtCursor()
    {
        _modified_ = true;
        _not_saved_ = true;

        // current line and col
        Cursor::CursorPos_t c_line{_cursor_.GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_.GetPosCol()};

        if(c_col > 0)
        {
            //-- c_col; // decrement to erase correct character

            // use at() here in case we did something wrong
            // when updating the cursor
            #if DEBUG
                //_line_.at(c_line).InsertChar(ch, c_col);
                _line_text_.at(c_line).erase(_line_text_.at(c_line).begin() + c_col - 1);
            #else
                //_line_[c_line].InsertChar(ch, c_col);
                _line_text_[c_line].erase(_line_text_[c_line].begin() + c_col - 1);
            #endif

            // TODO: not all chars increment?
            // incrementing is done by the sdl event loop
            //std::cout << "_line_text_.at(c_line)=" << _line_text_.at(c_line) << std::endl;
        
            // TODO: i suspect that the cursor does not get moved correctly!
            //if(c_col > 0)
            //{
                return true;
            //}
            //else
            //{
            //    return false;
            //}
        }
        else if(c_line > 0)
        {
            // c_col is zero
            // delete the c_line
            _line_text_.erase(_line_text_.begin() + c_line);
            //_cursor_.SetPos(, );
            //_cursor_.CR(); // TODO might be needed see below comment
            //_cursor_.Up(); // TODO this will not work if the lines are different lengths
            Cursor::CursorPos_t goto_line{_cursor_.GetPosLine() - 1};
            _cursor_.SetPos(goto_line, _line_text_.at(goto_line).size());
        }
        return false;
    }

    private:

    // compile lines into complete buffer object inside std::string
    void create_data() const
    {
        if(_modified_ == true)
        {
            //std::cout << "modified is true" << std::endl;
            _data_.clear();
            //for(Cursor::CursorPos_t line{1}; line < _line_count_; ++ line)
            for(Cursor::CursorPos_t line{0}; line < _line_text_.size(); ++ line)
            {
                //_data_ += _line_[line].GetString();
                _data_ += _line_text_[line];
                // add new lines except for last line
                //if(line < _line_count_ - 1)
                if(line < _line_text_.size() - 1)
                {
                    _data_ += _new_line_string_;
                }
            }
        }
        _modified_ = false;
    }

    Cursor _cursor_;

    // TODO: don't need this variable, or the one below
    //Cursor::CursorPos_t _line_count_; // not maintained yet
    //std::vector<Cursor::CursorPos_t> _line_size_; // not maintained yet
    //std::vector<BufferLine> _line_;
    std::vector<std::string> _line_text_;

    // "raw buffer" data emulation
    mutable std::string _data_;

    // if buffer is modified, the create_data() function must be called
    // before Get()
    mutable bool _modified_;

    // not saved flag
    // this flag is set to true when the buffer is modified but not saved
    // when the buffer is saved, this flag is set to false
    mutable bool _not_saved_;

    // const
    const std::string _new_line_string_{std::string("\n")};
};


class ActionBuffer : Buffer
{
    // TODO: the "actions" are implemented here
    // eg; left key press should move the _cursor_ inside of buffer
};


#endif // BUFFER_HPP
