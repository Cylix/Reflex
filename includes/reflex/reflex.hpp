#pragma once

#include <string>

#include "reflex/reflectable/reflectable.hpp"
#include "reflex/reflection_manager.hpp"

namespace reflex {

template <typename T>
struct reflection_maker;

//! partial template specialization is not available for functions
//! so we can't have an std::function like syntax for reflection_manager::make_reflection<>
//! we must make reflection_manager::make_reflection<void, int, int> and can't do reflection_manager::make_reflection<void(int, int)>
//! this is not really convenient...
//!
//! we get around of this limitation by wrapping this function call inside a simple struct
//! this struct is partially specialized
//! this way, we can do make_reflection<void(int, int)>::invoke(...)
template <typename ReturnType, typename... Params>
struct reflection_maker<ReturnType(Params...)> {
    static ReturnType invoke(const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::get_instance().invoke<ReturnType, Params...>(class_name, function_name, params...);
    }

    template <typename Type>
    static ReturnType invoke(Type* obj, const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::get_instance().invoke<Type, ReturnType, Params...>(obj, class_name, function_name, params...);
    }

    template <typename Type>
    static ReturnType invoke(const std::shared_ptr<Type>& obj, const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::get_instance().invoke<Type, ReturnType, Params...>(obj, class_name, function_name, params...);
    }

    template <typename Type>
    static ReturnType invoke(const std::unique_ptr<Type>& obj, const std::string& class_name, const std::string& function_name, Params... params) {
        return reflection_manager::get_instance().invoke<Type, ReturnType, Params...>(obj, class_name, function_name, params...);
    }

    static ReturnType invoke(const std::string& function_name, Params... params) {
        return reflection_manager::get_instance().invoke<ReturnType, Params...>("", function_name, params...);
    }
};

//! this define creates a static reflectable
//! REGISTER_REFLECTABLE(type, (fct)(other_fct)) will generates a static reflectable<type> reflectable_int("type", { "fct", &type::fct }, { "other_fct", &type::other_fct }) var
//! since it is static, the type is registered at program startup
#define TO_STRING(val) #val
#define MAKE_REGISTERABLE_FUNCTION(r, type, i, function) BOOST_PP_COMMA_IF(i) std::make_pair( std::string(TO_STRING(function)), &type::function )
#define REGISTER_REFLECTABLE(type, functions) static reflex::reflectable<type> reflectable_##type(#type, BOOST_PP_SEQ_FOR_EACH_I( MAKE_REGISTERABLE_FUNCTION, type, functions ));

#define REGISTER_CLASS_FUNCTIONS(type, functions) REGISTER_REFLECTABLE(type, functions)
#define REGISTER_FUNCTIONS(functions) REGISTER_REFLECTABLE(, functions)

} //! reflex
