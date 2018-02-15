#ifndef HCONTAINER_HPP
#define HCONTAINER_HPP


#include <typeinfo>
#include <memory>
#include <utility>
#include <typeindex>


///////////////////////////////////////////////////////////////////////////////
// HETROGENEOUS CONTAINER
////////////////////////////////////////////////////////////////////////////////


class HContainerBase
{

    protected:

    HContainerBase()
        : _data_{nullptr}
    {
    }

    public:

    virtual
    ~HContainerBase() = 0;

    protected:

    void *_data_;
};

HContainerBase::~HContainerBase() {};


template<typename T>
class HContainer : public HContainerBase
{

    public:
    
    HContainer(std::unique_ptr<T> alloc)
        //:
    {
        HContainerBase::_data_ = alloc.get();
    }

    HContainer(T* const alloc)
        //: HContainerBase
    {
        HContainerBase::_data_ = static_cast<void* const>(alloc);
    }

    HContainer()
    //    : HContainerBase::_data_{new T}
    {
        HContainerBase::_data_ = static_cast<void* const>(new T);
    }

    ~HContainer()
    {
        delete static_cast<T* const>(HContainerBase::_data_);
    }

    HContainer(const HContainer& other)
    {
        HContainerBase::_data_ = new T;
        *_data_ = *other._data_;
    }

    HContainer(HContainer&& other)
    {
        HContainerBase::_data_ = other._data_;
        other._data_ = nullptr;
    }

    HContainer& operator=(HContainer other)
    {
        std::swap(HContainerBase::_data_, other._data_);
        return *this;
    }

    T Get() const
    {
        return *static_cast<T*>(HContainerBase::_data_);
    }

    /*
    void Get(T * const ptr)
    {
        if(_type_info_ == )
        {
            ptr = ;
        }
    }
    */

    void Set(const T& value)
    {
        //T *const ptr{static_cast<T* const>(HContainerBase::_data_)};
        *static_cast<T* const>(HContainerBase::_data_) = value;
        //T *const ptr{(T* const)(HContainerBase::_data_)};
        //*ptr = value;
    }


};

/*
class HContainerPIMPLBase
{
   
    HContainerPIMPLBase()
        : _ptr_{new HContainer<>}
    {

    }


    private:

    std::unique_ptr<HContainer> _ptr_;

}
*/

class HContainerPair
{

    public:

    template<typename T>
    //HContainerPair(std::type_id& type_id)
    //HContainerPair(const std::unique_ptr<T> alloc)
    HContainerPair(T* alloc)
    //    : _data_{type_id, new HContainerBase}
    //    : _data_{typeid(T), new HContainer<T>}
    //    : _data_{typeid(T), alloc}
    //    : _data_(std::pair<std::type_info, std::unique_ptr<HContainerBase>>(typeid(T), std::move(alloc)))
        //: _data_{typeid(T), alloc}
        : _data_{typeid(T), new HContainer<T>(alloc)} // alloc
    {
        //std::pair<std::type_info, std::unique_ptr<HContainerBase>> _data_;
        //_data_ = std::pair<std::type_info, std::unique_ptr<HContainerBase>>();
        //_data_.first = typeid(T);
        //_data_.second = std::move(alloc);
    }

    /*
    double GetDouble() const
    {
        if(type_id == DOUBLE)
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    int GetInt() const
    {
        if(_data_.first == INT)
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    std::string GetString() const
    {
        if(_data_.first == STRING)
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    long long GetLong() const
    {
        if(_data_.first == LONG LONG)
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }
    */

    template<typename T>
    T& Mutable()
    {
        if(_data_.first == typeid(T))
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    template<typename T>
    const T& Get() const
    {
        if(_data_.first == typeid(T))
        {
            return *_data_.second;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    template<typename T>
    void Set(const T& value)
    {
        if(_data_.first == typeid(T))
        {
            *_data_.second = value;
        }
        else
        {
            throw "Invalid operation";
        }
    }

    private:

    //std::pair<std::type_info, std::unique_ptr<HContainerBase>> _data_;
    std::pair<std::type_index, HContainerBase*> _data_;
};

/*
class HContainerPIMPL
{
    
    public:

    HContainerPIMPL()
    {

    }


    private:

    std::map<const std::string, 
    std::vector<

};
*/


//class HVector
//{



    //std::vector<HContainer> _vector_;


//}


#endif
