#include "cpp_reflection/reflection_manager.hpp"
#include "cpp_reflection/reflection_exception.hpp"

#include <algorithm>

namespace cpp_reflection {

reflection_manager&
reflection_manager::get_instance(void) {
    static reflection_manager instance;

    return instance;
}

void
reflection_manager::_register_reflectable(const reflectable_base* reflectable) {
    m_types.push_back(reflectable);
}

void
reflection_manager::register_reflectable(const reflectable_base* reflectable) {
    get_instance()._register_reflectable(reflectable);
}

void
reflection_manager::make_reflection(const std::string& class_name, const std::string& function_name) {
    get_instance()._make_reflection(class_name, function_name);
}

void
reflection_manager::_make_reflection(const std::string& class_name, const std::string& function_name) {
    auto it = std::find_if(m_types.begin(), m_types.end(), [class_name](const auto& type) {
        return type->get_name() == class_name;
    });

    if (it == m_types.end())
        throw reflection_exception("Class " + class_name + " is not registered as reflectable.");

    (*it)->call(function_name);
}

} //! cpp_reflection
