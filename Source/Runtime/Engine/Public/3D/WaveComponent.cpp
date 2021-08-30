#include "WaveComponent.h"
#include "Precompiled.h"
#include <cmath>


using namespace CK::DDD;

WaveComponent::~WaveComponent()
{

}

void WaveComponent::SetThetaValue(float theta)
{
	_CurrentTheta = theta;
}

void WaveComponent::SetSpeed(float speed)
{
	_Speed = speed;
}

void WaveComponent::Init()
{
	_StartScale = _GameObject->GetTransform().GetScale();
}

void WaveComponent::Update()
{
	const float delta = sinf(_CurrentTheta) * 0.5f;
	const Vector3 deltaScale = _StartScale * delta;
	_GameObject->GetTransform().SetScale(deltaScale);

	_CurrentTheta += _Speed;
}

void WaveComponent::Destroy()
{

}
