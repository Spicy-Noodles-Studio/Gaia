#include "SoundListener.h"
#include "GameObject.h"
#include "Quaternion.h"
#include <iostream>

SoundListener::SoundListener(GameObject* gameObject) : GaiaComponent(gameObject)
{
    SoundSystem::GetInstance()->initListener(this);
}

SoundListener::~SoundListener()
{
}

void SoundListener::update(float deltaTime)
{
    Vector3 pos, forward, up;
    //std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
    pos = gameObject->transform->getPosition();
    forward = gameObject->transform->getForwardVector();
    up = gameObject->transform->getUpVector();

    SoundSystem::GetInstance()->setListenerAttributes(pos, forward, up);
    // std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
}

