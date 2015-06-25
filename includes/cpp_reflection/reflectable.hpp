#pragma once

#include <map>
#include <string>
#include <iostream>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include "cpp_reflection/reflectable_base.hpp"
#include "cpp_reflection/reflection_manager.hpp"
#include "cpp_reflection/reflection_exception.hpp"

namespace cpp_reflection {

template <typename Type>
class reflectable : public reflectable_base {
public:
    //! typdef on the templated type
    //! useful for building a new object (new some_reflectable::type)
    typedef Type type;

    //! member function typedef
    typedef void (Type::*member_function)();

public:
    //! ctor & dotr
    reflectable(const std::string& name, const std::map<std::string, member_function>& functions)
    : m_name(name)
    , m_member_functions(functions) {
        reflection_manager::register_reflectable(this);
    }

    ~reflectable(void) = default;

    //! copy ctor & assignment operator
    reflectable(const reflectable& reflectable) = default;
    reflectable& operator=(const reflectable& reflectable) = default;

public:
    //! register a new member function for this
    //! function must respect the member_function typedef prototype
    //! name is really important because reflection will be made with it
    void register_member_function(const std::string& name, member_function f) {
        m_member_functions[name] = f;
    }

    //! call member function by string name
    void call(const std::string& function_name) const {
        if (not is_registered(function_name))
            throw reflection_exception("Function " + m_name + "::" + function_name + " is not registered.");

        (Type().*m_member_functions.at(function_name))();
    }

    //! is member function registered
    bool is_registered(const std::string& function_name) const {
        return m_member_functions.count(function_name);
    }

    const std::string& get_name(void) const {
        return m_name;
    }

private:
    //! name of the reflectable
    //! this is by this name that the reflection will operate
    std::string m_name;

    //! list of functions for this object
    std::map<std::string, member_function> m_member_functions;
};

//! this define creates a static reflectable
//! REGISTER_REFLECTABLE(type, (fct)) will generates a static reflectable<type> reflectable_int("type", { { "fct", &type::fct } }) var
//! since it is static, the type is registered at program startup
#define TO_STRING(val) #val
#define MAKE_REGISTERABLE_FUNCTION(r, type, i, function) BOOST_PP_COMMA_IF(i) { TO_STRING(function), &type::function }
#define REGISTER_REFLECTABLE(type, functions) static cpp_reflection::reflectable<type> reflectable_##type(#type, { BOOST_PP_SEQ_FOR_EACH_I( MAKE_REGISTERABLE_FUNCTION, type, functions ) });

} //! cpp_reflection
