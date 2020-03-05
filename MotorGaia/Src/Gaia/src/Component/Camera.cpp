#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include <sstream>

Camera::Camera(GameObject* gameObject) : GaiaComponent(gameObject), isMainCamera(false)
{
	cam = gameObject->getScene()->getSceneManager()->createCamera(gameObject->getName() + "Cam");

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

void Camera::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		if (prop.first == "main") {
			if (prop.second == "true") {
				if (gameObject->getScene()->getMainCamera() == nullptr) {
					isMainCamera = true;
					gameObject->getScene()->setMainCamera(this);
				}
				else {
					printf("CAMERA: there's already a main Camera\n");
				}
			}
			else if (prop.second == "false") {
				isMainCamera = false;
			}
			else {
				printf("CAMERA: %s value not valid for \"main\" property\n", prop.second.c_str());
			}
		}
		else {
			printf("CAMERA: %s is not a valid property name\n", prop.first.c_str());
		}
	}
}
