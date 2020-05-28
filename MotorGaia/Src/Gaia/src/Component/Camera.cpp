#include "Camera.h"

#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <sstream>

#include "GameObject.h"
#include "Scene.h"
#include "ComponentData.h"
#include "DebugUtils.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Camera);

Camera::Camera(GameObject* gameObject) : GaiaComponent(gameObject), camera(nullptr), isMainCamera(false)
{
	checkNullAndBreak(gameObject);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);
	checkNullAndBreak(gameObject->node);
	camera = sceneManager->createCamera(gameObject->node->getName() + "Cam");
	checkNullAndBreak(camera);
	camera->setAutoAspectRatio(true);
	gameObject->node->attachObject(camera);

	setClipDistances(0.01, 10000);
}

Camera::~Camera()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	checkNullAndBreak(camera);
	gameObject->node->detachObject(camera);
	Scene* scene = gameObject->getScene();
	checkNullAndBreak(scene);
	Ogre::SceneManager* sceneManager = scene->getSceneManager();
	checkNullAndBreak(sceneManager);
	sceneManager->destroyCamera(camera);
	camera = nullptr;
}

void Camera::lookAt(const Vector3& position, SpaceReference space)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	switch (space)
	{
	case SpaceReference::LocalSpace:
		gameObject->node->lookAt(Ogre::Vector3(position.x, position.y, position.z), Ogre::Node::TS_LOCAL);
		break;
	case SpaceReference::WorldSpace:
		gameObject->node->lookAt(Ogre::Vector3(position.x, position.y, position.z), Ogre::Node::TS_WORLD);
		break;
	case SpaceReference::ParentSpace:
		gameObject->node->lookAt(Ogre::Vector3(position.x, position.y, position.z), Ogre::Node::TS_PARENT);
		break;
	default:
		break;
	}
}

Vector3 Camera::getDirection() const
{
	checkNullAndBreak(gameObject, Vector3::ZERO);
	checkNullAndBreak(gameObject->node, Vector3::ZERO);
	auto direction = gameObject->node->getOrientation().zAxis() * -1;
	return Vector3(direction.x, direction.y, direction.z);
}

void Camera::setDirection(const Vector3& direction)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	gameObject->node->setDirection(Ogre::Vector3(direction.x, direction.y, direction.z));
}

Quaternion Camera::getOrientation() const
{
	checkNullAndBreak(gameObject, Quaternion());
	checkNullAndBreak(gameObject->node, Quaternion());
	auto orientation = gameObject->node->getOrientation();
	return Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);
}

void Camera::setOrientation(const Quaternion& orientation)
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->node);
	gameObject->node->setOrientation(orientation.w, orientation.x, orientation.y, orientation.z);
}

Ogre::Camera* Camera::getCamera() const
{
	checkNullAndBreak(camera, nullptr);
	return camera;
}

Vector3 Camera::worldToScreen(const Vector3& worldPoint)
{
	Ogre::Vector3 world;
	world.x = worldPoint.x;
	world.y = worldPoint.y;
	world.z = worldPoint.z;

	checkNullAndBreak(camera, Vector3::ZERO);
	Ogre::Vector3 screenPoint = camera->getProjectionMatrix() * camera->getViewMatrix() * world;

	Vector3 result;
	result.x = (screenPoint.x * 0.5 + 0.5);
	result.y = (-screenPoint.y * 0.5 + 0.5);
	result.z = screenPoint.z * 0.5 + 0.5;

	return result;
}

Vector3 Camera::worldToScreenPixel(const Vector3& worldPoint)
{
	Ogre::Vector3 world;
	world.x = worldPoint.x;
	world.y = worldPoint.y;
	world.z = worldPoint.z;

	checkNullAndBreak(camera, Vector3::ZERO);
	Ogre::Vector3 screenPoint = camera->getProjectionMatrix() * camera->getViewMatrix() * world;

	Ogre::Viewport* viewport = camera->getViewport();
	checkNullAndBreak(viewport, Vector3::ZERO);
	Vector3 result;
	result.x = (screenPoint.x * 0.5 + 0.5) * viewport->getActualWidth();
	result.y = (-screenPoint.y * 0.5 + 0.5) * viewport->getActualHeight();
	result.z = screenPoint.z * 0.5 + 0.5;

	return result;
}

void Camera::setClipDistances(double near, double far)
{
	checkNullAndBreak(camera);
	camera->setNearClipDistance(near);
	camera->setFarClipDistance(far);
}

void Camera::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties()) {
		if (prop.first == "main") {
			if (prop.second == "true") {
				checkNullAndBreak(gameObject);
				Scene* scene = gameObject->getScene();
				checkNullAndBreak(scene);
				if (scene->getMainCamera() == nullptr) {
					isMainCamera = true;
					scene->setMainCamera(this);
				}
				else {
					LOG_ERROR("CAMERA", "There's already a main Camera");
				}
			}
			else if (prop.second == "false") {
				isMainCamera = false;
			}
			else {
				LOG_ERROR("CAMERA", "%s value not valid for \"main\" property", prop.second.c_str());
			}
		}
		else {
			LOG_ERROR("CAMERA", "%s is not a valid property name", prop.first.c_str());
		}
	}
}
