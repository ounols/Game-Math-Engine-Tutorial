#pragma once
#include "Component.h"
#include "Precompiled.h"

namespace CK
{
	namespace DDD
	{

		class WaveComponent : public Component
		{
		public:
			WaveComponent() = default;
			~WaveComponent() override;

			void SetThetaValue(float theta);
			void SetSpeed(float speed);

			void Init() override;
			void Update() override;
			void Destroy() override;

		private:
			Vector3 _StartScale;
			float _CurrentTheta = 0;
			float _Speed = 0.01f;
		};

	}
}