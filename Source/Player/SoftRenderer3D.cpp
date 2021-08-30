
#include "Precompiled.h"
#include "SoftRenderer.h"
#include "3D/WaveComponent.h"
#include <random>
using namespace CK::DDD;

// ����� �׸��� �Լ�
void SoftRenderer::DrawGizmo3D()
{
	auto& r = GetRenderer();
	const GameEngine& g = Get3DGameEngine();

	// �� ����� �׸���
	std::vector<Vertex3D> viewGizmo = {
		Vertex3D(Vector4(Vector3::Zero)),
		Vertex3D(Vector4(Vector3::UnitX * _GizmoUnitLength)),
		Vertex3D(Vector4(Vector3::UnitY * _GizmoUnitLength)),
		Vertex3D(Vector4(Vector3::UnitZ * _GizmoUnitLength)),
	};

	Matrix4x4 viewMatRotationOnly = g.GetMainCamera().GetViewMatrixRotationOnly();
	VertexShader3D(viewGizmo, viewMatRotationOnly);

	// �� �׸���
	Vector2 v0 = viewGizmo[0].Position.ToVector2() + _GizmoPositionOffset;
	Vector2 v1 = viewGizmo[1].Position.ToVector2() + _GizmoPositionOffset;
	Vector2 v2 = viewGizmo[2].Position.ToVector2() + _GizmoPositionOffset;
	Vector2 v3 = viewGizmo[3].Position.ToVector2() + _GizmoPositionOffset;
	r.DrawLine(v0, v1, LinearColor::Red);
	r.DrawLine(v0, v2, LinearColor::Green);
	r.DrawLine(v0, v3, LinearColor::Blue);
}

// ���� ������Ʈ ���
static const std::string PlayerGo = "Player";
std::vector<GameObject*> BackObjects;

// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene3D()
{
	GameEngine& g = Get3DGameEngine();

	// �÷��̾�
	constexpr float playerScale = 100.f;

	// �÷��̾� ����
	GameObject& goPlayer = g.CreateNewGameObject(PlayerGo);
	goPlayer.SetMesh(GameEngine::CubeMesh);
	goPlayer.GetTransform().SetPosition(Vector3::Zero);
	goPlayer.GetTransform().SetScale(Vector3::One * playerScale);
	goPlayer.GetTransform().SetRotation(Rotator(0.f, 45.f, -60.f));
	goPlayer.SetColor(LinearColor::Blue);

	// ī�޶� ����
	CameraObject& mainCamera = g.GetMainCamera();
	mainCamera.GetTransform().SetPosition(Vector3(0.f, 0.f, 500.f));
	mainCamera.GetTransform().SetRotation(Rotator(180.f, 0.f, 0.f));

	BackObjects.reserve(30);

	const auto& startPosition = Vector3(-(10 * playerScale) / 2, -(10 * playerScale) / 2, 0);

	for (size_t x = 0; x < 10; ++x)
	{
		for (size_t y = 0; y < 10; ++y)
		{
			auto& gObject = g.CreateNewGameObject(std::to_string(x * 10 + y));
			gObject.SetMesh(GameEngine::CubeMesh);
			gObject.GetTransform().SetPosition(startPosition + Vector3(x * playerScale, y * playerScale, 0));
			gObject.GetTransform().SetScale(Vector3::One * playerScale);
			gObject.GetTransform().SetRotation(Rotator(0.f, 45.f, -60.f));
			gObject.SetColor(LinearColor::Blue);
			const auto wave = new WaveComponent();
			gObject.AddComponent(wave);
			wave->SetThetaValue((x + y) * 0.5f);
			BackObjects.push_back(&gObject);
		}
	}
}

// ���� ������ ������ ������ �����ϴ� ����

// ���� ������ ����ϴ� �Լ�
void SoftRenderer::Update3D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	GameEngine& g = Get3DGameEngine();
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float moveSpeed = 500.f;
	static float rotateSpeed = 180.f;

	// ���� �������� ����� ���� ������Ʈ ���۷���
	GameObject& goPlayer = g.GetGameObject(PlayerGo);
	CameraObject& camera = g.GetMainCamera();

	// �Է¿� ���� �÷��̾� Ʈ�������� ����
	goPlayer.GetTransform().AddPosition(Vector3::UnitZ * input.GetAxis(InputAxis::ZAxis) * moveSpeed * InDeltaSeconds);
	goPlayer.GetTransform().AddPitchRotation(-input.GetAxis(InputAxis::WAxis) * rotateSpeed * InDeltaSeconds);

	// �Է¿� ���� ī�޶� Ʈ�������� ����
	camera.GetTransform().AddYawRotation(-input.GetAxis(InputAxis::XAxis) * rotateSpeed * InDeltaSeconds);
	camera.GetTransform().AddPitchRotation(-input.GetAxis(InputAxis::YAxis) * rotateSpeed * InDeltaSeconds);

	for (const auto& gObject : BackObjects) {
		gObject->UpdateComponents();
	}

}

