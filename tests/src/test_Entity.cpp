#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME Entity

using namespace Ontology;

namespace Ontology {
    class Entity;
    class Component;
}

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct MockEntityManager : public EntityManagerInterface
{
    Entity& createEntity(const char*) {}
    MOCK_METHOD1(destroyEntity, void(Entity*));
    MOCK_METHOD1(destroyEntities, void(const char*));
    MOCK_CONST_METHOD2(informAddComponent, void(Entity*, const Component*));
    MOCK_CONST_METHOD2(informRemoveComponent, void(Entity*, const Component*));
};

struct TestComponent : public Component
{
    TestComponent(int x, int y) : x(x), y(y) {}
    int x, y;
};
inline bool operator==(const TestComponent& lhs, const TestComponent& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct NonExistingComponent : public Component
{
};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, CreateEntityWithName)
{
    MockEntityManager entityManager;
    Entity entity("entity1", &entityManager);
    ASSERT_EQ(std::string("entity1"), std::string(entity.getName()));
}

TEST(NAME, AddingAndRemovingComponentsInformsEntityManager)
{
    MockEntityManager entityManager;
    TestComponent* component = new TestComponent(2, 3);
    Entity entity("entity", &entityManager);

    EXPECT_CALL(entityManager, informAddComponent(&entity, component))
        .Times(1);
    EXPECT_CALL(entityManager, informRemoveComponent(&entity, component))
        .Times(1);

    entity.addComponent(component); // entity now owns the component, no need to delete
    entity.removeComponent<TestComponent>();
}

TEST(NAME, AddingTwoComponentsOfTheSameTypeCausesDeath)
{
    MockEntityManager entityManager;
    Entity entity("entity", &entityManager);
    EXPECT_CALL(entityManager, informAddComponent(&entity, testing::_))
        .Times(1);

    entity.addComponent(new TestComponent(2, 3));

    ASSERT_DEATH(entity.addComponent(new TestComponent(4, 5)), "");
}

TEST(NAME, GettingNonExistingComponentsCausesDeath)
{
    MockEntityManager entityManager;
    Entity entity("entity", &entityManager);
    EXPECT_CALL(entityManager, informAddComponent(&entity, testing::_))
        .Times(1);
    entity.addComponent(new TestComponent(2, 3));

    ASSERT_EQ(TestComponent(2, 3), entity.getComponent<TestComponent>());
    ASSERT_DEATH(entity.getComponent<NonExistingComponent>(), "");
}
