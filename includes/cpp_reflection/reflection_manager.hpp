#pragma once

#include <list>
#include <string>

#include "cpp_reflection/reflectable_base.hpp"

namespace cpp_reflection {

//! singleton class
//! this class makes the reflection
//! it stores reflectable object that are used for reflection
class reflection_manager {
public:
    //! register a new reflectable
    //! this reflectable will later can be used for reflection
    static void register_reflectable(const reflectable_base* reflectable);

    //! make reflection
    static void make_reflection(const std::string& class_name, const std::string& function_name);

private:
    //! singleton
    static reflection_manager& get_instance(void);

    //! ctor & dtor
    reflection_manager(void) = default;
    ~reflection_manager(void) = default;

    //! copy ctor & assignment operator
    reflection_manager(const reflection_manager& manager) = default;
    reflection_manager& operator=(const reflection_manager& manager) = default;

private:
    //! implementation
    void _register_reflectable(const reflectable_base* reflectable);
    void _make_reflection(const std::string& class_name, const std::string& function_name);

private:
    //! registered types
    std::list<const reflectable_base*> m_types;
};

} //! cpp_reflection
