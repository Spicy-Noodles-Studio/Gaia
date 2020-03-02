#include "Camera.h"
#include "GameObject.h"
Camera::Camera(GameObject* gameObject) : GaiaComponent(gameObject)
{
	
	node = RenderSystem::GetInstance()->getSceneManager()->createSceneNode();
	cam = RenderSystem::GetInstance()->getSceneManager()->createCamera(gameObject->getName()+"Cam");
	cam->setAutoAspectRatio(true);
	node->attachObject(cam);
}

Camera::~Camera()
{
	
}



void Camera::setPosition(Ogre::SceneNode* node, Ogre::Vector3 position)
{
	node->setPosition(position);
}


void Camera::setRotation(Ogre::SceneNode* node, Axis axis, float degrees)
{
	switch (axis)
	{
	case X:
		node->pitch(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	case Y:
		node->yaw(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	case Z:
		node->roll(Ogre::Radian(Ogre::Degree(degrees)));
		break;
	default:
		break;
	}
}

void Camera::lookAt(Ogre::Vector3 pos, SpaceReference space)
{
	switch (space)
	{
	case LocalSpace:
		node->lookAt(pos, Ogre::Node::TS_LOCAL);
		break;
		node->lookAt(pos, Ogre::Node::TS_WORLD);
	case WorldSpace:
		break;
	case ParentSpace:
		node->lookAt(pos, Ogre::Node::TS_PARENT);
		break;
	default:
		break;
	}
}

void Camera::setDirecction(Ogre::Vector3 dir)
{
	node->setDirection(dir);
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
