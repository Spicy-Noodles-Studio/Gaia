#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include <OgreSceneManager.h>
#include <sstream>
#include "DebugUtils.h"
#include <OgreViewport.h>

Camera::Camera(GameObject* gameObject) : GaiaComponent(gameObject), isMainCamera(false)
{
	camera = gameObject->getScene()->getSceneManager()->createCamera(gameObject->getName() + "Cam");

	camera->setAutoAspectRatio(true);
	gameObject->node->attachObject(camera);

	
}

Camera::~Camera()
{
	gameObject->getScene()->getSceneManager()->destroyCamera(camera);
	camera = nullptr;
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
	return camera;
}
//Vector2 worldToScreen(const Vector3& worldPoint, Camera* cam)
//{
//	// Pass point through camera projection matrices
//	Vector4 screenPoint = cam->getProjectionMatrix() *
//		cam->getViewMatrix() *
//		worldPoint;
//
//	// Convert to relative screen space
//	return Vector2(screenPoint.x * 0.5f / screenPoint.w + 0.5f,
//		screenPoint.y * 0.5f / screenPoint.w + 0.5f);
//}
Vector3 Camera::worldToScreen(const Vector3& worldPoint )
{
	Ogre::Vector3 world;
	
	world.x = worldPoint.x;
	world.y = worldPoint.y;
	world.z = worldPoint.z;
	Ogre::Vector3 screenPoint =   camera->getProjectionMatrix() * camera->getViewMatrix() * world;//*world;
	
	//camera->getViewport()

 	Vector3 x;
	x.x = (screenPoint.x*0.5 +0.5 );
	x.y = (-screenPoint.y * 0.5 + 0.5);
	x.z = screenPoint.z * 0.5 + 0.5;

	
	return x;
}

Vector3 Camera::worldToScreenPixel(const Vector3& worldPoint)
{
	Ogre::Vector3 world;

	world.x = worldPoint.x;
	world.y = worldPoint.y;
	world.z = worldPoint.z;
	Ogre::Vector3 screenPoint = camera->getProjectionMatrix() * camera->getViewMatrix() * world;//*world;

	//camera->getViewport()

	Vector3 x;
	x.x = (screenPoint.x * 0.5 + 0.5) * camera->getViewport()->getActualWidth();
	x.y = (-screenPoint.y * 0.5 + 0.5) * camera->getViewport()->getActualHeight();
	x.z = screenPoint.z * 0.5 + 0.5;


	return x;
}

void Camera::setClipDistances(double near, double far)
{
	camera->setNearClipDistance(near);
	camera->setFarClipDistance(far);
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
					LOG("CAMERA: there's already a main Camera\n");
				}
			}
			else if (prop.second == "false") {
				isMainCamera = false;
			}
			else {
				LOG("CAMERA: %s value not valid for \"main\" property\n", prop.second.c_str());
			}
		}
		else {
			LOG("CAMERA: %s is not a valid property name\n", prop.first.c_str());
		}
	}
}
