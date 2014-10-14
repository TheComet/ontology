#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME System

using namespace Ontology;

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct MockSystem : public System
{
    MOCK_METHOD1(processEntity, void(Entity&));
    MOCK_METHOD0(initialise, void());
};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, SystemRecievesWorld)
{
    World world;
    MockSystem system;
    system.setWorld(&world);

    ASSERT_EQ(&world, system.world);
}
