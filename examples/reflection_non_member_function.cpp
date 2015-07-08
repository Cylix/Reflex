#include <iostream>

#include "reflex/reflex.hpp"

using namespace reflex;

class SomeClass {
public:
    static std::string concat(const std::string& str, unsigned int nb) {
        std::cout << "concat(" << str << ", " << nb << ")" << std::endl;
        return str + std::to_string(nb);
    }
};

REGISTER_CLASS_FUNCTIONS(SomeClass, (concat))

int basic_fct_1(float f, char c) {
    std::cout << "basic_fct_1(" << f << ", " << c << ")" << std::endl;
    return 42;
}

void basic_fct_2(void) {
    std::cout << "basic_fct_2()" << std::endl;
}

REGISTER_FUNCTIONS((basic_fct_1)(basic_fct_2))

int main(void) {
    auto res1 = reflection_maker<std::string(const std::string&, unsigned int)>::invoke("SomeClass", "concat", std::string("hello"), 42);
    std::cout << res1 << std::endl;

    auto res2 = reflection_maker<int(float, char)>::invoke("basic_fct_1", 4.2, 'z');
    std::cout << res2 << std::endl;

    reflection_maker<void()>::invoke("basic_fct_2");

    return 0;
}
