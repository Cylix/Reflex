#include "reflex/reflection_manager.hpp"
#include "reflex/reflection_exception.hpp"

#include <algorithm>

namespace reflex {

reflection_manager&
reflection_manager::get_instance(void) {
    static reflection_manager instance;

    return instance;
}

void
reflection_manager::register_reflectable(const reflectable_base& reflectable) {
    m_types.push_back(std::cref(reflectable));
}

} //! reflex
