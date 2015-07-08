#include <iostream>

#include "reflex/reflex.hpp"

using namespace reflex;

class SomeClass {
public:
    unsigned int nb;
    SomeClass() : nb(42) {}

    void set_nb(unsigned int nb) {
        this->nb = nb;
    }

    unsigned int get_nb(void) const {
        return nb;
    }
};

REGISTER_CLASS_FUNCTIONS(SomeClass, (get_nb)(set_nb))

int main(void) {
    SomeClass s;
    auto res1 = reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res1 << std::endl;

    reflection_maker<void(unsigned int)>::invoke(&s, "SomeClass", "set_nb", 1234);

    auto res2 = reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res2 << std::endl;

    return 0;
}
