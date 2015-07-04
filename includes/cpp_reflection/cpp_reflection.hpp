#pragma once

#include <string>

#include "reflectable.hpp"
#include "reflection_manager.hpp"

namespace cpp_reflection {

template <typename T>
struct make_reflection;

//! partial template specialization is not available for functions
//! so we can't have an std::function like syntax for reflection_manager::make_reflection<>
//! we must make reflection_manager::make_reflection<void, int, int> and can't do reflection_manager::make_reflection<void(int, int)>
//! this is not really convenient...
//!
//! we get around of this limitation by wrapping this function call inside a simple struct
//! this struct is partially specialized
//! this way, we can do make_reflection<void(int, int)>::call(...)
template <typename ReturnType, typename... Params>
struct make_reflection<ReturnType(Params...)> {
    static ReturnType call(const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::make_reflection<ReturnType, Params...>(class_name, function_name, params...);
    }
};

} //! cpp_reflection
