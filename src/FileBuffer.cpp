#include "FileBuffer.hpp"

FileBuffer::FileBuffer()
    : _not_saved_{false} // do not save a new buffer
{
}

FileBuffer::~FileBuffer()
{
}

std::string Buffer::GetFilename() const
{
    return _filename_;
}


void Buffer::SetFilename(const std::string& filename)
{
    std::cout << "Buffer::SetFilename(" << filename << ")" << std::endl;
    _filename_ = filename;
}


// save to text file
void Buffer::Save(const std::string& filename) const
{
    _filename_ = filename;
    
    Save();
}

void Buffer::Save() const
{
    std::ofstream ofs(_filename_, std::ios::out | std::ios::trunc);
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
    _filename_ = filename;

    Open();
}

void Buffer::Open()
{

    // TODO: maximum memory useage (virtual machine memory)
    // clear old contents
    _line_text_.clear();

    std::ifstream ifs(_filename_, std::ios::in | std::ios::ate);
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
            std::cerr << "Error opening file " << _filename_ << "\nData corruption or invalid file type detected\nPosition: " << buf_p1 - buf << std::endl;
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



