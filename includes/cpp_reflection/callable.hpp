#pragma once

#include <functional>

#include "cpp_reflection/callable_base.hpp"

namespace cpp_reflection {

template <typename T>
class callable;

//! callable class
//! contains an std::function corresponding to the class template
//!
//! simple wrapper of std::function, but inheriting from callable_base
//! useful for storing std::function of different types in the same container
//!
//! uses partial template specialization for std::function like syntax (callable<void(int)>)
template <typename ReturnType, typename... Params>
class callable<ReturnType(Params...)> : public callable_base {
public:
    callable(const std::function<ReturnType(Params...)>& fct) : m_fct(fct) {}
    ~callable(void) = default;

    callable(const callable&) = default;
    callable& operator=(const callable&) = default;

public:
    //! functor for calling internal std::function
    ReturnType operator()(Params... params) {
        return m_fct(params...);
    }

private:
    std::function<ReturnType(Params...)> m_fct;
};

} //! cpp_reflection
