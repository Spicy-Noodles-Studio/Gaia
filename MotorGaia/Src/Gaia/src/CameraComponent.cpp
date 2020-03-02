#include "CameraComponent.h"

CameraComponent::CameraComponent(GameObject* gameObject,std::string name) : GaiaComponent(gameObject)
{
	node = RenderSystem::GetInstance()->getSceneManager()->createSceneNode();
	cam = RenderSystem::GetInstance()->getSceneManager()->createCamera(name);
	cam->setAutoAspectRatio(true);
	node->attachObject(cam);
	
}

CameraComponent::~CameraComponent()
{
	
}



void CameraComponent::setPosition(Ogre::SceneNode* node, Ogre::Vector3 position)
{
	node->setPosition(position);
}


void CameraComponent::setRotation(Ogre::SceneNode* node, Axis axis, float degrees)
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

void CameraComponent::lookAt(Ogre::Vector3 pos, Ogre::Node::TransformSpace space)
{
	node->lookAt(pos, space);
}

void CameraComponent::setDirecction(Ogre::Vector3 dir)
{
	node->setDirection(dir);
}


void CameraComponent::setClipDistances(double near, double far)
{
	cam->setNearClipDistance(near);
	cam->setFarClipDistance(far);
	
}
