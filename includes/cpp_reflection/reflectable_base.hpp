#pragma once

namespace cpp_reflection {

class reflectable_base {
public:
    //! ctor & dotr
    reflectable_base(void) = default;
    virtual ~reflectable_base(void) = default;

    //! copy ctor & assignment operator
    reflectable_base(const reflectable_base& reflectable) = default;
    reflectable_base& operator=(const reflectable_base& reflectable) = default;

public:
    //! returns the name of the registered reflectable class
    virtual const std::string& get_name(void) const = 0;

    //! call member function by string name
    virtual void call(const std::string& function_name) const = 0;

    //! is member function registered
    virtual bool is_registered(const std::string& function_name) const = 0;
};

} //! cpp_reflection
