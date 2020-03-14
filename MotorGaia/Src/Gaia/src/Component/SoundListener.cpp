#include "SoundListener.h"
#include "GameObject.h"
#include "Quaternion.h"
#include <iostream>

SoundListener::SoundListener(GameObject* gameObject) : GaiaComponent(gameObject)
{
   SoundSystem::GetInstance()->createListener(&gameObject->transform->getPosition(), &gameObject->transform->getQuaternion());
}

SoundListener::~SoundListener()
{
}



