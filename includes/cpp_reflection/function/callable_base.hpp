#pragma once

namespace cpp_reflection {

//! callable_base class
//! empty base class inherited by callable class
class callable_base {
public:
    virtual ~callable_base(void) = default;

    virtual bool is_member_function(void) const = 0;
};

} //! cpp_reflection
