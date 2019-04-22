// ----------------------------------------------------------------------------
// ComponentFactory.hxx
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// include files

#include "ontology/Config.hpp"
#include "ontology/Type.hpp"

#include <vector>
#include <tuple>

// ----------------------------------------------------------------------------
// forward declarations

namespace ontology {
    class Entity;
    class EntityManager;
}

namespace ontology {

class ComponentFactoryInterface
{
public:
    ComponentFactoryInterface(EntityManager* entityManager);
    virtual ~ComponentFactoryInterface() {}

    virtual Index createComponentForEntityWithDefaultArgs(const Entity& entity) = 0;
    virtual void destroyComponentForEntity(const Entity& e) = 0;

    void addRef();
    bool subRef();

protected:
    EntityManager* entityManager_;

private:
    uint32_t refcount = 1;
};

template <class T>
class ComponentFactory : public ComponentFactoryInterface
{
public:
    ComponentFactory(EntityManager* entityManager);

    template <class... Args>
    Index createComponentForEntityWithCustomArgs(const Entity& entity, Args&&... args);
    Index createComponentForEntityWithDefaultArgs(const Entity& entity) override;
    void destroyComponentForEntity(const Entity& e) override;

    T& getComponentForEntity(const Entity& e);
    T& getComponent(Index componentIndex);

protected:
    std::vector<T> components_;
};

template <class T, class... DefaultArgs>
class DefaultComponentFactory : public ComponentFactory<T>
{
public:
    DefaultComponentFactory(EntityManager* entityManager, DefaultArgs&&... args);
    Index createComponentForEntityWithDefaultArgs(const Entity& entity) override final;

private:
    std::tuple<DefaultArgs...> defaultArgs_;
};

}
