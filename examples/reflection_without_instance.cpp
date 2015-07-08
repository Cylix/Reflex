#include <iostream>

#include "cpp_reflection/cpp_reflection.hpp"

using namespace cpp_reflection;

class SomeClass {
public:
    int add(int nb1, int nb2) const {
        std::cout << "add(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 + nb2;
    }

    int sub(int nb1, int nb2) const {
        std::cout << "sub(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 - nb2;
    }

};

REGISTER_CLASS_FUNCTIONS(SomeClass, (add)(sub))

int main(void) {
    auto res1 = reflection_maker<int(int, int)>::invoke("SomeClass", "add", 30, 12);
    std::cout << res1 << std::endl;

    auto res2 = reflection_maker<int(int, int)>::invoke("SomeClass", "sub", 44, 2);
    std::cout << res2 << std::endl;

    return 0;
}
