#include "Buffer.hpp"

// TODO: seperate size and drawing code to new class Textbox
Buffer::Buffer() //const Config& config)
    //: _line_count_{0}
    //: _config_{config}
    //,
    : _modified_{true} // ensure first call to create_data works
    , _not_saved_{false} // do not save a new buffer
{
    //std::string blank_string;
    //_line_text_.push_back(blank_string); // this is left here to ensure first
    _line_text_.emplace_back(std::string());
    // call to insert works without having to insert a blank line


    // line numbers start at 1, cursor/text index starts at zero

}


Buffer::~Buffer()
{
    //delete _cursor_;
}


std::size_t Buffer::Size() const
{
    return Get().size();
}


// save to text file
void Buffer::Save(const std::string& filename) const
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
void Buffer::Open(const std::string& filename)
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
bool Buffer::NotSaved() const
{
    return _not_saved_;
}


// is the buffer in the "default" state - contains nothing, empty
bool Buffer::IsEmpty() const
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
const std::string& Buffer::Get() const
{
    // check done in create_data()
    //if(_modified == true)
    //{
    create_data();
    //}
    return _data_;
}


// get number of lines in buffer (line count)
std::size_t /*int*/ Buffer::GetLineCount() const
{
    return _line_text_.size();
}


const std::vector<std::string>& Buffer::GetContainer() const
{
    return _line_text_;
}

void Buffer::Insert(const char ch, const std::size_t line, const std::size_t col)
{
    _modified_ = true;
    _not_saved_ = true;
    
    _line_text_.at(line).insert(col, 1, ch);
    
    // use at() here in case we did something wrong
    // when updating the cursor
    //#if DEBUG
    //    //_line_.at(c_line).InsertChar(ch, c_col);
    //    _line_text_.at(c_line).insert(_line_text_.at(c_line).begin() + c_col, ch);
    //#else
    //    //_line_[c_line].InsertChar(ch, c_col);
    //    _line_text_[c_line].insert(_line_text_[c_line].begin() + c_col, ch);
    //#endif
}
    
// TODO: performance profile, this is probably extremely slow for large files
// when inserting new line near begining
// change vector to list?
void Buffer::InsertNewLine(const std::size_t line, const std::size_t col)
{
    _modified_ = true;
    _not_saved_ = true;
    
    
    std::string s{_line_text_.at(line).substr(col)};
    std::cout << "InsertNewLine, split line: remaining text is: " << s << std::endl;
    
    _line_text_.insert(_line_text_.cbegin() + line + 1, s);
    // push back a new line (string)
    //_line_text_.push_back(std::string{});
    
    // use at() here in case we did something wrong
    // when updating the cursor
    //#if DEBUG
    //    //_line_.at(c_line).InsertChar(ch, c_col);
    //    //_line_text_.at(c_line).insert(_line_text_.at(c_line).begin() + c_col, '\n');
    //#else
    //    //_line_[c_line].InsertChar(ch, c_col);
    //    //_line_text_[c_line].insert(_line_text_[c_line].begin() + c_col, '\n');
    //#endif
}
    
// bool: return true if something was deleted, in which case the cursor can
// move right, if false is returned nothing was deleted and the cursor should
// not be moved right
bool Buffer::Delete(const std::size_t line, const std::size_t col)
{
    //_modified_ = true;
    //_not_saved_ = true;

    if(col > 0)
    {
        //-- c_col; // decrement to erase correct character

        _modified_ = true;
        _not_saved_ = true;

        _line_text_.at(line).erase(col, 1);

        // use at() here in case we did something wrong
        // when updating the cursor
        //#if DEBUG
        //    //_line_.at(c_line).InsertChar(ch, c_col);
        //    _line_text_.at(c_line).erase(_line_text_.at(c_line).begin() + c_col - 1);
        //#else
        //    //_line_[c_line].InsertChar(ch, c_col);
        //    _line_text_[c_line].erase(_line_text_[c_line].begin() + c_col - 1);
        //#endif

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
    else if(line > 0) // col == 0
    {

        _modified_ = true;
        _not_saved_ = true;


        // c_col is zero
        // delete the c_line
        std::string s{_line_text_.at(line)};
        _line_text_.erase(_line_text_.begin() + line);
        _line_text_.at(line - 1).append(s);
        //_cursor_.SetPos(, );
        //_cursor_.CR(); // TODO might be needed see below comment
        //_cursor_.Up(); // TODO this will not work if the lines are different lengths
        //Cursor::CursorPos_t goto_line{_cursor_->GetPosLine() - 1};
        //_cursor_->SetPos(goto_line, _line_text_.at(goto_line).size()); // TODO: move out to Textbox::BackspaceAtCursor
        
        // TODO: should return true here??
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////
// Create data as string if buffer is modifed
////////////////////////////////////////////////////////////////////////////

// compile lines into complete buffer object inside std::string
void Buffer::create_data() const
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

