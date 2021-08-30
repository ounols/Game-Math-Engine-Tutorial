#pragma once

namespace CK
{
namespace DDD
{

class GameObject
{
public:
	GameObject(const std::string& InName) : _Name(InName)
	{
		_Hash = std::hash<std::string>()(_Name);
	}

	~GameObject() {
		for (const auto& comp : _Components) {
			if (comp != nullptr) {
				comp->Destroy();
				delete comp;
			}
		}
		_Components.clear();
	}

public:
	// 종류
	void SetGameObjectType(const GameObjectType& _InGameObjectType) { _GameObjectType = _InGameObjectType; }
	FORCEINLINE bool IsGizmoObject() const { return _GameObjectType == GameObjectType::Gizmo; }

	// 트랜스폼
	TransformComponent& GetTransform() { return _Transform; }
	const TransformComponent& GetTransform() const { return _Transform; }

	// 메시
	void SetMesh(const std::size_t& InMeshKey) { _MeshKey = InMeshKey; }
	bool HasMesh() const { return _MeshKey != Math::InvalidHash; }
	FORCEINLINE std::size_t GetMeshKey() const { return _MeshKey; }

	// 색상
	void SetColor(const LinearColor& InColor) { _Color = InColor; }
	FORCEINLINE const LinearColor& GetColor() const { return _Color; }

	// 키 관련
	const std::string& GetName() const { return _Name; }
	std::size_t GetHash() const { return _Hash; }

	// 검색 관련
	bool IsValid() const { return _Hash != Math::InvalidHash; }
	FORCEINLINE bool operator==(const GameObject& InGameObject) const;
	FORCEINLINE bool operator!=(const GameObject& InGameObject) const;
	static GameObject Invalid;

	// 렌더링 설정
	bool IsVisible() const { return _IsVisible; }
	void SetVisible(bool InVisible) { _IsVisible = InVisible; }

	// 컴포넌트 관련
	void AddComponent(Component* component) { 
		_Components.push_back(component); 
		component->SetGameObject(this); 
		component->Init(); 
	}

	void UpdateComponents() {
		for (const auto& comp : _Components) {
			if (comp != nullptr) comp->Update();
		}
	}

private:
	GameObjectType _GameObjectType = GameObjectType::Normal;
	bool _IsVisible = true;
	std::size_t _Hash = Math::InvalidHash;
	std::string _Name;
	std::size_t _MeshKey = Math::InvalidHash;
	TransformComponent _Transform;
	LinearColor _Color = LinearColor::Error;
	std::list<ComponentInterface*> _Components;
};

FORCEINLINE bool GameObject::operator==(const GameObject& InGameObject) const
{
	return _Hash == InGameObject.GetHash();
}

FORCEINLINE bool GameObject::operator!=(const GameObject& InGameObject) const
{
	return _Hash == InGameObject.GetHash();
}


}
}