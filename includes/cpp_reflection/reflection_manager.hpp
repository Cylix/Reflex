#pragma once

#include <list>
#include <string>

#include "cpp_reflection/callable.hpp"
#include "cpp_reflection/reflectable_base.hpp"
#include "cpp_reflection/reflection_exception.hpp"

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
    template <typename ReturnType, typename... Params>
    static ReturnType make_reflection(const std::string& class_name, const std::string& function_name, Params... params) {
        return get_instance()._make_reflection<ReturnType, Params...>(class_name, function_name, params...);
    }

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

    //! TODO: COMMENT
    template <typename ReturnType, typename... Params>
    ReturnType _make_reflection(const std::string& class_name, const std::string& function_name, Params... params) {
        auto it = std::find_if(m_types.begin(), m_types.end(), [class_name](const auto& type) {
            return type->get_name() == class_name;
        });

        if (it == m_types.end())
            throw reflection_exception("Class " + class_name + " is not registered as reflectable.");

        auto fct = std::dynamic_pointer_cast<callable<ReturnType(Params...)>>((*it)->get_function(function_name));

        if (not fct)
            throw reflection_exception("invalid function signature for " + class_name + "::" + function_name);

        return (*fct)(params...);
    }

private:
    //! registered types
    std::list<const reflectable_base*> m_types;
};

} //! cpp_reflection
