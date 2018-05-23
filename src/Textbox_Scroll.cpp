#include "Textbox.hpp"

void Textbox::ScrollDown()
{
    // always need to update the wrap count vector before accessing it
    //update_wrap_count(); // not accessed in this function

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_index_ < buffer_contents.size() - 1)
    {
        ++ _scroll_index_;
        // 2018-05-23: bug fix: program crash unless _scroll_sub_index_ is set
        // to an in bounds value
        _scroll_sub_index_ = 0;
    }
    else
    {
        std::cout << "Textbox::ScrollDown(): cannot scroll down" << std::endl;
    }
}


void Textbox::ScrollUp()
{
    // always need to update the wrap count vector before accessing it
    //update_wrap_count(); // not accessed in this function

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_index_ > 0)
    {
        -- _scroll_index_;
        // 2018-05-23: bug fix: program crash unless _scroll_sub_index_ is set
        // to an in bounds value
        //_scroll_sub_index_ = wrap_count.at(_scroll_index_); // <- an idea
        _scroll_sub_index_ = 0;
    }
    else
    {
        std::cout << "Textbox::ScrollUp(): cannot scroll up" << std::endl;
    }
}


void Textbox::ScrollDownSub()
{
    // always need to update the wrap count vector before accessing it
    update_wrap_count();

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    std::cout << _scroll_index_ << " SI" << std::endl;
    if(_scroll_sub_index_ < wrap_count.at(_scroll_index_) - 1)
    {
        std::cout << "A" << std::endl;
        ++ _scroll_sub_index_;
    }
    else
    {
        std::cout << "B" << std::endl;

        if(_scroll_index_ < buffer_contents.size() - 1)
        {
        std::cout << "C" << std::endl;
            _scroll_sub_index_ = 0;
            ++ _scroll_index_;
        }
        else
        {
            std::cout << "Textbox::ScrollDownSub(): cannot scroll down" << std::endl;
        }
    }
}


void Textbox::ScrollUpSub()
{
    // always need to update the wrap count vector before accessing it
    update_wrap_count();

    // const reference to data structure
    const std::vector<std::string> &buffer_contents{GetContainer()};

    if(_scroll_sub_index_ > 0)
    {
        -- _scroll_sub_index_;
    }
    else
    {
        if(_scroll_index_ > 0)
        {
            _scroll_sub_index_ = wrap_count.at(_scroll_index_ - 1) - 1;
            -- _scroll_index_;
        }
        else
        {
            std::cout << "Textbox::ScrollUpSub(): cannot scroll up" << std::endl;
        }
    }
}