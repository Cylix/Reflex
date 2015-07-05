#pragma once

#include <functional>

#include "cpp_reflection/function/function_base.hpp"

namespace cpp_reflection {

template <typename T>
class class_function;

//! function class
//! contains an std::function corresponding to the class template
//!
//! simple wrapper of std::function, but inheriting from function_base
//! useful for storing std::function of different types in the same container
//!
//! uses partial template specialization for std::function like syntax (function<void(int)>)
template <typename ReturnType, typename... Params>
class class_function<ReturnType(Params...)> : public function_base {
public:
    class_function(const std::function<ReturnType(Params...)>& call_on_new_instance)
      : m_call_on_new_instance(call_on_new_instance) {}

    ~class_function(void) = default;

    class_function(const class_function&) = default;
    class_function& operator=(const class_function&) = default;

    bool is_member_function(void) const { return true; }

public:
    //! functor for calling internal std::function
    ReturnType operator()(Params... params) {
        return m_call_on_new_instance(params...);
    }

private:
    std::function<ReturnType(Params...)> m_call_on_new_instance;
};

} //! cpp_reflection
