#include "Camera.h"
#include "GameObject.h"

Camera::Camera(GameObject* gameObject) : GaiaComponent(gameObject)
{
	cam = RenderSystem::GetInstance()->getSceneManager()->createCamera(gameObject->getName() + "Cam");

	cam->setAutoAspectRatio(true);
	gameObject->node->attachObject(cam);
}

Camera::~Camera()
{
	
}

void Camera::lookAt(const Vector3& pos, SpaceReference space)
{
	switch (space)
	{
	case LocalSpace:
		gameObject->node->lookAt(Ogre::Vector3(pos.x, pos.y, pos.z), Ogre::Node::TS_LOCAL);
		break;
	case WorldSpace:
		gameObject->node->lookAt(Ogre::Vector3(pos.x, pos.y, pos.z), Ogre::Node::TS_WORLD);
		break;
	case ParentSpace:
		gameObject->node->lookAt(Ogre::Vector3(pos.x, pos.y, pos.z), Ogre::Node::TS_PARENT);
		break;
	default:
		break;
	}
}

void Camera::setDirection(const Vector3& dir)
{
	gameObject->node->setDirection(Ogre::Vector3(dir.x, dir.y, dir.z));
}

Ogre::Camera* Camera::getCamera()
{
	return cam;
}

void Camera::setClipDistances(double near, double far)
{
	cam->setNearClipDistance(near);
	cam->setFarClipDistance(far);
}