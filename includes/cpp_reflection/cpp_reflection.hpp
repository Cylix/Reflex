#pragma once

#include <string>

#include "reflectable.hpp"
#include "reflection_manager.hpp"

namespace cpp_reflection {

template <typename T>
struct make_reflection;

template <typename ReturnType, typename... Params>
struct make_reflection<ReturnType(Params...)> {
    static ReturnType call(const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::make_reflection<ReturnType, Params...>(class_name, function_name, params...);
    }
};

} //! cpp_reflection
