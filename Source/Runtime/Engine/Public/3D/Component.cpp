#include "Component.h"
#include "Precompiled.h"

using namespace CK::DDD;

Component::~Component()
{
}

void Component::SetGameObject(GameObject* gameObject)
{
	_GameObject = gameObject;
}
