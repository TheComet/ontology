#include <gmock/gmock.h>
#include "ontology/World.hpp"
#include "ontology/Entity.hpp"
#include "ontology/Exception.hpp"

#define NAME Entity_ComponentMemoryOrganization

using namespace testing;
using namespace ontology;

class NAME : public Test
{
public:
    void SetUp() override {}
    void TearDown() override {}

    World world;
};

struct C1 : public Component
{
    C1(int value=0) : value(value) {}
    int value;
};

struct C2 : public Component
{
    C2(int value=0) : value(value) {}
    int value;
};

struct C3 : public Component
{
    C3(int value=0) : value(value) {}
    int value;
};

TEST_F(NAME, OrderOfComponentsAfterAddingIsSameAsOrderOfEntities)
{
    EntityManager& em = world.getEntityManager();

    ID e1_id = em.createEntity().getID();
    ID e2_id = em.createEntity().getID();
    ID e3_id = em.createEntity().getID();

    Entity& e1 = em.getEntity(e1_id);
    Entity& e2 = em.getEntity(e2_id);
    Entity& e3 = em.getEntity(e3_id);

    e1.addComponent<C1>(55);
    e2.addComponent<C1>(66);
    e3.addComponent<C1>(77);

    e3.addComponent<C2>(77);
    e2.addComponent<C2>(66);
    e1.addComponent<C2>(55);

    e1.addComponent<C3>(55);
    e3.addComponent<C3>(77);
    e2.addComponent<C3>(66);

    EXPECT_THAT(em.getEntity(0).getID(), Eq(e1.getID()));
    EXPECT_THAT(em.getEntity(1).getID(), Eq(e2.getID()));
    EXPECT_THAT(em.getEntity(2).getID(), Eq(e3.getID()));

    auto factoryC1 = em.findComponentFactory<C1>();
    EXPECT_THAT(factoryC1->getComponent(0).value, Eq(e1.getComponent<C1>().value));
    EXPECT_THAT(factoryC1->getComponent(1).value, Eq(e2.getComponent<C1>().value));
    EXPECT_THAT(factoryC1->getComponent(2).value, Eq(e3.getComponent<C1>().value));

    auto factoryC2 = em.findComponentFactory<C2>();
    EXPECT_THAT(factoryC2->getComponent(0).value, Eq(e1.getComponent<C2>().value));
    EXPECT_THAT(factoryC2->getComponent(1).value, Eq(e2.getComponent<C2>().value));
    EXPECT_THAT(factoryC2->getComponent(2).value, Eq(e3.getComponent<C2>().value));

    auto factoryC3 = em.findComponentFactory<C3>();
    EXPECT_THAT(factoryC3->getComponent(0).value, Eq(e1.getComponent<C3>().value));
    EXPECT_THAT(factoryC3->getComponent(1).value, Eq(e2.getComponent<C3>().value));
    EXPECT_THAT(factoryC3->getComponent(2).value, Eq(e3.getComponent<C3>().value));
}
