#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME EntityManager

using namespace Ontology;

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct MockEntityManagerListener : public EntityManagerListener
{
    MOCK_METHOD1(onCreateEntity, void(Entity&));
    MOCK_METHOD1(onDestroyEntity, void(Entity&));
    MOCK_METHOD2(onAddComponent, void(Entity&, const Component*));
    MOCK_METHOD2(onRemoveComponent, void(Entity&, const Component*));
    MOCK_METHOD1(onEntitiesReallocated, void(std::vector<Entity>&));
};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, EntitiesAreCreatedCorrectly)
{
    EntityManager em;
    MockEntityManagerListener mock;
    em.event.addListener(&mock, "mock");
    EXPECT_CALL(mock, onCreateEntity(testing::_))
        .Times(1);
    EXPECT_CALL(mock, onEntitiesReallocated(testing::_))
        .Times(testing::AtLeast(0));
    Entity& e = em.createEntity("entity");
    em.event.removeListener("mock");
}

//TEST(NAME, )
