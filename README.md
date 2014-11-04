Ontology
========

C++ entity component framework with listener dispatch system.

Compiling/Installing
====================

CMake
-----
CMake is a cross-platform, open source build system. You can find more information about CMake [here](http://www.cmake.org/).

It is very simple to compile Ontology using CMake. Just follow these steps:

1. Install CMake in your system.
2. Make sure you have cmake in your $PATH variable.
3. Open a new terminal and go to ontology's folder.
4. Execute the following commands:
   - ```mkdir build```
   - ```cd build```
   - ```cmake ..```
   - ```make```
   -```sudo make install```
5. Enjoy.

**Note:** You may need to use the ```-G``` option with CMake when generating project files. For a list of generators, type ```cmake --help```. Depending on what system you're on the required generator will vary. On linux for instance you'd use ```cmake -G "Unix Makefiles" ..```

Example
=======
Top Level View
--------------
Ontology tries to make it easy to tie different components and systems in a program together. It allows you to declare what components a system supports as well as execution dependencies. The following demonstrates what a 2D game might look like:
``` cpp
int main()
{
	// The world holds everything together
	Ontology::World world;

	// register various systems with the world
	world.getSystemManager().addSystem<MovementSystem>()
		.supportsComponents<
			Position,
			Velocity>();
	world.getSystemManager().addSystem<RenderSystem>()
		.supportsComponents<
			Position,
			Sprite>();
	world.getSystemManager().addSystem<InputSystem>();
	world.getSystemManager().addSystem<MainLoop>()
		.supportsComponents<
			Ontology::None>();

	world.getSystemManager().initialise()

	// create an entity in the world
	world.getEntityManager().createEntity("Player")
		.addComponent<Position>(0, 0)
		.addComponent<Velocity>(0, 0)
		.addComponent<Sprite>("res/player.png")
		;

	// run game
	world.getSystem<MainLoop>().execute();

	return 0;
}
```
A unique feature of Ontology is the ability to filter which entities are processed by which systems. When creating a system, one simply has to declare which components the system supports. That system will then only receive entities that actually have all of the required components. Declaring nothing, such as with the InputSystem in the example above, means that the system will receive **all** entities. If a system shouldn't receive any at all, one must declare Ontology::None.

In the above case, the "Player" entity we created would be processed by the following systems:
 - MovementSystem, since MovementSystem requires Position and Velocity and "Player" has them both.
 - RenderSystem, since RenderSystem requires Position and Sprite and "Player" has them both.
 - InputSystem, since by default a system will process all entities.
 - **NOT** MainLoop, since it Ontology::None was specified.

Components
----------
Components hold data and can be added to entities. To create components one must inherit from Ontology::Component:
``` cpp
struct Position : public Ontology::Component
{
	Position(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};
```

Systems
-------
Systems manipulate entities and their components. To create a system one must inherit from Ontology::System:
``` cpp
class MovementSystem : public Ontology::System
{
public:
	virtual void initialise()
	{
		// initialise the system here
	}
	virtual void processEntity(Ontology::Entity& entity)
	{
		// calculate new position based on velocity of entity
		entity.getComponent<Position>().x += entity.getComponent<Velocity>().x * world->getDeltaTime();
		entity.getComponent<Position>().y += entity.getComponent<Velocity>().y * world->getDeltaTime();
	}
};
```

Polymorphic Systems
-------------------
Sometimes you may want to add a polymorphic system. This is just like adding a normal system, but using System::addPolymorphicSystem instead:
``` cpp
class BaseSystem : public OntologySystem
{
public:
	virtual ~BaseSystem() {} // virtual destructor for destroying instances through base class pointer
	virtual void initialise() { /* initialise system */ }
	virtual void processEntity(Ontology::Entity& entity) { /* process entity */ }
};

class DerivedSystem : public BaseSystem
{
};
```

Then, when creating the system and adding it to the world, use:
``` cpp
world.getSystemManager().addPolymorphicSystem<BaseSystem, DerivedSystem>();
```

Communication between systems
-----------------------------
Here you are pretty flexible. Ontology provides a class for implementing the observer pattern if that's your slice of cake, but you could also go with something like Boost.Signals2 (I personally recommend this).

To learn more about the provided observer pattern, an example is provided in the comments section on the file *ontology/ListenerDispatcher.hxx*.
