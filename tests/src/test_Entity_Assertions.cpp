#include <tests/TestFixture_Entity.hpp>

#ifdef TESTS_WITH_ASSERT

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, AddingTwoComponentsOfTheSameTypeCausesDeath)
{
    MockEntityManager em;
    Entity entity("entity", &em);

    // uninteresting calls
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));

    // duplicate component should not be added, which means an addComponent event should not
    // be triggered
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::Pointee(TestComponent(2, 3))))
        .Times(1);

    entity.addComponent<TestComponent>(2, 3);

    ASSERT_DEATH(entity.addComponent<TestComponent>(3, 4), "");
}

TEST(NAME, GettingNonExistingComponentsCausesDeath)
{
    MockEntityManager em;
    Entity entity("entity", &em);

    // uninteresting calls
    EXPECT_CALL(em, informRemoveComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));
    EXPECT_CALL(em, informAddComponentHelper(testing::_, testing::_)).Times(testing::AtLeast(0));

    entity.addComponent<TestComponent>(2, 3);
    ASSERT_EQ(TestComponent(2, 3), entity.getComponent<TestComponent>());
    ASSERT_DEATH(entity.getComponent<NonExistingComponent>(), "");
}

#endif // TESTS_WITH_ASSERT