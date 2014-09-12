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
3. Open a new terminal and go to Artemis-Cpp's folder.
4. Run this code:
   ```
   mkdir build.
   cd build.
   cmake ..
   make
   sudo make install.
   ```
5. Enjoy.

**Note:** You may need to use the ```-G``` option with CMake when generating project files. For a list of generators, type ```cmake --help```. Depending on what system you're on the required generator will vary. On linux for instance you'd use ```cmake -G "Unix Makefiles" ..```

Example
=======
Top Level View
--------------
Ontology tries to make it easy to tie different components and systems in a program together. The following demonstrates what a 2D game might look like:
``` cpp
int main()
{
	// The world holds everything together
	Ontology::World world;

	// register various systems with the world
	world.getSystemManager()
		.addSystem(new MovementSystem,
			Ontology::SupportsComponents<
				Position,
				Velocity
				>())
		.addSystem(new RenderSystem,
			Ontology::SupportsComponents<
				Position,
				Sprite
				>())
		.addSystem(new InputSystem)
		.addSystem(new MainLoop,
			Ontology::SupportsComponents<
				Ontology::None>())
		.initialise()
		;

	// create an entity in the world
	world.getEntityManager().createEntity("Player")
		.addComponent(new Position(0, 0))
		.addComponent(new Velocity(0, 0))
		.addComponent(new Sprite("res/player.png"))
		;

	// run game
	world.getSystem<MainLoop>().execute();

	return 0;
}
```
A unique feature of Ontology is the ability to filter which entities are processed by which systems. When creating a system, one simply has to declare which components the system supports. That system will then only receive entities that actually have all of the required components. Declaring nothing, such as with the InputSystem in the example above, means that the system will receive **all** entities. If a system shouldn't receive any at all, one must declare Ontology::None.

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

Communication between systems
-----------------------------
(TODO)
