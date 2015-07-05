#pragma once

#include <list>
#include <string>
#include <algorithm>

#include "cpp_reflection/function/function.hpp"
#include "cpp_reflection/reflectable/reflectable_base.hpp"
#include "cpp_reflection/reflection_exception.hpp"

namespace cpp_reflection {

//! singleton class
//! this class makes the reflection
//! it stores reflectable object that are used for reflection
class reflection_manager {
public:
    //! singleton
    static reflection_manager& get_instance(void);

    //! register a new reflectable
    //! this reflectable will later can be used for reflection
    void register_reflectable(const reflectable_base* reflectable);

    //! make reflection
    template <typename ReturnType, typename... Params>
    ReturnType invoke(const std::string& class_name, const std::string& function_name, Params... params) {
        auto reflectable = find_reflectable(class_name);
        auto fct = reflectable->get_function(function_name);

        return fct.template invoke<ReturnType, Params...>(params...);
    }

    //! make reflection
    template <typename Type, typename ReturnType, typename... Params>
    ReturnType invoke(Type* obj, const std::string& class_name, const std::string& function_name, Params... params) {
        auto reflectable = find_reflectable(class_name);
        auto fct = reflectable->get_function(function_name);

        return fct.template invoke<Type, ReturnType, Params...>(obj, params...);
    }

    const reflectable_base& get_class(const std::string& class_name) {
        return *find_reflectable(class_name);
    }

    const reflectable_base& get_functions(void) const {
        return *find_reflectable("");
    }

private:
    const reflectable_base* find_reflectable(const std::string& class_name) const {
        auto it = std::find_if(m_types.begin(), m_types.end(), [class_name](const auto& type) {
            return type->get_name() == class_name;
        });

        if (it == m_types.end())
            throw reflection_exception("Class " + class_name + " is not registered as reflectable.");

        return *it;
    }

private:
    //! ctor & dtor
    reflection_manager(void) = default;
    ~reflection_manager(void) = default;

    //! copy ctor & assignment operator
    reflection_manager(const reflection_manager& manager) = default;
    reflection_manager& operator=(const reflection_manager& manager) = default;

private:
    //! registered types
    std::list<const reflectable_base*> m_types;
};

} //! cpp_reflection
