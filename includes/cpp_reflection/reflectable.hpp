#pragma once

#include <map>
#include <string>

#include "cpp_reflection/reflectable_base.hpp"
#include "cpp_reflection/reflection_manager.hpp"
#include "cpp_reflection/reflection_exception.hpp"

namespace cpp_reflection {

template <typename Type>
class reflectable : public reflectable_base {
public:
    //! ctor & dotr
    reflectable(const std::string& name) : m_name(name) {
        reflection_manager::register_reflectable(this);
    }

    ~reflectable(void) = default;

    //! copy ctor & assignment operator
    reflectable(const reflectable& reflectable) = default;
    reflectable& operator=(const reflectable& reflectable) = default;

public:
    //! typdef on the templated type
    //! useful for building a new object (new some_reflectable::type)
    typedef Type type;

    //! member function typedef
    typedef void (Type::*member_function)();

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
//! REGISTER_REFLECTABLE(int) will generates a static reflectable<int> reflectable_int("int") var
//! since it is static, the type is registered at program startup
#define REGISTER_REFLECTABLE(type) static cpp_reflection::reflectable<type> reflectable_##type(#type);

#define REGISTER_MEMBER_FUNCTION(type, function) reflectable_##type.register_member_function(#function, &type::function);

} //! cpp_reflection
