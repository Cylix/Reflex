# cpp_reflection
Simple reflection system in C++.

## Requirements
* C++14
* Boost.PP includes

## Compiling
The library uses `cmake`. In order to build the library, follow these steps:

```bash
git clone https://github.com/Cylix/cpp_reflection.git
cd cpp_reflection
mkdir build
cd build
cmake .. # -DBUILD_TESTING=true for building tests
make -j
```

Then, you just have to link the `cpp_reflection` library with your project.

## Register class
In order to enable reflection, we must explicitly "register" our class.

For this, `cpp_reflection` provides a simple macro: `REGISTER_CLASS_FUNCTIONS(type, list of functions)`.
* `Type` is the type on which we want to enable reflection
* `List of functions` is the list of member functions (non-static and static member functions) we want to be able to call during reflection. These functions can have any signature (there is no restriction concerning the return value type, the number of parameters and the types of these parameters).

The list of functions must be formatted in a specific way: `(fct1)(fct2)(fct3)...`.
This format is the format used by Boost.PP (which is used in this library) and must be respected in order to compile.

For example, if we have the following class:

```cpp
class SomeClass {
public:
    int fct(void) {
        std::cout << "fct()" << std::endl;
    }

    static void other_fct(const std::string&, float) {
        std::cout << "other_fct()" << std::endl;
    }
};
```

We just need to do `REGISTER_CLASS_FUNCTIONS(SomeClass, (fct)(other_fct))` and this will register `SomeClass` and its two member functions for reflection.

## Register C-Style functions
Reflection is not only limited to class member functions. It can be also used on C-style functions.

For this, `cpp_reflection` provides a simple macro: `REGISTER_FUNCTIONS(list of functions)`.
* `List of functions` is the list of functions we want to be able to call during reflection. These functions can have any signature (there is no restriction concerning the return value type, the number of parameters and the types of these parameters).

The list of functions must be formatted in a specific way: `(fct1)(fct2)(fct3)...`.
This format is the format used by Boost.PP (which is used in this library) and must be respected in order to compile.

For example, if we have the following class:

```cpp
int fct(void) {
    std::cout << "fct()" << std::endl;
}

void other_fct(const std::string&, float) {
    std::cout << "other_fct()" << std::endl;
}
};
```

We just need to do `REGISTER_FUNCTIONS((fct)(other_fct))` and this will register the two functions for reflection.

## Making Reflection
Each time we register a type and its member functions, it stores this type into the `reflection_manager` singleton class.

This class is the class which does the reflection. By calling `reflection_manager::invoke<RetVal, Params...>("class_name", "function_name", ...)`, this will call `class_name::function_name` (on a new object).

A facility function with a more elegant syntax is also provided: `reflection_maker<RetVal(Params...)>::invoke("class_name", "function_name", ...)`. It provides the std::function template syntax which is more readable.

If we take the previous example, by calling `cpp_reflection::reflection_maker<void(const std::string&, float)>::invoke("SomeClass", "other_fct", some_str, some_float);`, we will invoke `SomeClass::fct`.

`reflection_maker::invoke` is overloaded for C-Style functions: `reflection_maker<RetVal(Params...)>::invoke("function_name", ...)`.

## Making Reflection with instance

By default, reflection for member function is done on a new object.

It is however possible to make reflection on a custom object instance.

`cpp_reflection::reflection_maker<void(const std::string&, float)>::invoke(&some_obj, "SomeClass", "other_fct", some_str, some_float);` will invoke `SomeClass::other_fct` on some_obj instance.

For the moment, this feature only work for raw pointer to the object. Making the same thing with copy, ref, shared_ptr and unique_ptr is under development.

# How does it work
`REGISTER_CLASS_FUNCTIONS` and `REGISTER_FUNCTIONS` macros are based on the `REGISTER_REFLECTABLE` macro
The `REGISTER_REFLECTABLE` macro uses variadic macro parameters and works with Boost.PP in order to iterate through the list of functions.

This macro will create a static object of type `reflectable<SomeClass>`.
For example `REGISTER_CLASS_FUNCTIONS(SomeClass, (fct)(other_fct))` will generates the following code:

