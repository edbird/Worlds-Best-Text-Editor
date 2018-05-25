#ifndef BUFFER_HPP
#define BUFFER_HPP


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>


#include "Cursor.hpp"


#define DEBUG 1


// TODO: seperate size and drawing code to new class BufferWindow
class Buffer
{

    public:


    //Buffer(const Config& config);
    Buffer();
    
    ~Buffer();

    std::size_t Size() const;

    std::string GetFilename() const;
    void SetFilename(const std::string& filename);

    void Save() const;
    void Open();

    // save to text file
    void Save(const std::string& filename) const;

    // this open method overwrites the buffer
    // and sets the _not_saved_ flag to FALSE
    void Open(const std::string& filename);

    // 
    bool NotSaved() const;

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
    void Insert(const char ch, const std::size_t line, const std::size_t col);
    void InsertNewLine(const std::size_t line, const std::size_t col);
    bool Delete(const std::size_t line, const std::size_t col);

    
    private:
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Private functions
    ////////////////////////////////////////////////////////////////////////////

    // compile lines into complete buffer object inside std::string
    void create_data() const;

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

    // not saved flag
    // this flag is set to true when the buffer is modified but not saved
    // when the buffer is saved, this flag is set to false
    mutable bool _not_saved_;

    // const
    const std::string _new_line_string_{std::string("\n")};

    // filename associated with buffer
    mutable std::string _filename_;

};


#endif // BUFFER_HPP
