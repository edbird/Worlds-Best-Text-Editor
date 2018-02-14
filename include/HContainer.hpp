#ifndef HCONTAINER_HPP
#define HCONTAINER_HPP


///////////////////////////////////////////////////////////////////////////////
// HETROGENEOUS CONTAINER
////////////////////////////////////////////////////////////////////////////////


class HContainerBase
{


    protected:

    void *_data_;
};


template<typename T>
class HContainer : public HContainerBase
{

    public:
    

    HContainer()
    //    : HContainerBase::_data_{new T}
    {
        HContainerBase::_data_ = new T;
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




#endif
