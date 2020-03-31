![GaiaLogo](Other/gaiaLogoWithTitle.png)

# Description
**Gaia(c)** is a **Spicy Noodles(c)** videogame engine. A simple engine where you can make a game very easily. Gaia works with data oriented programming. This means you just have to compile Gaia once and you will program your game via data (files).

# How to Install
 - Download the most recent release version. **DO NOT** download the master version, only release versions are stable.
 - Open the solution on Visual Studio and compile. It is recommended that both configurations to be compiled. Debug for game debug purposes, and Release for final release version of the game.
 - Once compiled, you should configurate your own game project, including header files, .lib files and .dll file to your project.
 
*****This section must be written with more detail***

# How to Use
 - Next to the executable you should have:
	 - Assets folder (just like in the Gaia project)
	 - window.cfg file
	 - plugins.cfg file
	 - resources.cfg file
	 - resources.asset file

****We recommend to copy the files given in the Gaia version downloaded.**
- If you do as recommended, you should only navigate through the Assets folder and place in its place all kind of resources you need.
- To **create a scene**, you just have to go to **Assets/Scenes** and create a **<*YourSceneName*>.scene** and go to **Assets/Files/scenes.asset** and write scene location on it. **(Remember that the order matters, the first written path will be the first scene to initialize)**
- To create a blueprint, you have to go to **Assets/Blueprints** and create a **<*YourBluepirntName*>.blueprint** and go to **Assets/Files/blueprints.asset** and write scene location on it.
- To **create a component**, in your own game project, create a class that inherits from UserComponent. There you can define for your game logic:
	- `awake()`
	- `start()`
	- `preUpdate(float deltaTime)`
	- `update(float deltaTime)`
	- `postUpdate(float deltaTime)`
	- more
- Once you create a component, **remember to register it** using `ComponentManager::registerComponent<YourComponentType>(YourComponentNameID)` and now Gaia would recognize your component.

***WIP***
*****This section must be written with more detail***

# Licenses
***WIP***
*****This section must be written with more detail***