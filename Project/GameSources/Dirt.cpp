#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Dirt::Dirt(
		const shared_ptr<Stage>& StagePtr,
		const int& Id,
		const Vec3& Scale,
		const Quat& Quatition,
		const Vec3& Position
	):
		GameObject(StagePtr, Id),
		m_nowDeleteCount(0),
		m_alphaColor(1.0f)
	{
		m_scale = Scale;
		m_quatition = Quatition;
		m_position = Position;
	}

	Dirt::~Dirt()
	{

	}

	void Dirt::OnCreate()
	{
		AddTag(L"Dirt");

		m_trans = AddComponent<Transform>();
		m_trans->SetPosition(m_position);
		m_trans->SetScale(m_scale);
		m_trans->SetQuaternion(m_quatition);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(true);
		col->SetAfterCollision(AfterCollision::None);

		SetIsEditorSave(true);
	}

	void Dirt::OnUpdate()
	{
	}

	void Dirt::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Bubble"))
		{
			GetStage()->RemoveGameObject<Dirt>(GetThis<Dirt>());
		}
	}
}