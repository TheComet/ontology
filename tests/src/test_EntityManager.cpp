#include <tests/TestFixture_EntityManager.hpp>

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

// TODO test if world was set correctly

TEST(NAME, DispatchesCreateAndDestroyEntityEvents)
{
    World w;
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager(&w);
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
    World w;
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager(&w);
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
    World w;
    MockEntityManagerListener mock;
    EntityManager em(&w);
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
    World w;
    MockEntityManagerListener mock;
    EntityManager em(&w);
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
    World w;
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager(&w);
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
    World w;
    MockEntityManagerListener mock;
    EntityManager* em = new EntityManager(&w);
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

TEST(NAME, GetEntityByID)
{
    World w;
    EntityManager em(&w);
    Entity::ID a = em.createEntity("entity1").getID();
    Entity::ID b = em.createEntity("entity2").getID();
    Entity::ID c = em.createEntity("entity3").getID();
    
    ASSERT_EQ(std::string("entity1"), em.getEntity(a).getName());
    ASSERT_EQ(std::string("entity2"), em.getEntity(b).getName());
    ASSERT_EQ(std::string("entity3"), em.getEntity(c).getName());
}