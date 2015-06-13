#pragma once

#include <stdexcept>

namespace cpp_reflection {

class reflection_exception : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

} //! cpp_reflection
