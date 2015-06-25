#include <iostream>

#include "cpp_reflection/reflectable.hpp"
#include "cpp_reflection/reflection_manager.hpp"

class SomeClass {
public:
    void fct(void) {
        std::cout << "fct()" << std::endl;
    }

    void other_fct(void) {
        std::cout << "other_fct()" << std::endl;
    }
};

REGISTER_REFLECTABLE(SomeClass, (fct)(other_fct))

int main(void) {
    cpp_reflection::reflection_manager::make_reflection("SomeClass", "fct");
    cpp_reflection::reflection_manager::make_reflection("SomeClass", "other_fct");

    return 0;
}
