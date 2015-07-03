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

} //! cpp_reflection
