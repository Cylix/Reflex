#include <iostream>

#include "cpp_reflection/reflectable.hpp"
#include "cpp_reflection/reflection_manager.hpp"

class SomeClass {
public:
    void fct(void) {
        std::cout << "Hello World" << std::endl;
    }
};

REGISTER_REFLECTABLE(SomeClass)

int main(void) {
    REGISTER_MEMBER_FUNCTION(SomeClass, fct)

    cpp_reflection::reflection_manager::make_reflection("SomeClass", "fct");

    return 0;
}
