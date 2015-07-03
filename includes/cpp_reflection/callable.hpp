#pragma once

#include <functional>

#include "cpp_reflection/callable_base.hpp"

namespace cpp_reflection {

template <typename T>
class callable;

template <typename ReturnType, typename... Params>
class callable<ReturnType(Params...)> : public callable_base {
public:
    callable(const std::function<ReturnType(Params...)>& fct) : m_fct(fct) {}
    ~callable(void) = default;

    callable(const callable&) = default;
    callable& operator=(const callable&) = default;

public:
    ReturnType operator()(Params... params) {
        return m_fct(params...);
    }

private:
    std::function<ReturnType(Params...)> m_fct;
};

} //! cpp_reflection
