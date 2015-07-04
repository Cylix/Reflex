#include <iostream>

#include "cpp_reflection/cpp_reflection.hpp"

class SomeClass {
public:
    int add(int nb1, int nb2) {
        std::cout << "add(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 + nb2;
    }

    int sub(int nb1, int nb2) {
        std::cout << "sub(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 - nb2;
    }

    static std::string concat(const std::string& str, unsigned int nb) {
        std::cout << "concat(" << str << ", " << nb << ")" << std::endl;
        return str + std::to_string(nb);
    }
};

REGISTER_REFLECTABLE(SomeClass, (add)(sub)(concat))

int main(void) {
    auto res1 = cpp_reflection::make_reflection<int(int, int)>::call("SomeClass", "add", 30, 12);
    std::cout << res1 << std::endl;

    auto res2 = cpp_reflection::make_reflection<int(int, int)>::call("SomeClass", "sub", 44, 2);
    std::cout << res2 << std::endl;

    auto res3 = cpp_reflection::make_reflection<std::string(const std::string&, unsigned int)>::call("SomeClass", "concat", std::string("hello"), 42);
    std::cout << res3 << std::endl;

    return 0;
}
