#include <gmock/gmock.h>
#include "ontology/World.hpp"
#include "ontology/Entity.hpp"
#include "ontology/Exception.hpp"

#define NAME Entity_AddAndRemoveComponents

using namespace testing;
using namespace ontology;

class NAME : public Test
{
public:
    void SetUp() override {}
    void TearDown() override {}

    World world;
};

struct C1 : public Component
{
    C1(int value=0) : value(value) {}
    int value;
};

struct C2 : public Component
{
    C2(int value=0) : value(value) {}
    int value;
};

struct C3 : public Component
{
    C3(int value=0) : value(value) {}
    int value;
};

TEST_F(NAME, canAddAndGetComponent)
{
    Entity& e = world.getEntityManager().createEntity();
    ASSERT_THAT(e.hasComponent<C1>(), IsFalse());
    e.addComponent<C1>(5);
    ASSERT_THAT(e.hasComponent<C1>(), IsTrue());
    EXPECT_THAT(e.getComponent<C1>().value, Eq(5));

    ASSERT_THAT(e.hasComponent<C2>(), IsFalse());
    e.addComponent<C2>(7);
    ASSERT_THAT(e.hasComponent<C2>(), IsTrue());
    EXPECT_THAT(e.getComponent<C2>().value, Eq(7));
}

TEST_F(NAME, canRemoveComponent)
{
    Entity& e = world.getEntityManager().createEntity()
        .addComponent<C1>(5)
        .addComponent<C2>(7);

    e.removeComponent<C1>();
    ASSERT_THAT(e.hasComponent<C1>(), IsFalse());
    ASSERT_THAT(e.hasComponent<C2>(), IsTrue());

    e.removeComponent<C2>();
    ASSERT_THAT(e.hasComponent<C1>(), IsFalse());
    ASSERT_THAT(e.hasComponent<C2>(), IsFalse());
}

#if defined(ONTOLOGY_EXCEPTIONS)
TEST_F(NAME, addingDuplicateComponentThrowsException)
{
    EXPECT_THROW(world.getEntityManager().createEntity()
        .addComponent<C1>()
        .addComponent<C1>()
    , DuplicateComponentException);
}
#elif defined(DEBUG)
TEST_F(NAME, addingDuplicateComponentAsserts)
{
    EXPECT_DEATH(world.getEntityManager().createEntity()
        .addComponent<C1>()
        .addComponent<C1>(), "");
}
#endif

#if defined(ONTOLOGY_EXCEPTIONS)
TEST_F(NAME, removingNonExistingComponentThrowsException)
{
    EXPECT_THROW(world.getEntityManager().createEntity().removeComponent<C1>(), InvalidComponentException);
}
#elif defined(DEBUG)
TEST_F(NAME, removingNonExistingComponentAsserts)
{
    EXPECT_DEATH(world.getEntityManager().createEntity().removeComponent<C1>(), "");
}
#endif

#if defined(ONTOLOGY_EXCEPTIONS)
TEST_F(NAME, gettingNonExistingComponentThrowsException)
{
    EXPECT_THROW(world.getEntityManager().createEntity().getComponent<C1>(), InvalidComponentException);
}
#elif defined(DEBUG)
TEST_F(NAME, gettingNonExistingComponentAsserts)
{
    EXPECT_DEATH(world.getEntityManager().createEntity().getComponent<C1>(), "");
}
#endif
