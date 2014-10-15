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

TEST(NAME, 
