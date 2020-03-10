#include "GaiaCore.h"

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <iostream>

#include "RenderSystem.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Reverb.h"

#include "RigidBody.h"
#include "DebugDrawer.h"
#include "gTime.h"
#include "InputSystem.h"

GaiaCore::GaiaCore()
{
}

GaiaCore::~GaiaCore()
{
	delete root;
	delete obj;
	delete win;
	delete rManager;
}

void GaiaCore::init()
{
#ifdef _DEBUG
	root = new Ogre::Root("plugins_d.cfg", "window_d.cfg");
#else
	root = new Ogre::Root("plugins.cfg", "window.cfg");
#endif

	if (!(root->restoreConfig() || root->showConfigDialog(nullptr)))
		return;

	// Setup window
	Window* win = new Window(root, "Test window - 2020 (c) Gaia ");

	rManager = new ResourcesManager("resources.asset");
	rManager->init();
 
	SoundSystem::GetInstance()->initSystem();
	RenderSystem::GetInstance()->setup(root);
	InputSystem::GetInstance()->init();
	PhysicsSystem::GetInstance()->setup();
	PhysicsSystem::GetInstance()->setWorldGravity({ 0, -1.0f, 0 });
	gTime::GetInstance()->setup();

	aux = new GameObject("Camera", "Cam", nullptr);
	Transform* transform1 = new Transform(aux);
	Camera* cam = new Camera(aux);
	SoundListener* lis = new SoundListener(aux);
	
	//aux->transform->rotate(Vector3(0, -90, 0));

	Ogre::Viewport* vp = win->addViewport(cam->getCamera());

	Light* lz = new Light(aux);
	lz->setType(Light::Point);
	lz->setColour(0.7, 0.1, 0.7);

	DebugDrawer* db = new DebugDrawer(RenderSystem::GetInstance()->getSceneManager());
	PhysicsSystem::GetInstance()->setDebugDrawer(db);

	obj = new GameObject("Churro", "Ch", nullptr);
	Transform* transform2 = new Transform(obj);
	obj->transform->setPosition(Vector3(25, 100, -400));
	obj->transform->setScale(Vector3(0.5, 0.5, 0.5));
	obj->transform->rotate(Vector3(0, 0, 0));
	MeshRenderer* ms = new MeshRenderer(obj);
	ms->createEntity("knot", "knot.mesh");
	
	SoundEmitter* emi = new SoundEmitter(obj);
	
	
	Reverb* rev = new Reverb(aux);
	rev->setReverbMaxDistance(100000);
	rev->setReverbPreset(rev->MOUNTAINS);
	
	emi->playSound("prueba2",true);
  
	RigidBody* rb = new RigidBody(obj);
	rb->setRigidBody(1.0, SPHERE_RB_SHAPE, {}, { 2,2,2 });
	rb->addImpulse({ 10.0f, 0.0f,0.0f });
	rb->addImpulse({ 0.0f, 500.0f,0.0f }, TORQUE);

	GameObject* obj1 = new GameObject("Cubo", "Cu", nullptr);
	Transform* transform3 = new Transform(obj1);
	obj1->transform->setPosition(Vector3(25, 0, -400));
	obj1->transform->setScale(Vector3(4, 0.05, 1));
	obj1->transform->rotate(Vector3(0, 0, 0));
	MeshRenderer* ms1 = new MeshRenderer(obj1);
	ms1->createEntity("cube", "cube.mesh");
	RigidBody* rb2 = new RigidBody(obj1);
	rb2->setRigidBody(0, BOX_RB_SHAPE);

}

void GaiaCore::run()
{
	while (true)
	{
		PhysicsSystem::GetInstance()->update();
		RenderSystem::GetInstance()->render();
		update();

		 SoundSystem::GetInstance()->updateSounds();
		InputSystem::GetInstance()->update();

	}
}

void GaiaCore::close()
{
	 SoundSystem::GetInstance()->destroy();
}

void GaiaCore::update()
{
	obj->transform->translate(Vector3(0, 0, 0));
	//obj->getComponent<SoundEmitter>()->update(2);

	SoundSystem::GetInstance()->update();


	aux->transform->rotate(Vector3(0, 0, 0));
/*	SoundListener* lis = aux->getComponent<SoundListener>();
	emi->update(0);*/

	if (InputSystem::GetInstance()->getKeyPress("D"))
		PhysicsSystem::GetInstance()->clearWorld();
}