#include <tests/TestFixture_Entity.hpp>

#ifdef TESTS_WITH_ASSERT

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, GettingNonExistingEntityThrowsInvalidEntityException)
{
    EntityManager em;
    ASSERT_DEATH(em.getEntity(0), "");
}

#endif // TESTS_WITH_EXCEPTIONS