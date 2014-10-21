#include <tests/Config.hpp>
#include <gmock/gmock.h>
#include <ontology/Ontology.hpp>
#define NAME EntityManager

using namespace Ontology;

// ----------------------------------------------------------------------------
// test fixture
// ----------------------------------------------------------------------------

struct TestComponent : public Component
{
    TestComponent(int x, int y) : x(x), y(y) {}
    int x, y;
    bool operator==( const TestComponent& other) const
    {
        return x == other.x && y == other.y;
    }
};

// insert ourselves between listener calls and upcast returned components
// so they can be tested.
class ListenerHelper : public EntityManagerListener
{
    void onCreateEntity(Entity& e)
    { this->onCreateEntityHelper(e);}
    void onDestroyEntity(Entity& e)
    { this->onDestroyEntityHelper(e);}
    void onAddComponent(Entity& e, const Component* c)
    { this->onAddComponentHelper(e, static_cast<const TestComponent*>(c));}
    void onRemoveComponent(Entity& e, const Component* c)
    { this->onRemoveComponentHelper(e, static_cast<const TestComponent*>(c));}
    void onEntitiesReallocated(std::vector<Entity>& l)
    { this->onEntitiesReallocatedHelper(l);}
public:
    virtual void onCreateEntityHelper(Entity&) {}
    virtual void onDestroyEntityHelper(Entity&) {}
    virtual void onAddComponentHelper(Entity&, const TestComponent*) {}
    virtual void onRemoveComponentHelper(Entity&, const TestComponent*) {}
    virtual void onEntitiesReallocatedHelper(std::vector<Entity>&) {}
};

struct MockEntityManagerListener : public ListenerHelper
{
    MOCK_METHOD1(onCreateEntityHelper, void(Entity&));
    MOCK_METHOD1(onDestroyEntityHelper, void(Entity&));
    MOCK_METHOD2(onAddComponentHelper, void(Entity&, const TestComponent*));
    MOCK_METHOD2(onRemoveComponentHelper, void(Entity&, const TestComponent*));
    MOCK_METHOD1(onEntitiesReallocatedHelper, void(std::vector<Entity>&));
};