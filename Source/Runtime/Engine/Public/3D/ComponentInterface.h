#pragma once

namespace CK
{
	namespace DDD
	{

		class ComponentInterface
		{
		public:
			ComponentInterface() = default;
			virtual ~ComponentInterface() = default;

			virtual void Init() = 0;
			virtual void Update() = 0;
			virtual void Destroy() = 0;
		};

	}
}