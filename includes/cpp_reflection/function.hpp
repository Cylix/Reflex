#pragma once

#include <functional>

#include "cpp_reflection/function_base.hpp"

namespace cpp_reflection {

template <typename T>
class function;

//! function class
//! contains an std::function corresponding to the class template
//!
//! simple wrapper of std::function, but inheriting from function_base
//! useful for storing std::function of different types in the same container
//!
//! uses partial template specialization for std::function like syntax (function<void(int)>)
template <typename ReturnType, typename... Params>
class function<ReturnType(Params...)> : public function_base {
public:
    function(const std::function<ReturnType(Params...)>& fct) : m_fct(fct) {}
    ~function(void) = default;

    function(const function&) = default;
    function& operator=(const function&) = default;

public:
    //! functor for calling internal std::function
    ReturnType operator()(Params... params) {
        return m_fct(params...);
    }

private:
    std::function<ReturnType(Params...)> m_fct;
};

} //! cpp_reflection
