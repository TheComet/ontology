#include <tests/Config.hpp>
#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>

#define NAME Entity

using namespace ontology;

// ----------------------------------------------------------------------------
// test fixtures
// ----------------------------------------------------------------------------

struct TestComponent : public Component
{
    TestComponent(int x, int y) : x(x), y(y) {}
    int x, y;
};
inline bool operator==(const TestComponent& lhs, const TestComponent& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct NonExistingComponent : public Component
{
};

// insert ourselves between System events so the components can be upcast
// to TestComponent. This is so they can be tested.
class MockEntityManagerHelper : public EntityManagerInterface
{
    void notifyAddComponent(Entity& e, const Component* c) const override
    { this->notifyAddComponentHelper(e, static_cast<const TestComponent*>(c)); }
    void notifyRemoveComponent(Entity& e, const Component* c) const override
    { this->notifyRemoveComponentHelper(e, static_cast<const TestComponent*>(c)); }

    // WARNING: DO NOT CALL THIS FUNCTION - It is required to be implemented
    // by the base class (abstract function) but doesn't do what it is intended
    // to. This is only here to make the compiler happy.
    Entity& createEntity(std::string name) override { return e; }
    Entity& getEntity(Entity::ID) override { return e; }

    Entity e; // dummy entity
    World w; // dummy world

public:
    MockEntityManagerHelper() : EntityManagerInterface(&w), e("dont_call_this", this) {}

    virtual ~MockEntityManagerHelper() {}
    virtual void notifyAddComponentHelper(Entity&, const TestComponent*) const = 0;
    virtual void notifyRemoveComponentHelper(Entity&, const TestComponent*) const = 0;
};

struct MockEntityManager : public MockEntityManagerHelper
{
    MOCK_METHOD1(destroyEntity, void(Entity&));
    MOCK_METHOD1(destroyEntities, void(const char*));
    MOCK_METHOD0(destroyAllEntities, void());
    MOCK_CONST_METHOD2(notifyAddComponentHelper, void(Entity&, const TestComponent*));
    MOCK_CONST_METHOD2(notifyRemoveComponentHelper, void(Entity&, const TestComponent*));
};

struct TestSystem : public System
{
    void initialise() override {}
    void processEntity(Entity&) override {}
    MOCK_METHOD2(configureEntity, void(Entity&, std::string));
};
