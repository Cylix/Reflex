#include <iostream>

#include "cpp_reflection/cpp_reflection.hpp"

//! some class with member functions and static member function
class SomeClass {
public:
    unsigned int nb;
    SomeClass() : nb(42) {}

    void set_nb(unsigned int n) {
        std::cout << "set to " << n << std::endl;
        nb = n;
    }

    unsigned int get_nb(void) const {
        return nb;
    }

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

//! register this class and its functions
REGISTER_CLASS_FUNCTIONS(SomeClass, (add)(sub)(concat)(get_nb)(set_nb))

//! some c-style functions
int basic_fct_1(float f, char c) {
    std::cout << "basic_fct_1(" << f << ", " << c << ")" << std::endl;
    return 42;
}

void basic_fct_2(void) {
    std::cout << "basic_fct_2()" << std::endl;
}

//! register c-style functions
REGISTER_FUNCTIONS((basic_fct_1)(basic_fct_2))

int main(void) {
    //! reflection on member functions
    auto res1 = cpp_reflection::reflection_maker<int(int, int)>::invoke("SomeClass", "add", 30, 12);
    std::cout << res1 << std::endl;

    auto res2 = cpp_reflection::reflection_maker<int(int, int)>::invoke("SomeClass", "sub", 44, 2);
    std::cout << res2 << std::endl;

    auto res3 = cpp_reflection::reflection_maker<std::string(const std::string&, unsigned int)>::invoke("SomeClass", "concat", std::string("hello"), 42);
    std::cout << res3 << std::endl;

    //! reflection on c-style functions
    auto res4 = cpp_reflection::reflection_maker<int(float, char)>::invoke("basic_fct_1", 4.2, 'z');
    std::cout << res4 << std::endl;

    cpp_reflection::reflection_maker<void()>::invoke("basic_fct_2");

    //! reflection on member functions with custom object instance
    SomeClass s;
    auto res5 = cpp_reflection::reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res5 << std::endl;

    cpp_reflection::reflection_maker<void(unsigned int)>::invoke(&s, "SomeClass", "set_nb", 1234);

    auto res6 = cpp_reflection::reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res6 << std::endl;

    return 0;
}
