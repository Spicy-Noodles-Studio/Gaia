#include "SoundListener.h"
#include "GameObject.h"


SoundListener::SoundListener(GameObject* gameObject) : GaiaComponent(gameObject)
{

}

SoundListener::~SoundListener()
{
}

void SoundListener::update(float deltaTime)
{
    Vector3 pos, forward, up;
    pos = { 0,0,0 };
    forward = { 0,0,0 };
    up = { 0,0,0 };
    pos = gameObject->transform->getPosition();
    gameObject->transform->getRotation();

    SoundSystem::GetInstance()->setListenerAttributes(pos, forward, up);
}
