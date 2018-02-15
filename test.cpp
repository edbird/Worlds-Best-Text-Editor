
#include <iostream>

#include "./include/HContainer.hpp"

#include <typeindex>


int main()
{

    //std::type_info tinfo(typeid(double));
    std::type_index tinfo(typeid(double));

    std::pair<int, int> mypair{1, 1};
    //std::pair<std::type_info, void*>{typeid(double), nullptr};
    std::pair<std::type_index, HContainerBase*> thepair{typeid(int), new HContainer<int>};
    delete thepair.second;

    

    HContainer<double> container;
    container.Set(2.0);
    std::cout << container.Get() << std::endl;

//    std::map<const std::string, HContainerBase> map;
//    map["configoption1"] = 

    //HContainerPair hcontainerpair(std::unique_ptr<int>(new int));
    //HContainerPair hcontainerpair(std::unique_ptr<int>(new int));
    HContainerPair hcontainerpair(new int);

    return 0;
}
