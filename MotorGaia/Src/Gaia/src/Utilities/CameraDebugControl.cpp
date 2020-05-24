#include "CameraDebugControl.h"

#include "InputSystem.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "SceneManager.h"
#include "ComponentRegister.h"

#ifdef _DEBUG
REGISTER_FACTORY(CameraDebugControl);
#endif 

CameraDebugControl::CameraDebugControl(GameObject* gameObject) : UserComponent(gameObject), camera(nullptr)
{
}

CameraDebugControl::~CameraDebugControl()
{
}

void CameraDebugControl::start()
{
	camera = gameObject->getComponent<Camera>();
}

void CameraDebugControl::update(float deltaTime)
{
	checkNullAndBreak(camera);

	InputSystem* inputSystem = InputSystem::GetInstance();
	checkNullAndBreak(inputSystem);

	Vector3 camDir = camera->getDirection();
	Vector3 viewY = camDir.cross(Vector3(0, 1, 0)).normalized();
	Vector3 myPos = camera->gameObject->transform->getPosition();
	float vel = 5.f * deltaTime;
	auto currentPos = inputSystem->getMousePosition();

	//Teclado
	if (inputSystem->isKeyPressed("W"))
		myPos += camDir * vel;
	if (inputSystem->isKeyPressed("S"))
		myPos -= camDir * vel;
	if (inputSystem->isKeyPressed("A"))
		myPos -= viewY * vel;
	if (inputSystem->isKeyPressed("D"))
		myPos += viewY * vel;

	//Mouse
	if (inputSystem->getMouseButtonHold('l')) {
		int xDelta = mousePos.first - currentPos.first;
		int yDelta = mousePos.second - currentPos.second;
		camera->gameObject->transform->rotate(Vector3(yDelta, xDelta, 0.0));
	}
	mousePos = currentPos;

	camera->gameObject->transform->setPosition(myPos);

	// PARA PROBAR FISICAS
	if (inputSystem->getKeyPress("C")) {
		instantiate("Cubo", Vector3(0, 1, 0));
		instantiate("Cubo", Vector3(1, 1, 0));
	}
	if (inputSystem->getKeyPress("V")) {
		GameObject* gObject = instantiate("Bola", gameObject->transform->getPosition());
		gObject->getComponent<RigidBody>()->setLinearVelocity(camDir * 20);
	}
}
