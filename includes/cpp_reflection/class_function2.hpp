#pragma once

#include <functional>

#include "cpp_reflection/function_base.hpp"

namespace cpp_reflection {

template <typename T, typename U>
class class_function2;

//! function class
//! contains an std::function corresponding to the class template
//!
//! simple wrapper of std::function, but inheriting from function_base
//! useful for storing std::function of different types in the same container
//!
//! uses partial template specialization for std::function like syntax (function<void(int)>)
template <typename Type, typename ReturnType, typename... Params>
class class_function2<Type, ReturnType(Params...)> : public function_base {
public:
    class_function2(const std::function<ReturnType(Type*, Params...)>& call_on_given_instance)
      : m_call_on_given_instance(call_on_given_instance) {}

    ~class_function2(void) = default;

    class_function2(const class_function2&) = default;
    class_function2& operator=(const class_function2&) = default;

    bool is_member_function(void) const { return true; }

public:
    ReturnType operator()(Type* obj, Params... params) {
        return m_call_on_given_instance(obj, params...);
    }

private:
    std::function<ReturnType(Type*, Params...)> m_call_on_given_instance;
};

} //! cpp_reflection