```cpp
static reflectable<SomeClass> reflectable_SomeClass(
    "SomeClass",
    { "fct", &SomeClass::fct },
    { "other_fct", &SomeClass::other_fct }
);
```

Another example: `REGISTER_FUNCTIONS((fct)(other_fct))` will generates the following code:

```cpp
static reflectable<cpp_reflection::Void> reflectable_(
    "",
    { "fct", &fct },
    { "other_fct", &other_fct }
);
```


This generation is done at compile time but the registration is only effective at runtime at the beginning of the execution. Registering a class for reflection will impact on the compilation time and at program startup (when all static variables are initialized).

The constructor of a reflectable object registers itself into the reflectable_manager that we can use for reflection.

## Example
```cpp
#include <iostream>

#include "cpp_reflection/cpp_reflection.hpp"

//! some class with member functions and static member function
class SomeClass {
public:
    unsigned int nb;
    SomeClass() : nb(42) {}

    void set_nb(unsigned int n) {
        std::cout << "set to " << n << std::endl;
        nb = n;
    }

    unsigned int get_nb(void) const {
        return nb;
    }

    int add(int nb1, int nb2) {
        std::cout << "add(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 + nb2;
    }

    int sub(int nb1, int nb2) {
        std::cout << "sub(" << nb1 << ", " << nb2 << ")" << std::endl;
        return nb1 - nb2;
    }

    static std::string concat(const std::string& str, unsigned int nb) {
        std::cout << "concat(" << str << ", " << nb << ")" << std::endl;
        return str + std::to_string(nb);
    }
};

//! register this class and its functions
REGISTER_CLASS_FUNCTIONS(SomeClass, (add)(sub)(concat)(get_nb)(set_nb))

//! some c-style functions
int basic_fct_1(float f, char c) {
    std::cout << "basic_fct_1(" << f << ", " << c << ")" << std::endl;
    return 42;
}

void basic_fct_2(void) {
    std::cout << "basic_fct_2()" << std::endl;
}

//! register c-style functions
REGISTER_FUNCTIONS((basic_fct_1)(basic_fct_2))

int main(void) {
    //! reflection on member functions
    auto res1 = cpp_reflection::reflection_maker<int(int, int)>::invoke("SomeClass", "add", 30, 12);
    std::cout << res1 << std::endl;

    auto res2 = cpp_reflection::reflection_maker<int(int, int)>::invoke("SomeClass", "sub", 44, 2);
    std::cout << res2 << std::endl;

    auto res3 = cpp_reflection::reflection_maker<std::string(const std::string&, unsigned int)>::invoke("SomeClass", "concat", std::string("hello"), 42);
    std::cout << res3 << std::endl;

    //! reflection on c-style functions
    auto res4 = cpp_reflection::reflection_maker<int(float, char)>::invoke("basic_fct_1", 4.2, 'z');
    std::cout << res4 << std::endl;

    cpp_reflection::reflection_maker<void()>::invoke("basic_fct_2");

    //! reflection on member functions with custom object instance
    SomeClass s;
    auto res5 = cpp_reflection::reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res5 << std::endl;

    cpp_reflection::reflection_maker<void(unsigned int)>::invoke(&s, "SomeClass", "set_nb", 1234);

    auto res6 = cpp_reflection::reflection_maker<unsigned int()>::invoke(&s, "SomeClass", "get_nb");
    std::cout << res6 << std::endl;

    return 0;
}
```

## To Do

| Type | Description | Priority | Status |
|------|-------------|----------|--------|
| Improvement | Improve test coverage | High | To Do |
| Improvement | Add full documentation | Moderate | To Do |
| Improvement | Clear syntax for templates (<ReturnType, Params...> -> <ReturnType(Params)> whenever it is possible) | Low | To Do |
| Improvement | Handling case of multiple REGISTER_FUNCTIONS (or multiple REGISTER_CLASS_FUNCTIONS for same type) in a single project | Moderate | To Do |
| BugFix | Handle reflection for namespaced classes | High | To Do |
| Feature | Reflection for variables | Low | To Do |
| Feature | Reflection for overloads | Medium | To Do |

## Author
[Simon Ninon](http://simon-ninon.fr)
