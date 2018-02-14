#ifndef HCONTAINER_HPP
#define HCONTAINER_HPP


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


class HVector


#endif
