#pragma once

#include <map>
#include <string>
#include <iostream>
#include <memory>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include "cpp_reflection/callable.hpp"
#include "cpp_reflection/reflectable_base.hpp"
#include "cpp_reflection/reflection_manager.hpp"
#include "cpp_reflection/reflection_exception.hpp"

namespace cpp_reflection {

//! reflectable class
//! contains information about registered class (class name, member functions name and member functions pointers)
//! inherits from reflectable_base in order to store reflectables of different types in the same container
//!
//! This class registered itself in the reflection_manager and is used by the manager during reflection
template <typename Type>
class reflectable : public reflectable_base {
public:
    //! ctor & dotr
    template <typename... Fcts>
    reflectable(const std::string& name, Fcts... fcts)
    : m_name(name) {
        reflection_manager::register_reflectable(this);
        register_member_function(fcts...);
    }

    ~reflectable(void) = default;

    //! copy ctor & assignment operator
    reflectable(const reflectable& reflectable) = default;
    reflectable& operator=(const reflectable& reflectable) = default;

public:
    //! unpack Fcts... variadic template
    template <typename Fct, typename... Fcts>
    void register_member_function(const std::pair<std::string, Fct> fct, Fcts... fcts) {
        register_member_function(fct);
        register_member_function(fcts...);
    }

    //! register_member_function
    //! in order to have an equivalent of std::bind with default std::placeholders, we use closures
    //! this closure has the same signature than the registered function and simply forwards its parameters to the function
    //! the closure is stored inside a callable<> object
    template <typename ReturnType, typename... Params>
    void register_member_function(const std::pair<std::string, ReturnType (Type::*)(Params...)> fct) {
        m_member_functions[fct.first] = std::make_shared<callable<ReturnType(Params...)>>([=] (Params... params) -> ReturnType {
            return (Type().*fct.second)(params...);
        });
    }

    //! get function by name
    const std::shared_ptr<callable_base>& get_function(const std::string& function_name) const {
        if (not is_registered(function_name))
            throw reflection_exception(m_name + "::" + function_name + " is not registered");

        return m_member_functions.at(function_name);
    }

    //! is member function registered
    bool is_registered(const std::string& function_name) const {
        return m_member_functions.count(function_name);
    }

    //! return reflectable class name
    const std::string& get_name(void) const {
        return m_name;
    }

private:
    //! name of the reflectable
    //! this is by this name that the reflection will operate
    std::string m_name;

    //! list of functions for this object
    //! associate function name to a callable<> object
    std::map<std::string, std::shared_ptr<callable_base>> m_member_functions;
};

//! this define creates a static reflectable
//! REGISTER_REFLECTABLE(type, (fct)(other_fct)) will generates a static reflectable<type> reflectable_int("type", { "fct", &type::fct }, { "other_fct", &type::other_fct }) var
//! since it is static, the type is registered at program startup
#define TO_STRING(val) #val
#define MAKE_REGISTERABLE_FUNCTION(r, type, i, function) BOOST_PP_COMMA_IF(i) std::make_pair( std::string(TO_STRING(function)), &type::function )
#define REGISTER_REFLECTABLE(type, functions) static cpp_reflection::reflectable<type> reflectable_##type(#type, BOOST_PP_SEQ_FOR_EACH_I( MAKE_REGISTERABLE_FUNCTION, type, functions ));

} //! cpp_reflection
