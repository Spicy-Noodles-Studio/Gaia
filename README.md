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
- To **create a scene**, you just have to go to **Assets/Scenes**, create a **<*YourSceneName*>.scene** and go to **Assets/Files/scenes.asset** and write scene location on it. **(Remember that the order matters, the first written path will be the first scene to initialize)**.
	- Scenes are defined in plain text with the following format:
		```
		"SceneName": "Game",
    		"SceneObjects": [
        	{
            		"ObjectType": "BLUEPRINT",
            		"ObjectName": "name",
            		"Tag": tag
            		"BlueprintPath": "blueprintName"
        	},
        	{
            		"ObjectType": "GAMEOBJECT",
            		"ObjectName": "name",
            		"Tag": "tag",
            		"Components": 
            		[
                		{
                    		"ComponentName" : "name",
				"ComponentProperties": [
                        		["attribute" , "value"],
                        		["attributeVector" , "value1 value2 value3"]
                    		]
                		}
            		]
        	},
		```
- To create a blueprint, you have to go to **Assets/Blueprints**, create a **<*YourBluepirntName*>.blueprint** and go to **Assets/Files/blueprints.asset** and register the blueprint location on it.
- To **create a component**, in your own game project, create a class that inherits from UserComponent. There you can define for your game logic:
	- `awake()`
	- `start()`
	- `preUpdate(float deltaTime)`
	- `update(float deltaTime)`
	- `postUpdate(float deltaTime)`
	- more
- Once you create a component, **remember to register it** using 'REGISTER_FACTORY(YourComponentNameID);' on the .cpp so that Gaia recognizes your component.
- To **create a sound**, save any kind of sound file (.mp3, .wav, .ogg, etc) in your **Assets/Sounds** folder, and register it's location on **Assets/Files/sounds.asset** for it to be loaded into the project.
- To **create an interface or menu**, you have to create a .layout file in the **Assets/UI/layouts** folder. It will be loaded automaticaly so that you only need to call it's route inside the code.
	- The Interface System is a wrapper of the CEGUI library. For further refining, documentation, tutorials or guides, you can check [the CEGUI wiki](http://cegui.org.uk/wiki/).
- To load **meshes and skeletons**, you can create them from an external tool such as Blender, and then store them in the **Assets/Meshes** folder.
	- **Animations** are defined within the .skeleton and are called from the program via the **Animator** component, pre-built into GAIA.
- To load **textures**, save the image file (preferably as .png) inside **Assets/Textures**.
- To create **materials, shaders, trails and particles**, GAIA follows the OpenGL/Ogre3D standard, which can be checked in the [Ogre3D wiki]() for further reference. To load them, just save the **.material**, **.particle**, **.trail** or shader files (**.glsl, .compositor, .material**) inside of their respective folders in **Assets**.

# Licenses
GAIA Engine is powered by the following tools: 
- Ogre3D (Copyright (c) 2000-2013 [Torus Knot Software Ltd](https://www.ogre3d.org/))
- Bullet Continuous Collision Detection and Physics Library (Copyright (c) 2003-2006 [Erwin Coumans](https://pybullet.org/Bullet/phpBB3/))
- FMOD Studio (Copyright (c) [Firelight Technologies Pty Ltd.](https://www.fmod.com/))
- SDL 2.0 (Copyright (c) [SDL Community](http://hg.libsdl.org/SDL))
- CEGUI (Copyright (c) [The CEGUI Team](http://cegui.org.uk/wiki/))