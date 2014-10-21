#include <tests/TestFixture_Entity.hpp>

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, CreateEntityWithName)
{
    MockEntityManager em;
    Entity entity("entity1", &em);
    ASSERT_EQ(std::string("entity1"), std::string(entity.getName()));
}

TEST(NAME, GUIDIncrementsCorrectly)
{
    MockEntityManager em;
    // Entity GUIDs rely on a global static variable. Reset it so we know what
    // to expect
    Entity::GUIDCounter = 0;
    Entity entity1("entity1", &em);
    Entity entity2("entity2", &em);
    Entity entity3("entity3", &em);
    
    EXPECT_EQ(0, entity1.getID());
    EXPECT_EQ(1, entity2.getID());
    EXPECT_EQ(2, entity3.getID());
}

TEST(NAME, AddingAndRemovingComponentsInformsEntityManager)
{
    MockEntityManager em;
    Entity entity("entity", &em);

    // Listeners should be informed by addComponent and removeComponent events
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::Pointee(TestComponent(2, 3))))
        .Times(1);
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::Pointee(TestComponent(12, 3))))
        .Times(1);

    entity.addComponent<TestComponent>(2, 3);
    entity.getComponent<TestComponent>().x = 12;
    entity.removeComponent<TestComponent>();
}

TEST(NAME, RemovingNonExistingComponentDoesNothing)
{
    MockEntityManager em;
    Entity entity("entity", &em);

    // uninteresting calls
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));

    entity.removeComponent<NonExistingComponent>();
}

TEST(NAME, EntityDestructionInformsEntityManagerAboutComponentRemoval)
{
    MockEntityManager em;
    Entity entity("entity", &em);

    // uninteresting calls
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));

    // listeners should be informed about component removal upon destruction
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::Pointee(TestComponent(2, 4))))
        .Times(1);

    entity.addComponent<TestComponent>(2, 4);
}

TEST(NAME, CheckForSupportedSystemsUpdatesWhenComponentsAreAddedOrRemoved)
{
    MockEntityManager em;
    TestSystem supported; supported.supportsComponents<TestComponent>();
    TestSystem unsupported; unsupported.supportsComponents<None>();
    Entity entity("entity", &em);

    // uninteresting calls
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));

    ASSERT_EQ(true, !entity.supportsSystem(supported));
    ASSERT_EQ(true, !entity.supportsSystem(unsupported));

    entity.addComponent<TestComponent>(5, 5);

    ASSERT_EQ(true, entity.supportsSystem(supported));
    ASSERT_EQ(true, !entity.supportsSystem(unsupported));

    entity.removeComponent<TestComponent>();

    ASSERT_EQ(true, !entity.supportsSystem(supported));
    ASSERT_EQ(true, !entity.supportsSystem(unsupported));
}
