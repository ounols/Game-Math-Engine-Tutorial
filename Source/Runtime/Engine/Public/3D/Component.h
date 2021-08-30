#pragma once
#include "ComponentInterface.h"

namespace CK
{
	namespace DDD
	{
		class GameObject;

		class Component : public ComponentInterface
		{
		public:
			Component() = default;
			virtual ~Component();

			void SetGameObject(GameObject* gameObject);

		protected:
			GameObject* _GameObject = nullptr;

		};

	}
}