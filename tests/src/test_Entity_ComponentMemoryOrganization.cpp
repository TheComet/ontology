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

    // Create 3 entities, give each 3 components C1, C2, C3
    ID e1_id = em.createEntity().getID();
    ID e2_id = em.createEntity().getID();
    ID e3_id = em.createEntity().getID();

    Entity& e1 = em.getEntity(e1_id);
    Entity& e2 = em.getEntity(e2_id);
    Entity& e3 = em.getEntity(e3_id);

    e1.addComponent<C1>(155);
    e2.addComponent<C1>(166);
    e3.addComponent<C1>(177);

    e3.addComponent<C2>(277);
    e2.addComponent<C2>(266);
    e1.addComponent<C2>(255);

    e1.addComponent<C3>(355);
    e3.addComponent<C3>(377);
    e2.addComponent<C3>(366);

    // When accessing the entity by index from the entity manager, the entities
    // should appear in the order in which they were created
    EXPECT_THAT(em.getEntity(0).getID(), Eq(e1.getID()));
    EXPECT_THAT(em.getEntity(1).getID(), Eq(e2.getID()));
    EXPECT_THAT(em.getEntity(2).getID(), Eq(e3.getID()));

    // Get the 3 component factories for C1, C2 and C3
    auto& factoryC1 = e1.getComponentFactory<C1>();
    auto& factoryC2 = e1.getComponentFactory<C2>();
    auto& factoryC3 = e1.getComponentFactory<C3>();

    // Sanity check. The 3 entities should all be using the same component
    // factories
    EXPECT_THAT(&factoryC1, Eq(&e2.getComponentFactory<C1>()));
    EXPECT_THAT(&factoryC1, Eq(&e3.getComponentFactory<C1>()));
    EXPECT_THAT(&factoryC2, Eq(&e2.getComponentFactory<C2>()));
    EXPECT_THAT(&factoryC2, Eq(&e3.getComponentFactory<C2>()));
    EXPECT_THAT(&factoryC3, Eq(&e2.getComponentFactory<C3>()));
    EXPECT_THAT(&factoryC3, Eq(&e3.getComponentFactory<C3>()));

    EXPECT_THAT(factoryC1.getComponent(0).value, Eq(155));
    EXPECT_THAT(factoryC1.getComponent(1).value, Eq(166));
    EXPECT_THAT(factoryC1.getComponent(2).value, Eq(177));
    EXPECT_THAT(e1.getComponent<C1>().value, Eq(155));
    EXPECT_THAT(e2.getComponent<C1>().value, Eq(166));
    EXPECT_THAT(e3.getComponent<C1>().value, Eq(177));

    EXPECT_THAT(factoryC2.getComponent(0).value, Eq(255));
    EXPECT_THAT(factoryC2.getComponent(1).value, Eq(266));
    EXPECT_THAT(factoryC2.getComponent(2).value, Eq(277));
    EXPECT_THAT(e1.getComponent<C2>().value, Eq(255));
    EXPECT_THAT(e2.getComponent<C2>().value, Eq(266));
    EXPECT_THAT(e3.getComponent<C2>().value, Eq(277));

    EXPECT_THAT(factoryC3.getComponent(0).value, Eq(355));
    EXPECT_THAT(factoryC3.getComponent(1).value, Eq(366));
    EXPECT_THAT(factoryC3.getComponent(2).value, Eq(377));
    EXPECT_THAT(e1.getComponent<C3>().value, Eq(355));
    EXPECT_THAT(e2.getComponent<C3>().value, Eq(366));
    EXPECT_THAT(e3.getComponent<C3>().value, Eq(377));
}

