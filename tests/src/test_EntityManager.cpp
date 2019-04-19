#include <gmock/gmock.h>
#include "ontology/Component.hpp"
#include "ontology/Entity.hpp"
#include "ontology/EntityManager.hpp"
#include "ontology/World.hpp"

#define NAME ontology_EntityManager

using namespace testing;
using namespace ontology;

class NAME : public Test
{
public:
    void SetUp() override {}
    void TearDown() override {}

    World world;
};

struct Vec3i : public Component
{
    Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
    int x, y, z;
};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST_F(NAME, test)
{
    world.getEntityManager().addPrototype("test")
        .addComponent<Vec3i>(1, 2, 3);

    Entity& e = world.getEntityManager().createEntity("test");
    e.getComponent<Vec3i>();
}
