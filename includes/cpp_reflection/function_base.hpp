#pragma once

namespace cpp_reflection {

//! function_base class
//! empty base class inherited by callable class
class function_base {
public:
    virtual ~function_base(void) = default;

    virtual bool is_member_function(void) const = 0;
};

} //! cpp_reflection
