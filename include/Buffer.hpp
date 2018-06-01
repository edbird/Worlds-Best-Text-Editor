#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <memory>


//#include "Cursor.hpp"


//#define DEBUG 1


// TODO: seperate size and drawing code to new class BufferWindow
class Buffer
{

    public:


    //Buffer(const Config& config);
    Buffer();
    
    virtual ~Buffer();

    std::size_t Size() const;


    // is the buffer in the "default" state - contains nothing, empty
    bool IsEmpty() const;

    // get reference 
    const std::string& Get() const;

    // get number of lines in buffer (line count)
    std::size_t /*int*/ GetLineCount() const;
    
    std::size_t LineLength(const std::size_t line) const
    {
        return _line_text_.at(line).size();
    }

    // get const reference to data as std::vector<std::string>
    // used for iteration where it is most convenient to iterate
    // matching the method of iteration to the way in which the
    // data is stored (as std::vector<std::string>>)
    const std::vector<std::string>& GetContainer() const;

    // these functions do not check if line, col are valid
    // error will be thrown if invalid

    // insert single char at position specified by line and col
    void Insert(const char ch, const std::size_t line, const std::size_t col);
    // insert new line at position specified by line
    void InsertNewLine(const std::size_t line, const std::size_t col);
    // delete single char / newline at position specified by line and col
    bool Delete(const std::size_t line, const std::size_t col);

    // copy len characters
    void Copy(const std::size_t line_to, const std::size_t col_to, const std::size_t line_from, const std::size_t col_from, const std::size_t len);
    // move len characters
    void Move(const std::size_t line_to, const std::size_t col_to, const std::size_t line_from, const std::size_t col_from, const std::size_t len);
    // insert len characters
    void Insert(const char ch, const std::size_t line, const std::size_t col, const std::size_t len = 1);
    // insert len characters pointed at by ch
    void Insert(const char *ch, const std::size_t line, const std::size_t col, const std::size_t len = 1);
    // insert std::string
    void Insert(const std::string &str, const std::size_t line, const std::size_t col);
    // delete len characters
    void Delete(const std::size_t line, const std::size_t col, const std::size_t len = 1);
    
    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Private functions
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Data members
    ////////////////////////////////////////////////////////////////////////////

    // reference to config
    //const Config& _config_;

    // actual data
    std::vector<std::string> _line_text_;

    // "raw buffer" data emulation
    mutable std::string _data_;

    // if buffer is modified, the create_data() function must be called
    // before Get()
    mutable bool _modified_;

    // const
    static const std::string _new_line_string_; //("\n");

};


#endif // BUFFER_HPP
