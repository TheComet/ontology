#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME EntityManager

using namespace Ontology;

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct TestComponent : public Component
{
    TestComponent(int x, int y) : x(x), y(y) {}
    int x, y;
    bool operator==( const TestComponent& other) const
    {
        return x == other.x && y == other.y;
    }
};

// insert ourselves between listener calls and upcast returned components
// so they can be tested.
class ListenerHelper : public EntityManagerListener
{
    void onCreateEntity(Entity& e)
    { this->onCreateEntityHelper(e);}
    void onDestroyEntity(Entity& e)
    { this->onDestroyEntityHelper(e);}
    void onAddComponent(Entity& e, const Component* c)
    { this->onAddComponentHelper(e, static_cast<const TestComponent*>(c));}
    void onRemoveComponent(Entity& e, const Component* c)
    { this->onRemoveComponentHelper(e, static_cast<const TestComponent*>(c));}
    void onEntitiesReallocated(std::vector<Entity>& l)
    { this->onEntitiesReallocatedHelper(l);}
public:
    virtual void onCreateEntityHelper(Entity&) {}
    virtual void onDestroyEntityHelper(Entity&) {}
    virtual void onAddComponentHelper(Entity&, const TestComponent*) {}
    virtual void onRemoveComponentHelper(Entity&, const TestComponent*) {}
    virtual void onEntitiesReallocatedHelper(std::vector<Entity>&) {}
};

struct MockEntityManagerListener : public ListenerHelper
{
    MOCK_METHOD1(onCreateEntityHelper, void(Entity&));
    MOCK_METHOD1(onDestroyEntityHelper, void(Entity&));
    MOCK_METHOD2(onAddComponentHelper, void(Entity&, const TestComponent*));
    MOCK_METHOD2(onRemoveComponentHelper, void(Entity&, const TestComponent*));
    MOCK_METHOD1(onEntitiesReallocatedHelper, void(std::vector<Entity>&));
};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, DispatchesCreateAndDestroyEntityEvents)
{
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager();
    em->event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(testing::AtLeast(0));

    // interesting calls
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(1);
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(1);

    Entity& e = em->createEntity("entity");
    ASSERT_EQ(1, em->getEntityList().size());
    em->destroyEntity(e);
    em->event.removeListener("mock");
    delete em;
}

TEST(NAME, DispatchesDestroyEntityEventOnDestruction)
{
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager();
    em->event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(testing::AtLeast(0));

    // interesting calls
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(1);

    Entity& e = em->createEntity("entity");
    delete em;
}

TEST(NAME, DestroyEntitiesByName)
{
    MockEntityManagerListener mock;
    EntityManager em;
    em.event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(testing::AtLeast(0));

    // interesting calls
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(3);
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(2);

    Entity& e1 = em.createEntity("entity");
    Entity& e2 = em.createEntity("entity");
    Entity& e3 = em.createEntity("keep me");
    ASSERT_EQ(3, em.getEntityList().size());
    em.destroyEntities("entity");
    ASSERT_EQ(1, em.getEntityList().size());
    em.event.removeListener("mock");
}

TEST(NAME, AddingAndRemovingComponentsDispatchesEvents)
{
    MockEntityManagerListener mock;
    EntityManager em;
    em.event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(testing::AtLeast(0));

    // interesting calls
    EXPECT_CALL(mock, onAddComponentHelper(testing::_, testing::Pointee(testing::Eq(TestComponent(336, 743)))))
        .Times(1);
    EXPECT_CALL(mock, onRemoveComponentHelper(testing::_, testing::Pointee(testing::Eq(TestComponent(254, 8376)))))
        .Times(1);

    Entity& e = em.createEntity("entity");
    e.addComponent<TestComponent>(336, 743);
    e.getComponent<TestComponent>().x = 254;
    e.getComponent<TestComponent>().y = 8376;
    e.removeComponent<TestComponent>();
    em.event.removeListener("mock");
}

TEST(NAME, RemoveComponentEventDispatchesOnDestruction)
{
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager();
    em->event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(testing::AtLeast(0));

    // interesting calls
    EXPECT_CALL(mock, onAddComponentHelper(testing::_, testing::Pointee(TestComponent(336, 743))))
        .Times(1);
    EXPECT_CALL(mock, onRemoveComponentHelper(testing::_, testing::Pointee(TestComponent(336, 743))))
        .Times(1);

    Entity& e = em->createEntity("entity");
    e.addComponent<TestComponent>(336, 743);
    delete em;
}

TEST(NAME, DestroyingEntitiesDispatchesReallocationEvent)
{
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager();
    em->event.addListener(&mock, "mock");

    // uninteresting calls
    EXPECT_CALL(mock, onCreateEntityHelper(testing::_))
        .Times(testing::AtLeast(0));
    EXPECT_CALL(mock, onDestroyEntityHelper(testing::_))
        .Times(testing::AtLeast(0));

    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_)).Times(testing::AtLeast(0)); // ignore this call during entity creation - not important
    em->createEntity("entity1");             // will be destroyed via destroyEntities()
    em->createEntity("entity2");             // will be via destroyAllEntities

    // expect 4 calls, since 4 entities are being destroyed
    EXPECT_CALL(mock, onEntitiesReallocatedHelper(testing::_))
        .Times(4);  // 4 entities were created...

    em->destroyEntities("entity1");
    em->destroyAllEntities();
    Entity& e = em->createEntity("entity3");
    em->destroyEntity(e);
    em->createEntity("entity4"); // destroyed by destructor
    delete em;
}