// �ִϸ��̼� ������ ����ϴ� �Լ�
void SoftRenderer::LateUpdate3D(float InDeltaSeconds)
{
	// �ִϸ��̼� �������� ����ϴ� ��� �� �ֿ� ���۷���
	GameEngine& g = Get3DGameEngine();

	// �ִϸ��̼� ������ ���� ����
}

// ������ ������ ����ϴ� �Լ�
void SoftRenderer::Render3D()
{
	// ������ �������� ����ϴ� ��� �� �ֿ� ���۷���
	const GameEngine& g = Get3DGameEngine();
	auto& r = GetRenderer();
	const CameraObject& mainCamera = g.GetMainCamera();

	// ��濡 ����� �׸���
	DrawGizmo3D();

	// ������ ������ ���� ����
	const Matrix4x4 vMatrix = mainCamera.GetViewMatrix();

	for (auto it = g.SceneBegin(); it != g.SceneEnd(); ++it)
	{
		const GameObject& gameObject = *(*it);
		if (!gameObject.HasMesh() || !gameObject.IsVisible())
		{
			continue;
		}

		// �������� �ʿ��� ���� ������Ʈ�� �ֿ� ���۷����� ���
		const Mesh& mesh = g.GetMesh(gameObject.GetMeshKey());
		const TransformComponent& transform = gameObject.GetTransform();

		Matrix4x4 finalMatrix = vMatrix * transform.GetModelingMatrix();

		// �޽� �׸���
		DrawMesh3D(mesh, finalMatrix, gameObject.GetColor());

		// �� ���������� �÷��̾� ��ġ�� ȭ�鿡 ǥ��
		if (gameObject == PlayerGo)
		{
			Vector3 viewPosition = vMatrix * transform.GetPosition();
			r.PushStatisticText("View : " + viewPosition.ToString());
		}
	}
}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh3D(const Mesh& InMesh, const Matrix4x4& InMatrix, const LinearColor& InColor)
{
	size_t vertexCount = InMesh.GetVertices().size();
	size_t indexCount = InMesh.GetIndices().size();
	size_t triangleCount = indexCount / 3;

	// �������� ����� ���� ���ۿ� �ε��� ���۷� ��ȯ
	std::vector<Vertex3D> vertices(vertexCount);
	std::vector<size_t> indice(InMesh.GetIndices());
	for (size_t vi = 0; vi < vertexCount; ++vi)
	{
		vertices[vi].Position = Vector4(InMesh.GetVertices()[vi]);

		if (InMesh.HasColor())
		{
			vertices[vi].Color = InMesh.GetColors()[vi];
		}

		if (InMesh.HasUV())
		{
			vertices[vi].UV = InMesh.GetUVs()[vi];
		}
	}

	// ���� ��ȯ ����
	VertexShader3D(vertices, InMatrix);

	// �ﰢ�� ���� �׸���
	for (int ti = 0; ti < triangleCount; ++ti)
	{
		int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
		std::vector<Vertex3D> tvs = { vertices[indice[bi0]] , vertices[indice[bi1]] , vertices[indice[bi2]] };

		size_t triangles = tvs.size() / 3;
		for (size_t ti = 0; ti < triangles; ++ti)
		{
			size_t si = ti * 3;
			std::vector<Vertex3D> sub(tvs.begin() + si, tvs.begin() + si + 3);
			DrawTriangle3D(sub, InColor, FillMode::Color);
		}
	}
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle3D(std::vector<Vertex3D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
	auto& r = GetRenderer();
	const GameEngine& g = Get3DGameEngine();

	LinearColor finalColor = _WireframeColor;
	if (InColor != LinearColor::Error)
	{
		finalColor = InColor;
	}

	r.DrawLine(InVertices[0].Position, InVertices[1].Position, finalColor);
	r.DrawLine(InVertices[0].Position, InVertices[2].Position, finalColor);
	r.DrawLine(InVertices[1].Position, InVertices[2].Position, finalColor);
}

