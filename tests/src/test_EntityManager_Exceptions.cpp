#include <tests/TestFixture_Entity.hpp>

#ifdef TESTS_WITH_EXCEPTIONS

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, GettingNonExistingEntityThrowsInvalidEntityException)
{
    World w;
    EntityManager em(&w);
    ASSERT_THROW(em.getEntity(0), InvalidEntityException);
}

#endif // TESTS_WITH_EXCEPTIONS