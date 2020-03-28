#include "CameraDebugControl.h"
#include "InputSystem.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "SceneManager.h"

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
	InputSystem* input = InputSystem::GetInstance();
	Vector3 camDir = camera->getDirection();
	Vector3 viewY = camDir.cross(Vector3(0, 1, 0)).normalized();
	Vector3 myPos = camera->gameObject->transform->getPosition();
	float vel = 5.f * deltaTime;
	auto currentPos = input->getMousePosition();

	//Teclado
	if (input->isKeyPressed("W"))
		myPos += camDir * vel;
	if (input->isKeyPressed("S"))
		myPos -= camDir * vel;
	if (input->isKeyPressed("A"))
		myPos -= viewY * vel;
	if (input->isKeyPressed("D"))
		myPos += viewY * vel;
	//Mouse
	if (input->getMouseButtonHold('l')) {
		int xDelta = mousePos.first - currentPos.first;
		int yDelta = mousePos.second - currentPos.second;
		camera->gameObject->transform->rotate(Vector3(yDelta, xDelta, 0.0));
	}
	mousePos = currentPos;

	camera->gameObject->transform->setPosition(myPos);

	// PARA PROBAR FISICAS
	if (input->getKeyPress("C")) {
		/*for (int x = -2; x < 3; x++) {
			for (int y = 1; y < 6; y++) {
				for (int z = -2; z < 3; z++) {
					GameObject* cubo = instantiate("Cubo");
					cubo->transform->setPosition(x, y, z);
					LOG("CUBO CREADO");
				}
			}
		}*/

		instantiate("Cubo", Vector3(0, 1, 0));
		instantiate("Cubo", Vector3(1, 1, 0));
	}
	if (input->getKeyPress("V")) {
		GameObject* gObject = instantiate("Bola", gameObject->transform->getPosition());
		gObject->getComponent<RigidBody>()->setLinearVelocity(camDir * 20);
	}


	if (input->getKeyPress("Escape")) {
		if(gameObject->getScene()->getName() == "MainScene")
			SceneManager::GetInstance()->changeScene("otherScene");
		else
			SceneManager::GetInstance()->changeScene("mainScene");
	}
}
