#include "SoundListener.h"

#include "SoundSystem.h"
#include "GameObject.h"
#include "Vector3.h"
#include "Quaternion.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(SoundListener);

SoundListener::SoundListener(GameObject* gameObject) : GaiaComponent(gameObject)
{
    checkNullAndBreak(gameObject);
    checkNullAndBreak(gameObject->transform);
    SoundSystem* soundSystem = SoundSystem::GetInstance();
    checkNullAndBreak(soundSystem);
    soundSystem->createListener(&gameObject->transform->getPosition(), &gameObject->transform->getQuaternion());
}

SoundListener::~SoundListener()
{

}
