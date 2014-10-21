#include <tests/TestFixture_Entity.hpp>

#ifdef TESTS_WITH_EXCEPTIONS

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, GettingNonExistingEntityThrowsInvalidEntityException)
{
    EntityManager em;
    ASSERT_THROW(em.getEntity(0), InvalidEntityException);
}

#endif // TESTS_WITH_EXCEPTIONS