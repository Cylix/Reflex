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
cmake ..
make -j
```

Then, you just have to link the `cpp_reflection` library with your project.

## Register class
In order to enable reflection, we must explicitly "register" our class.

For this, `cpp_reflection` provides a simple macro: `REGISTER_REFLECTABLE(type, list of functions)`.
* `Type` is the type on which we want to enable reflection
* `List of functions` is the list of member functions we want to be able to call during reflection.

The list of functions must be formatted in a specific way: `(fct1)(fct2)(fct3)...`.
This format is the format used by Boost.PP (which is used in this library) and must be respected in order to compile.

For example, if we have the following class:

```cpp
class SomeClass {
public:
    void fct(void) {
        std::cout << "fct()" << std::endl;
    }

    void other_fct(void) {
        std::cout << "other_fct()" << std::endl;
    }
};
```

We just need to do `REGISTER_REFLECTABLE(SomeClass, (fct)(other_fct))` and this will register `SomeClass` and its two member functions for reflection.

## Making Reflection
Each time we register a type and its member functions, it stores this type into the `reflection_manager` singleton class.

This class is the class which does the reflection. By calling `reflection_manager::make_reflection("class_name", "function_name")`, this will call `class_name::function_name` (on a new object).

If we take the previous example, by calling `cpp_reflection::reflection_manager::make_reflection("SomeClass", "fct");`, we will call `SomeClass::fct`.

# How does it work
The `REGISTER_REFLECTABLE` macro uses variadic macro parameters and works with Boost.PP in order to iterate through the list of member functions.

This macro will create a static object of type `reflectable<SomeClass>`.
For example `REGISTER_REFLECTABLE(SomeClass, (fct)(other_fct))` will generates the following code:

```cpp
static reflectable<SomeClass> reflectable_int("SomeClass", {
    { "fct", &SomeClass::fct },
    { "other_fct", &SomeClass::other_fct }
});
```

This generation is done at compile time but the registration is only effective at runtime at the beginning of the execution. Registering a class for reflection will impact on the compilation time and at program startup (when all static variables are initialized).

The constructor of a reflectable object registers itself into the reflectable_manager that we can use for reflection.

## Example
```cpp
#include <iostream>

#include "cpp_reflection/reflectable.hpp"
#include "cpp_reflection/reflection_manager.hpp"

class SomeClass {
public:
    void fct(void) {
        std::cout << "fct()" << std::endl;
    }

    void other_fct(void) {
        std::cout << "other_fct()" << std::endl;
    }
};

REGISTER_REFLECTABLE(SomeClass, (fct)(other_fct))

int main(void) {
    cpp_reflection::reflection_manager::make_reflection("SomeClass", "fct");
    cpp_reflection::reflection_manager::make_reflection("SomeClass", "other_fct");

    return 0;
}
```

## To Do

* Making reflection on the same object
* Getting a reflectable object
* Providing facility functions (for example, unregister member function or type)
* Passing arguments during reflection [IN PROGRESS]
* Reflection on non-member function
* ...

## Author
[Simon Ninon](http://simon-ninon.fr)
