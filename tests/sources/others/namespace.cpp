#include <gtest/gtest.h>
#include <reflex/reflex>

namespace some_namespace {

class some_namespaced_class {
public:
    unsigned int get_21(void) const { return 21; }
};

unsigned int get_84(void) { return 84; }

namespace nested_namespace {

class other_namespaced_class {
public:
    unsigned int get_21(void) const { return 21; }
};

unsigned int get_84(void) { return 84; }

} //! nested_namespace

} //! some_namespace

REGISTER_CLASS_FUNCTIONS((some_namespace), some_namespaced_class, (get_21))
REGISTER_FUNCTIONS((some_namespace), (get_84))

REGISTER_CLASS_FUNCTIONS((some_namespace)(nested_namespace), some_namespaced_class, (get_21))
REGISTER_FUNCTIONS((some_namespace)(nested_namespace), (get_84))

TEST(NameSpacedFunction, ClassFunction) {
    EXPECT_EQ(21, reflex::reflection_maker<unsigned int()>::invoke("some_namespace::some_namespaced_class", "get_21"));
}

TEST(NameSpacedFunction, CFunction) {
    EXPECT_EQ(84, reflex::reflection_maker<unsigned int()>::invoke("some_namespace", "get_84"));
}

TEST(InnerNameSpacedFunction, ClassFunction) {
    EXPECT_EQ(21, reflex::reflection_maker<unsigned int()>::invoke("some_namespace::nested_namespace::some_namespaced_class", "get_21"));
}

TEST(InnerNameSpacedFunction, CFunction) {
    EXPECT_EQ(84, reflex::reflection_maker<unsigned int()>::invoke("some_namespace::nested_namespace", "get_84"));
}
