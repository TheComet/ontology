#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME System

using namespace Ontology;

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct MockSystem : public System
{
    MOCK_METHOD0(initialise, void());
    MOCK_METHOD1(processEntity, void(Entity&));
    MOCK_METHOD2(configureEntity, void(Entity&, std::string));
};

class TestEntityManager : public EntityManagerInterface
{
    // WARNING: DO NOT CALL THIS FUNCTION - It is required to be implemented
    // by the base class (abstract function) but doesn't do what it is intended
    // to. This is only here to make the compiler happy.
    Entity& createEntity(const char* name) override { return e; }
    Entity& getEntity(Entity::ID) override { return e; }
    Entity e;
    
    World w; // dummy world
    
    // uninteresting, implement abstracts
    void destroyEntity(Entity&) override {}
    void destroyEntities(const char*) override {}
    void destroyAllEntities() override {}
    void informAddComponent(Entity&, const Component*) const override {}
    void informRemoveComponent(Entity&, const Component*) const override {}
public:
    TestEntityManager() : EntityManagerInterface(&w), e("dont_call_this", this) {}
};

#define OVERRIDE_NECESSARY void processEntity(Entity&) override {} void initialise() override {} 
struct DependencySystem1 : public System { OVERRIDE_NECESSARY };
struct DependencySystem2 : public System { OVERRIDE_NECESSARY };
struct NonDependingSystem : public System { OVERRIDE_NECESSARY };

struct SupportedComponent1 : public Component {};
struct SupportedComponent2 : public Component {};
struct UnsupportedComponent : public Component {};

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

TEST(NAME, RecievesWorld)
{
    World world;
    MockSystem system;
    system.setWorld(&world);

    ASSERT_EQ(&world, system.world);
}

TEST(NAME, ReceivesSupportedComponents)
{
    MockSystem system;
    system.supportsComponents<
        SupportedComponent1,
        SupportedComponent2>();
    
    ASSERT_NE(system.getSupportedComponents().end(), system.getSupportedComponents().find(&typeid(SupportedComponent1)));
    ASSERT_NE(system.getSupportedComponents().end(), system.getSupportedComponents().find(&typeid(SupportedComponent2)));
    ASSERT_EQ(system.getSupportedComponents().end(), system.getSupportedComponents().find(&typeid(UnsupportedComponent)));
}

TEST(NAME, ReceivesDependingSystems)
{
    MockSystem system;
    system.executesAfter<
        DependencySystem1,
        DependencySystem2>();
    
    ASSERT_NE(system.getDependingSystems().end(), system.getDependingSystems().find(&typeid(DependencySystem1)));
    ASSERT_NE(system.getDependingSystems().end(), system.getDependingSystems().find(&typeid(DependencySystem2)));
    ASSERT_EQ(system.getDependingSystems().end(), system.getDependingSystems().find(&typeid(NonDependingSystem)));
}

TEST(NAME, AcceptsSupportedEntities)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    Entity entity("entity", &em);
    
    ASSERT_EQ(0, system.m_EntityList.size());
    
    system.informEntityUpdate(entity);
    ASSERT_EQ(0, system.m_EntityList.size());
    
    entity.addComponent<SupportedComponent2>();
    system.informEntityUpdate(entity);
    ASSERT_EQ(0, system.m_EntityList.size());
    
    entity.addComponent<SupportedComponent1>();
    system.informEntityUpdate(entity);
    ASSERT_EQ(1, system.m_EntityList.size());
}

TEST(NAME, DoesntAddEntitiesTwiceOnEntityUpdate)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    Entity entity("entity1", &em);
    entity.addComponent<SupportedComponent1>();
    system.informEntityUpdate(entity);
    system.informEntityUpdate(entity);
    ASSERT_EQ(1, system.m_EntityList.size());
}

TEST(NAME, RemovesEntitiesNoLongerSupportedBySystem)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    Entity entity("entity", &em);
    entity.addComponent<SupportedComponent1>();
    
    system.informEntityUpdate(entity);
    ASSERT_EQ(1, system.m_EntityList.size());
    
    entity.removeComponent<SupportedComponent1>();
    system.informEntityUpdate(entity);
    ASSERT_EQ(0, system.m_EntityList.size());
}

TEST(NAME, RemovesEntitiesAboutToBeDestroyed)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    Entity entity("entity", &em);
    entity.addComponent<SupportedComponent1>();
    
    system.informEntityUpdate(entity);
    ASSERT_EQ(1, system.m_EntityList.size());
    
    system.informDestroyedEntity(entity);
    ASSERT_EQ(0, system.m_EntityList.size());
}

TEST(NAME, ReallocatedEntitiesAreReprocessed)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    std::vector<Entity> entityList;
    entityList.push_back(Entity("entity", &em));
    entityList.push_back(Entity("entity", &em));
    entityList[0].addComponent<SupportedComponent1>();
    entityList[1].addComponent<SupportedComponent1>();
    
    system.informEntityUpdate(entityList[0]);
    system.informEntityUpdate(entityList[1]);
    
    ASSERT_EQ(2, system.m_EntityList.size());
    
    system.informEntitiesReallocated(entityList);
    ASSERT_EQ(2, system.m_EntityList.size());
    
    // try removing a component and sending a re-allocation event
    entityList[0].removeComponent<SupportedComponent1>();
    system.informEntitiesReallocated(entityList);
    ASSERT_EQ(1, system.m_EntityList.size());
}

TEST(NAME, UpdatingSystemCallsInheritingClass)
{
    MockSystem system;
    system.supportsComponents<SupportedComponent1>();
    TestEntityManager em;
    Entity entity("entity", &em);
    entity.addComponent<SupportedComponent1>();
    system.informEntityUpdate(entity);
    
    EXPECT_CALL(system, processEntity(testing::_));
    system.update();
}