TEST_F(NAME, removingComponentsShiftsDownIndicesOfOtherEntities)
{
    EntityManager& em = world.getEntityManager();

    ID e1_id = em.createEntity().getID();
    ID e2_id = em.createEntity().getID();
    ID e3_id = em.createEntity().getID();

    Entity& e1 = em.getEntity(e1_id);
    Entity& e2 = em.getEntity(e2_id);
    Entity& e3 = em.getEntity(e3_id);

    e1.addComponent<C1>(155);
    e2.addComponent<C1>(166);
    e3.addComponent<C1>(177);
    e1.addComponent<C2>(255);
    e2.addComponent<C2>(266);
    e3.addComponent<C2>(277);
    e1.addComponent<C3>(355);
    e2.addComponent<C3>(366);
    e3.addComponent<C3>(377);

    e1.removeComponent<C1>();  // remove first component
    auto factoryC1 = e1.getComponentFactory<C1>();
    EXPECT_THAT(factoryC1.getComponent(0).value, Eq(166));
    EXPECT_THAT(factoryC1.getComponent(1).value, Eq(177));
    EXPECT_THROW(e1.getComponent<C1>(), InvalidComponentException);
    EXPECT_THAT(e2.getComponent<C1>().value, Eq(166));
    EXPECT_THAT(e3.getComponent<C1>().value, Eq(177));

    e2.removeComponent<C2>();  // remove middle component
    auto factoryC2 = e1.getComponentFactory<C2>();
    EXPECT_THAT(factoryC2.getComponent(0).value, Eq(255));
    EXPECT_THAT(factoryC2.getComponent(1).value, Eq(277));
    EXPECT_THAT(e1.getComponent<C2>().value, Eq(255));
    EXPECT_THROW(e2.getComponent<C2>(), InvalidComponentException);
    EXPECT_THAT(e3.getComponent<C2>().value, Eq(277));

    e3.removeComponent<C3>();  // remove last component
    auto factoryC3 = e1.getComponentFactory<C3>();
    EXPECT_THAT(factoryC3.getComponent(0).value, Eq(355));
    EXPECT_THAT(factoryC3.getComponent(1).value, Eq(366));
    EXPECT_THAT(e1.getComponent<C3>().value, Eq(355));
    EXPECT_THAT(e2.getComponent<C3>().value, Eq(366));
    EXPECT_THROW(e3.getComponent<C3>(), InvalidComponentException);
}

TEST_F(NAME, DestroyEntityUpdatesIndexMap)
{
    EntityManager& em = world.getEntityManager();

    ID e1_id = em.createEntity().getID();
    ID e2_id = em.createEntity().getID();
    ID e3_id = em.createEntity().getID();

    EXPECT_THAT(em.entityIDToIndex(e1_id), Eq(0));
    EXPECT_THAT(em.entityIDToIndex(e2_id), Eq(1));
    EXPECT_THAT(em.entityIDToIndex(e3_id), Eq(2));

    em.destroyEntity(e2_id);

    EXPECT_THAT(em.entityIDToIndex(e1_id), Eq(0));
    EXPECT_THROW(em.entityIDToIndex(e2_id), InvalidEntityException);
    EXPECT_THAT(em.entityIDToIndex(e3_id), Eq(1));
}

TEST_F(NAME, DestroyingEntityShiftsComponentIndicesOfOtherEntities)
{
    EntityManager& em = world.getEntityManager();

    ID e1_id = em.createEntity().getID();
    ID e2_id = em.createEntity().getID();
    ID e3_id = em.createEntity().getID();

    Entity& e1 = em.getEntity(e1_id);
    Entity& e2 = em.getEntity(e2_id);
    Entity& e3 = em.getEntity(e3_id);

    e1.addComponent<C1>(155);
    e2.addComponent<C1>(166);
    e3.addComponent<C1>(177);
    e1.addComponent<C2>(255);
    e2.addComponent<C2>(266);
    e3.addComponent<C2>(277);
    e1.addComponent<C3>(355);
    e2.addComponent<C3>(366);
    e3.addComponent<C3>(377);

    em.destroyEntity(e2);

    EXPECT_THAT(em.getEntity(0).getID(), Eq(e1_id));
    EXPECT_THAT(em.getEntity(1).getID(), Eq(e3_id));

    auto factoryC1 = e1.getComponentFactory<C1>();
    EXPECT_THAT(factoryC1.getComponent(0).value, Eq(155));
    EXPECT_THAT(factoryC1.getComponent(1).value, Eq(177));
    EXPECT_THAT(em.getEntity(e1_id).getComponent<C1>().value, Eq(155));
    EXPECT_THAT(em.getEntity(e3_id).getComponent<C1>().value, Eq(177));

    auto factoryC2 = e1.getComponentFactory<C2>();
    EXPECT_THAT(factoryC2.getComponent(0).value, Eq(255));
    EXPECT_THAT(factoryC2.getComponent(1).value, Eq(277));
    EXPECT_THAT(em.getEntity(e1_id).getComponent<C2>().value, Eq(255));
    EXPECT_THAT(em.getEntity(e3_id).getComponent<C2>().value, Eq(277));

    auto factoryC3 = e1.getComponentFactory<C3>();
    EXPECT_THAT(factoryC3.getComponent(0).value, Eq(355));
    EXPECT_THAT(factoryC3.getComponent(1).value, Eq(377));
    EXPECT_THAT(em.getEntity(e1_id).getComponent<C3>().value, Eq(355));
    EXPECT_THAT(em.getEntity(e3_id).getComponent<C3>().value, Eq(377));
}
