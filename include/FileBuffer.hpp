#ifndef FILEBUFFER_HPP
#define FILEBUFFER_HPP


#include "Buffer.hpp"



#include <iomanip>
#include <fstream>



class FileBuffer : public Buffer
{


    public:

    FileBuffer();

    virtual ~FileBuffer();

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



    private:

    ////////////////////////////////////////////////////////////////////////////
    // Private functions
    ////////////////////////////////////////////////////////////////////////////

    // compile lines into complete buffer object inside std::string
    void create_data() const;

    ////////////////////////////////////////////////////////////////////////////
    // Data members
    ////////////////////////////////////////////////////////////////////////////

    // not saved flag
    // this flag is set to true when the buffer is modified but not saved
    // when the buffer is saved, this flag is set to false
    mutable bool _not_saved_;

    // filename associated with buffer
    /*mutable*/ std::string _filename_;

};


#endif // FILEBUFFER_HPP
