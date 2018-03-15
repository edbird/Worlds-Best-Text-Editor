#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>


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


// TODO: seperate size and drawing code to new class BufferWindow
class Buffer
{

    public:

    Buffer(const Config& config, const std::map<const char, SDL_Rect>& _texture_chars_size_)
        //: _line_count_{0}
        : _config_{config}
        , _size_x_{300}
        , _size_y_{300}
        , _modified_{true} // ensure first call to create_data works
        , _not_saved_{false} // do not save a new buffer
        , _line_text_first_line_{0}
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
        
        int size_x{_texture_chars_size_.at(' ').w};
        int size_y{_texture_chars_size_.at(' ').h};
        _cursor_.reset(new Cursor(size_x, size_y, 2 * size_x, 0, config));

        // TODO: now NO zeroth line! we move the cursor index by -1
        // TODO: NO LONGER MOVE BY MINUS 1, SIMPLY ADJUST LINE NUMBERS

        // TODO: implement
        _cursor_->SetPos(0, 0);  // the cursor is always drawn in the location where the next
                                // character will be inserted: the buffer starts with zero
                                // size however the cursor will be drawn at position 0
                                // even though the buffer is not of size 1
    }
    
    ~Buffer()
    {
        //delete _cursor_;
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

    // this open method overwrites the buffer
    // and sets the _not_saved_ flag to FALSE
    void Open(const std::string& filename)
    {
        // TODO: maximum memory useage (virtual machine memory)
        // clear old contents
        _line_text_.clear();

        std::ifstream ifs(filename, std::ios::in | std::ios::ate);
        std::streampos fsize{ifs.tellg()};
        //std::cout << "file size is " << fsize << ". read" << std::endl;
        ifs.seekg(std::ios::beg);
        char * const buf{new char[fsize + 1]};
        char * buf_p0{buf}; // points to start of line
        char * buf_p1{buf}; // points to 1 char beyond end of line
        ifs.read(buf, fsize);
        std::cout << buf << std::endl;
        buf[fsize] = '\0';
        ifs.close();
        // TODO: use an iterator not ix!
        //for(std::streampos ix{0}; ix < fsize; ++ ix)
        for(;;)
        {
            // only store printable chars
            if((*buf_p1 >= 0x20 && *buf_p1 <= 0x7E) || *buf_p1 == '\t') // printable characters (valid)
            {
                ++ buf_p1;
            }
            else if(*buf_p1 == 0x0A || *buf_p1 == 0x0D) // CRLF characters (valid)
            {
                // push back new line
                std::string line(buf_p0, buf_p1);
                std::cout << "found new line: line is line=" << line << " size=" << line.size() << std::endl;
                _line_text_.push_back(line);

                ++ buf_p1;

                // check next character is not one of 0x0A, 0x0D
                //++ ix;
                if(*buf_p1 == 0x0A || *buf_p1 == 0x0D)
                {
                    ++ buf_p1;
                }
                
                // reset the p0 pointer
                buf_p0 = buf_p1;
            }
            else if(*buf_p1 == '\0')
            {
                // push back new line
                // this might be a zero terminated file or it might have a
                // new line before the zero terminator
                if(buf_p0 != buf_p1)
                {
                    std::string line(buf_p0, buf_p1);
                    std::cout << "found 0: line is line=" << line << " size=" << line.size() << std::endl;
                    _line_text_.push_back(line);
                }

                //++ buf_p1;
                // abort
                break;
            }
            else
            {
                // some other character was found which is invalid
                // abort
                std::cerr << "Error opening file " << filename << "\nData corruption or invalid file type detected\nPosition: " << buf_p1 - buf << std::endl;
                break; // abort immediate
            }
        }
        delete [] buf;

        // just read from file therefore file is already "saved"
        _not_saved_ = false;
        _modified_ = true; // without this there is a crash TODO why
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
        return _cursor_->GetPosLine();
    }

    Cursor::CursorPos_t GetCursorCol() const
    {
        return _cursor_->GetPosCol();
    }

    // TODO: should the buffer be responsible for setting the cursor
    // position or should the cursor be responsible for setting its own
    // bounds ?
    void CursorLeft()
    {
        //_cursor_->Left();
        //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
        //if(_col_ > 0)
        if(_cursor_->GetPosCol() > 0)
            _cursor_->Left();
    }

    void CursorRight()
    {
        // TODO DEBUG
        //if(_col_ < line_size)
        if(_cursor_->GetPosCol() < _line_text_.at(_cursor_->GetPosLine()).size())
            _cursor_->Right(); // TODO
        //_cursor_.RememberPosCol(); // TODO: can be done by call to left / right
    }

    // TODO: remember target line position
    // TODO: config: set rememberlineposition
    void CursorUp()
    {
        if(_cursor_->GetPosLine() > 0)
        {
            std::size_t _line_size_{_line_text_.at(_cursor_->GetPosLine() - 1).size()};
            Cursor::CursorPos_t _cursor_pos_{_cursor_->GetPosCol()};
            Cursor::CursorPos_t _cursor_pos_target_{_cursor_->GetTargetCol()};
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
            _cursor_->SetPos(_cursor_->GetPosLine() - 1, _cursor_pos_);
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
        if(_cursor_->GetPosLine() < _line_text_.size() - 1)
        {
            std::cout << "first if" << std::endl;
            std::size_t _line_size_{_line_text_.at(_cursor_->GetPosLine() + 1).size()};
            Cursor::CursorPos_t _cursor_pos_{_cursor_->GetPosCol()};
            Cursor::CursorPos_t _cursor_pos_target_{_cursor_->GetTargetCol()};
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
            _cursor_->SetPos(_cursor_->GetPosLine() + 1, _cursor_pos_);
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
        _cursor_->CR();
    }

    void InsertAtCursor(const char ch)
    {
        _modified_ = true;
        _not_saved_ = true;

        // current line and col
        Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};

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
        Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};

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
        Cursor::CursorPos_t c_line{_cursor_->GetPosLine()};
        Cursor::CursorPos_t c_col{_cursor_->GetPosCol()};

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
            Cursor::CursorPos_t goto_line{_cursor_->GetPosLine() - 1};
            _cursor_->SetPos(goto_line, _line_text_.at(goto_line).size());
        }
        return false;
    }


    //void ResetCursorLastBlinkTime();


    private:
    // check first line can be drawn (enough space in y direction)
    bool check_space_y(const SDL_Rect& dst_rect)
    {
        return (dst_rect.y + dst_rect.h <= _size_y_);
    }

    // print char
    // TODO: optimize this, flag might not be required
    bool print_char_texture(const SDL_Texture* const texture, const SDL_Rect& src_rect, const SDL_Rect& dst_rect)
    {

        // if this flag is true then print the character
        bool print_char_flag = true;
        
        // check if there is room to print character
        if(dst_rect.x + dst_rect.w >= _size_x_)
        {
            // TODO: check space y often occurs just after dst_rect.y
            // is changed so make function

            // TODO: ERROR IF h CHANGES!!!
            dst_rect.y += dst_rect.h;
            dst_rect.x = 0;
            dst_rect.x = dst_rect_origin_x; // this is only needed here because the line is split across multiple lines

            // check first line can be drawn (enough space in y direction)
            if(dst_rect.y + dst_rect.h <= _size_y_)
            {
                //print_char_flag = true;
            }
            else
            {
                // TODO: should clean up before return?
                // can I just "return" here?
                //break; // TODO
                print_char_flag = false;
                return false;
            }

            /*
            if(cursor_line == current_line)
            {
                if(current_col < cursor_col)
                {
                    // advance position of where cursor is to be drawn
                    cursor_texture_dst_rect.x = 0;
                    cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount
                    cursor_texture_dst_rect.y += dst_rect.h;
                }
            }
            // added to fix bug where cursor does not appear in correct place
            // when line is wrapped 2018-02-28
            else if(cursor_line > current_line)
            {
                cursor_texture_dst_rect.y += dst_rect.h;
            }
            */
        }

        if(print_char_flag == true)
        {
            // can draw, so continue
    
            // render texture to screen
            SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);

            //dst_rect.x += dst_rect.w;
            dst_rect.x += src_rect.w;
        }

    }

    
    public:
    // TODO: the buffer object should know its own WIDTH and HEIGHT
    void Draw(SDL_Renderer *const renderer, const std::map<const char, SDL_Texture*>& texture_chars, const std::map<const char, SDL_Rect>& texture_chars_size,  const Uint32 _timer_)
    {
        ////////////////////////////////////////////////////////////////////////////
        // DRAW BUFFER TEXT
        ////////////////////////////////////////////////////////////////////////////

        // TODO: remove pseudovariables
        SDL_Renderer *_renderer_{renderer};
        Buffer &_buffer_{*this};
        const std::map<const char, SDL_Texture*>& _texture_chars_{texture_chars};
        const std::map<const char, SDL_Rect>& _texture_chars_size_{texture_chars_size};

        bool line_number_enabled{false};
        int line_number_width{0};
        // Note: only 1 is true, any other integer is false
        if(_config_.GetInt("linenumber") == 1)
        {
            line_number_enabled = true;

            // set line number character count
            int line_count{_buffer_.GetLineCount()};
            for(;;)
            {
                ++ line_number_width;
                line_count = line_count / 10;
                if(line_count > 0)
                {
                    continue;
                }
                else
                {
                    break;
                    // line_number_char_count contains correct value
                    // line_count is invalid
                }
            }
        }


        // Draw line numbers
        // Create vector of line numbers
        //std::vector<std::string> line_number_str_vec;
        //const int line_count{_buffer_.GetLineCount()};
        //SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //for(int line{1}; line < line_count; ++ line)
        //{
        //    std::ostringstream ss;
        //    ss << std::setw(line_number_char_count) << std::setfill('0') << line;
        //    std::string line_number_str{ss.str()};
        //    //line_number_str_vec.push_back(line_number_str);
        //    std::string::const_iterator it{line_number_str.cbegin()};
        //    for(; it != line_number_str.cend(); ++ it)
        //    {
        //        char ch{*it};
        //        SDL_Texture *texture{_texture_chars_.at(ch)};
        //        SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
        //        dst_rect.w = src_rect.w;
        //        dst_rect.h = src_rect.h;
        //        SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);
        //        dst_rect.x += src_rect.w;
        //
        //    }
        //    dst_rect.y += dst_rect.h;
        //    dst_rect.x = 0;
        //}

        // Set rendering space and render to screen
        // size of individual characters
        // position set to origin of screen and character 'a' (first
        // character in the character string)
        int dst_rect_origin_x{0};
        // Move dst_rect_origin_x if line numbers are enabled
        if(line_number_enabled == true)
        {
            dst_rect_origin_x += line_number_width * _texture_chars_size_.at(' ').w;
        }

        int line_number{1};
        
        // Initialize destination rect for character printing
        SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //dst_rect = {0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //dst_rect.x = dst_rect_origin_x;
        //SDL_Rect src_rect{0, 0, _texture_width_ / _texture_chars_.size(), _texture_height_};

        // Print line number zero
        print_line_number(line_number, line_number_width, dst_rect, _renderer_, _texture_chars_, _texture_chars_size_);
        //dst_rect.x = dst_rect_origin_x; // move dst_rect to correct startpoint


        //std::cout << "texture_chars: " << _texture_chars_ << " size=" << _texture_chars_.size() << std::endl;
        //std::cout << "src_rect.w=" << src_rect.w << std::endl;


        // cursor position
        //Cursor::CursorPos_t cursor_line{_buffer_.GetCursorLine()};
        //Cursor::CursorPos_t cursor_col{_buffer_.GetCursorCol()};
        //Cursor::CursorPos_t current_line{0};
        //Cursor::CursorPos_t current_col{0};

        //std::cout << "cursor_line=" << cursor_line << " cursor_col=" << cursor_col << std::endl;

        // TODO: rather than using this we should be using the cursor pos variables
        // or add new variables to specify the position of the cursor on the screen
        // as well as the position of the cursor in the buffer
        //SDL_Rect cursor_texture_dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount

        SDL_Rect src_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        SDL_Rect dst_rect{0, 0, src_rect.w, src_rect.h};

        // check first line can be drawn (enough space in y direction)
        bool draw_buffer_flag{true};
        if(check_space_y(dst_rect))
        {
            // can draw, so continue
        //}
        //else
        //{
            // TODO: should clean up before return?
            // can I just "return" here?
            //draw_buffer_flag = false;
        //}



            // redesign to process line by line

            // iterate over lines
            std::size_t line_ix{_line_text_first_line_};
            std::size_t max_lines{_height_ / dst_rect.h}; // TODO
            for(; line_ix < _line_text_first_line_ + max_lines; ++ line_ix)
            {

                // iterate over characters in line
                std::size_t char_ix{0};
                for(; char_ix < _line_text_.at(line_ix).size(); ++ char_ix)
                {
                    const char ch{_line_text_.at(line_ix).at(char_ix)};
                
                    // source rect origin always 0, 0
                    // set the width and height on a per character basis using the map _texture_chars_size_
                    //SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
                    // set the dst_rect to have same size
                    //dst_rect.w = src_rect.w;
                    //dst_rect.h = src_rect.h;

                    // set the texture pointer
                    SDL_Texture *texture{_texture_chars_.at(ch)};
                    print_char_texture(texture, src_rect, dst_rect

                    //Render current frame
                    /*
                    if(dst_rect.x + dst_rect.w >= _size_x_)
                    {
                        // TODO: check space y often occurs just after dst_rect.y
                        // is changed so make function

                        // TODO: ERROR IF h CHANGES!!!
                        dst_rect.y += dst_rect.h;
                        dst_rect.x = 0;
                        dst_rect.x = dst_rect_origin_x; // this is only needed here because the line is split across multiple lines

                        // check first line can be drawn (enough space in y direction)
                        if(check_space_y(dst_rect))
                        {
                            // can draw, so continue
                        }
                        else
                        {
                            // TODO: should clean up before return?
                            // can I just "return" here?
                            break; // TODO
                        }

                        /*
                        if(cursor_line == current_line)
                        {
                            if(current_col < cursor_col)
                            {
                                // advance position of where cursor is to be drawn
                                cursor_texture_dst_rect.x = 0;
                                cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount
                                cursor_texture_dst_rect.y += dst_rect.h;
                            }
                        }
                        // added to fix bug where cursor does not appear in correct place
                        // when line is wrapped 2018-02-28
                        else if(cursor_line > current_line)
                        {
                            cursor_texture_dst_rect.y += dst_rect.h;
                        }
                        */
                        /*
                    }

                    // render texture to screen
                    SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);

                    //dst_rect.x += dst_rect.w;
                    dst_rect.x += src_rect.w;
                    */

                    //std::cout << "current_line=" << current_line << std::endl;
                    //std::cout << "current_col=" << current_line << std::endl;
                    //std::cout << "cursor_line=" << cursor_line << std::endl;
                    /*
                    if(cursor_line == current_line)
                    {
                        if(current_col < cursor_col)
                        {
                            // advance position of where cursor is to be drawn
                            cursor_texture_dst_rect.x += src_rect.w;
                            //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << " " << cursor_texture_dst_rect.w << " " << cursor_texture_dst_rect.h << std::endl;
                    
                        }
                    }
                    */

                    
                    //++ current_col;
                
                }

                // new line
                if(line_ix - 1 < _line_text_first_line_ + max_lines)
                {
                    
                    // TODO: ERROR IF h CHANGES!!!
                    dst_rect.y += dst_rect.h;
                    dst_rect.x = 0;
                    //dst_rect.x = dst_rect_origin_x;
                    
                    // check line can be drawn (enough space in y direction)
                    if(check_space_y(dst_rect))
                    {

                    }
                    else
                    {
                        // TODO
                        break;
                    }

                    ++ line_number;
                    print_line_number(line_number, line_number_width, dst_rect, _renderer_, _texture_chars_, _texture_chars_size_);
                    //dst_rect.x = dst_rect_origin.x; // below

                    //current_col = 0;
                    //++ current_line;

                    // only advance cursor y position if the cursor line
                    // is below the current line!
                    // without this the cursor ALWAYS sits on the final line
                    //if(cursor_line >= current_line)
                    //    cursor_texture_dst_rect.y += dst_rect.h;

                }


            }
        }

        
        /*
        std::string::const_iterator it{_buffer_.Get().cbegin()};
        //for(; it != _buffer_.Get().cend(); ++ it)
        for(; draw_buffer_flag == true && it != _buffer_.Get().cend(); ++ it)
        {

            //std::cout << "line=" << current_line << " col=" << current_col << std::endl;

            // print char if not at end
            const char ch{*it};
             
            if(ch == '\n')
            {

                // TODO: ERROR IF h CHANGES!!!
                dst_rect.y += dst_rect.h;
                dst_rect.x = 0;
                //dst_rect.x = dst_rect_origin_x;
                
                // check line can be drawn (enough space in y direction)
                if(check_space_y(dst_rect))
                {

                }
                else
                {
                    break;
                }

                ++ line_number;
                print_line_number(line_number, line_number_width, dst_rect, _renderer_, _texture_chars_, _texture_chars_size_);
                //dst_rect.x = dst_rect_origin.x; // below

                current_col = 0;
                ++ current_line;

                // only advance cursor y position if the cursor line
                // is below the current line!
                // without this the cursor ALWAYS sits on the final line
                if(cursor_line >= current_line)
                    cursor_texture_dst_rect.y += dst_rect.h;

            }
            else
            {
                
                // source rect origin always 0, 0
                // set the width and height on a per character basis using the map _texture_chars_size_
                SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
                // set the dst_rect to have same size
                dst_rect.w = src_rect.w;
                dst_rect.h = src_rect.h;
                // set the texture pointer
                SDL_Texture *texture{_texture_chars_.at(ch)};

                //std::cout << "ch=" << (int)ch << " offset=" << (int)(ch - _texture_chars_.at(0)) * src_rect.w << std::endl;
                //src_rect.x = (ch - _texture_chars_.at(0)) * src_rect.w;

                //Render current frame
                if(dst_rect.x + dst_rect.w >= _size_x_)
                {
                    // TODO: check space y often occurs just after dst_rect.y
                    // is changed

                    // TODO: ERROR IF h CHANGES!!!
                    dst_rect.y += dst_rect.h;
                    dst_rect.x = 0;
                    dst_rect.x = dst_rect_origin_x; // this is only needed here because the line is split across multiple lines

                    // check first line can be drawn (enough space in y direction)
                    if(check_space_y(dst_rect))
                    {
                        // can draw, so continue
                    }
                    else
                    {
                        // TODO: should clean up before return?
                        // can I just "return" here?
                        break;
                    }

                    if(cursor_line == current_line)
                    {
                        if(current_col < cursor_col)
                        {
                            // advance position of where cursor is to be drawn
                            cursor_texture_dst_rect.x = 0;
                            cursor_texture_dst_rect.x = dst_rect_origin_x; // shift the cursor dst rect by the same ammount
                            cursor_texture_dst_rect.y += dst_rect.h;
                        }
                    }
                    // added to fix bug where cursor does not appear in correct place
                    // when line is wrapped 2018-02-28
                    else if(cursor_line > current_line)
                    {
                        cursor_texture_dst_rect.y += dst_rect.h;
                    }
                }

                // render texture to screen
                SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);

                //dst_rect.x += dst_rect.w;
                dst_rect.x += src_rect.w;

                //std::cout << "current_line=" << current_line << std::endl;
                //std::cout << "current_col=" << current_line << std::endl;
                //std::cout << "cursor_line=" << cursor_line << std::endl;
                if(cursor_line == current_line)
                {
                    if(current_col < cursor_col)
                    {
                        // advance position of where cursor is to be drawn
                        cursor_texture_dst_rect.x += src_rect.w;
                        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << " " << cursor_texture_dst_rect.w << " " << cursor_texture_dst_rect.h << std::endl;
                
                    }
                }

                
                ++ current_col;
            }


            //if(it == _buffer_.Get().cend())
            //    break;

        }
        */
        //std::cin.get();
        
        ////////////////////////////////////////////////////////////////////////
        // Print cursor
        ////////////////////////////////////////////////////////////////////////

        
        // print cursor
        //_current_cursor_ = 0;
        //SDL_RenderCopy(_renderer_, cursor_texture.at(current_cursor), &cursor_texture_src_rect, &cursor_texture_dst_rect);
        //SDL_Rect src_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        //cursor_texture_dst_rect.w = src_rect.w;
        //cursor_texture_dst_rect.h = src_rect.h;

        //cursor_texture_dst_rect = dst_rect;


        // make cursor blink (TODO: use config option)
        //int cursor_blink_rate{500}; // TODO
        //if(_timer_ / 500 % 2 == 0)
        //{
        //    std::cout << "0" << std::endl;
        //    SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0xFF, 0x00, 0xFF));
        //}
        //else if(_timer_ / 500 % 2 == 1)
        //{
        //    std::cout << "1" << std::endl;
        //    SDL_FillRect(cursor_surface_normal, nullptr, SDL_MapRGBA(cursor_surface_normal->format, 0x00, 0x80, 0x00, 0xA0));
        //}

        // TODO: bug when cursor goes past end of buffer drawing
        // TODO: cursor color
        // TODO: what to do when cursor is out of screen range?
        // TODO: MULTIPLE TEXTURES REQUIRED BECAUSE COLOR CHANGE!
        //SDL_Texture* _current_cursor_texture_ptr_{_cursor_texture_.at(_current_cursor_)};
        //if(_timer_ / 500 % 2 == 1)
        //{
            //_current_cursor_texture_ptr_ = _cursor_texture_.at(3); // TODO: FIX THIS SHOULD NOT BE A CONST!
            _cursor_->Draw(_renderer_, cursor_texture_dst_rect, _timer_);
        //}
        //else
        //{
            // TODO
        //}
        
        //std::cout << "PRINT" << std::endl;
        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << " " << cursor_texture_dst_rect.w << " " << cursor_texture_dst_rect.h << std::endl;
        //std::cout << src_rect.x << " " << src_rect.y << " " << src_rect.w << " " << src_rect.h << std::endl;
        //if(SDL_RenderCopy(_renderer_, _texture_chars_.at('x'), &src_rect, &cursor_texture_dst_rect) != 0)
        //if(SDL_RenderCopy(_renderer_, _current_cursor_texture_ptr_, &src_rect, &cursor_texture_dst_rect) != 0)
        //{
        //    std::cout << SDL_GetError() << std::endl;
        //}
        //std::cout << "Cursor Position: " << cursor_line << ":" << cursor_col << std::endl;
        //SDL_RenderCopy(_renderer_, _texture_chars_.at('x'), &src_rect, &dst_rect);
        //std::cout << cursor_texture_dst_rect.x << " " << cursor_texture_dst_rect.y << std::endl;


        // when buffer empty print cursor at end
        //if(_buffer_.Get().size() == 0)
        //{

        //}
    }

    private:
    
    ////////////////////////////////////////////////////////////////////////////
    // Rendering helper functions
    ////////////////////////////////////////////////////////////////////////////
    
    // Convert line number to string
    std::string line_number_to_string(const int line_number, const int line_number_width)
    {
        std::ostringstream ss;
        ss << std::setw(line_number_width) << std::setfill('0') << line_number;
        std::string line_number_str{ss.str()};
        return line_number_str;
    }
        

    // TODO: pass dst_rect by reference and modify within function
    // TODO: remove _texture_chars_ arguments
    void print_line_number(const int line_number, const int line_number_width, SDL_Rect &dst_rect, SDL_Renderer *const _renderer_, const std::map<const char, SDL_Texture*>& _texture_chars_, const std::map<const char, SDL_Rect>& _texture_chars_size_)
    {
        std::string line_number_str{line_number_to_string(line_number, line_number_width)};
        //SDL_Rect dst_rect{0, 0, _texture_chars_size_.at(' ').w, _texture_chars_size_.at(' ').h};
        std::string::const_iterator it{line_number_str.cbegin()};
        for(; it != line_number_str.cend(); ++ it)
        {
            char ch{*it};
            SDL_Texture *texture{_texture_chars_.at(ch)};
            SDL_Rect src_rect{0, 0, _texture_chars_size_.at(ch).w, _texture_chars_size_.at(ch).h};
            dst_rect.w = src_rect.w;
            dst_rect.h = src_rect.h;
            SDL_RenderCopy(_renderer_, texture, &src_rect, &dst_rect);
            dst_rect.x += src_rect.w;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////

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

    // reference to config
    const Config& _config_;

    // size of buffer on screen
    int _size_x_;
    int _size_y_;

    // TODO smart pointer
    std::unique_ptr<Cursor> _cursor_;

    // TODO: don't need this variable, or the one below
    //Cursor::CursorPos_t _line_count_; // not maintained yet
    //std::vector<Cursor::CursorPos_t> _line_size_; // not maintained yet
    //std::vector<BufferLine> _line_;
    std::vector<std::string> _line_text_;

    // first line of text to print
    std::size_t _line_text_first_line_;

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
