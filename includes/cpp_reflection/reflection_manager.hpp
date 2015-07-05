#pragma once

#include <list>
#include <string>
#include <algorithm>

#include "cpp_reflection/function/c_function.hpp"
#include "cpp_reflection/function/class_function.hpp"
#include "cpp_reflection/function/class_function2.hpp"
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
    ReturnType invoke_class_function_on_new(const std::string& class_name, const std::string& function_name, Params... params) {
        //! we first search for the reflectable object matching the class name
        auto it = std::find_if(m_types.begin(), m_types.end(), [class_name](const auto& type) {
            return type->get_name() == class_name;
        });

        //! if nothing is found, class is not registered
        if (it == m_types.end())
            throw reflection_exception("Class " + class_name + " is not registered as reflectable.");

        //! we ask the reflectable object for a function<> object matching the function name
        //! get_function will throw a reflection_exception if function is not registered
        //!
        //! it also returns a generic std::shared_ptr<callable_base>
        //! we dynamically cast this pointer to a function<> object templated on the expected function signature
        auto fct_ = (*it)->get_function(function_name);

        if (fct_->is_member_function()) {
            auto fct = std::dynamic_pointer_cast<class_function<ReturnType(Params...)>>(fct_);

            //! if dynamic_cast failed, it means that the registered function has a different signature than the expected signature
            if (not fct)
                throw reflection_exception("invalid function signature for " + class_name + "::" + function_name);

            //! if everything is ok, we call the function
            return (*fct)(params...);
        }
        else {
            auto fct = std::dynamic_pointer_cast<c_function<ReturnType(Params...)>>(fct_);

            //! if dynamic_cast failed, it means that the registered function has a different signature than the expected signature
            if (not fct)
                throw reflection_exception("invalid function signature for " + class_name + "::" + function_name);

            //! if everything is ok, we call the function
            return (*fct)(params...);
        }
    }

    //! make reflection
    template <typename Type, typename ReturnType, typename... Params>
    ReturnType invoke_class_function_on_given(Type* obj, const std::string& class_name, const std::string& function_name, Params... params) {
        //! we first search for the reflectable object matching the class name
        auto it = std::find_if(m_types.begin(), m_types.end(), [class_name](const auto& type) {
            return type->get_name() == class_name;
        });

        //! if nothing is found, class is not registered
        if (it == m_types.end())
            throw reflection_exception("Class " + class_name + " is not registered as reflectable.");

        //! we ask the reflectable object for a function<> object matching the function name
        //! get_function will throw a reflection_exception if function is not registered
        //!
        //! it also returns a generic std::shared_ptr<callable_base>
        //! we dynamically cast this pointer to a function<> object templated on the expected function signature
        auto fct = std::dynamic_pointer_cast<class_function2<Type, ReturnType(Params...)>>((*it)->get_function_on_given_instance(function_name));

        //! if dynamic_cast failed, it means that the registered function has a different signature than the expected signature
        if (not fct)
            throw reflection_exception("invalid function signature for " + class_name + "::" + function_name);

        //! if everything is ok, we call the function
        return (*fct)(obj, params...);
    }

    //! make reflection
    template <typename ReturnType, typename... Params>
    ReturnType invoke_function(const std::string& function_name, Params... params) {
        //! we first search for the reflectable object matching the class name
        auto it = std::find_if(m_types.begin(), m_types.end(), [](const auto& type) {
            return type->get_name() == "";
        });

        //! if nothing is found, class is not registered
        if (it == m_types.end())
            throw reflection_exception("Class NOCLASS is not registered as reflectable.");

        //! we ask the reflectable object for a function<> object matching the function name
        //! get_function will throw a reflection_exception if function is not registered
        //!
        //! it also returns a generic std::shared_ptr<callable_base>
        //! we dynamically cast this pointer to a function<> object templated on the expected function signature
        auto fct = std::dynamic_pointer_cast<c_function<ReturnType(Params...)>>((*it)->get_function(function_name));

        //! if dynamic_cast failed, it means that the registered function has a different signature than the expected signature
        if (not fct)
            throw reflection_exception("invalid function signature for " + function_name);

        //! if everything is ok, we call the function
        return (*fct)(params...);
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
