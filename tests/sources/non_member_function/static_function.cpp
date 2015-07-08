#include <gtest/gtest.h>
#include <reflex/reflex.hpp>

class some_class_with_statics {
public:
    static unsigned int get_nb(void) { return 42; }

    static void do_something(const std::string&, int& nb, char, float, int nb2) { nb = nb2; }

    static int add(int nb1, int nb2) { return nb1 + nb2; }
};

REGISTER_CLASS_FUNCTIONS(some_class_with_statics, (get_nb)(do_something)(add))

TEST(StaticFunction, ReturnGoodValue) {
    EXPECT_EQ(42, reflex::reflection_maker<unsigned int()>::invoke("some_class_with_statics", "get_nb"));
}

TEST(StaticFunction, AcceptMultipleParams) {
    int nb = 0;

    reflex::reflection_maker<void(const std::string&, int&, char, float, int)>::invoke("some_class_with_statics", "do_something",
                                                                                       std::string("hello"), nb, 'a', 1.0, 42);
    EXPECT_EQ(42, nb);
}

TEST(StaticFunction, DoExpectedJob) {
    EXPECT_EQ(42, reflex::reflection_maker<int(int, int)>::invoke("some_class_with_statics", "add", 20, 22));
}

TEST(StaticFunction, UnregisteredFunction) {
    try {
        reflex::reflection_maker<void()>::invoke("some_class_with_statics", "unregistered");
        FAIL() << "Expected reflex::reflection_exception";
    }
    catch (const reflex::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Function some_class_with_statics::unregistered is not registered"));
    }
    catch (...) {
        FAIL() << "Expected reflex::reflection_exception";
    }
}

TEST(StaticFunction, UnregisteredClass) {
    try {
        reflex::reflection_maker<void()>::invoke("unregistered_class", "unregistered");
        FAIL() << "Expected reflex::reflection_exception";
    }
    catch (const reflex::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Class unregistered_class is not registered"));
    }
    catch (...) {
        FAIL() << "Expected reflex::reflection_exception";
    }
}


TEST(StaticFunction, WrongFunctionPrototype) {
    try {
        reflex::reflection_maker<void()>::invoke("some_class_with_statics", "get_nb");
        FAIL() << "Expected reflex::reflection_exception";
    }
    catch (const reflex::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Invalid function signature for function some_class_with_statics::get_nb"));
    }
    catch (...) {
        FAIL() << "Expected reflex::reflection_exception";
    }
}
