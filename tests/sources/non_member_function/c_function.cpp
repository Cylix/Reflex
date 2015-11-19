#include <gtest/gtest.h>
#include <reflex/reflex>

unsigned int get_nb(void) { return 42; }

void do_something(const std::string&, int& nb, char, float, int nb2) { nb = nb2; }

int add(int nb1, int nb2) { return nb1 + nb2; }

static int sub(int nb1, int nb2) { return nb1 - nb2; }

REGISTER_FUNCTIONS((get_nb)(do_something)(add)(sub))

TEST(CFunction, ReturnGoodValue) {
    EXPECT_EQ(42, reflex::reflection_maker<unsigned int()>::invoke("get_nb"));
}

TEST(CFunction, AcceptMultipleParams) {
    int nb = 0;

    reflex::reflection_maker<void(const std::string&, int&, char, float, int)>::invoke("do_something",
                                                                                       std::string("hello"), nb, 'a', 1.0, 42);
    EXPECT_EQ(42, nb);
}

TEST(CFunction, DoExpectedJob) {
    EXPECT_EQ(42, reflex::reflection_maker<int(int, int)>::invoke("add", 20, 22));
}

TEST(CFunction, StaticCFunction) {
    EXPECT_EQ(42, reflex::reflection_maker<int(int, int)>::invoke("sub", 44, 2));
}

TEST(CFunction, UnregisteredFunction) {
    try {
        reflex::reflection_maker<void()>::invoke("unregistered");
        FAIL() << "Expected reflex::reflection_exception";
    }
    catch (const reflex::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Function ::unregistered is not registered"));
    }
    catch (...) {
        FAIL() << "Expected reflex::reflection_exception";
    }
}

TEST(CFunction, WrongFunctionPrototype) {
    try {
        reflex::reflection_maker<void()>::invoke("get_nb");
        FAIL() << "Expected reflex::reflection_exception";
    }
    catch (const reflex::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Invalid function signature for function ::get_nb"));
    }
    catch (...) {
        FAIL() << "Expected reflex::reflection_exception";
    }
}
