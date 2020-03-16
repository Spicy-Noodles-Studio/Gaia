#include "SoundListener.h"

#include <iostream>

#include "GameObject.h"
#include "Quaternion.h"
#include "SoundSystem.h"

SoundListener::SoundListener(GameObject* gameObject) : GaiaComponent(gameObject)
{
   SoundSystem::GetInstance()->createListener(&gameObject->transform->getPosition(), &gameObject->transform->getQuaternion());
}

SoundListener::~SoundListener()
{
}



