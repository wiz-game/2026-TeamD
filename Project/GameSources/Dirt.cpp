#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Dirt::Dirt(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	):
		GameObject(StagePtr, objectParam),
		m_nowDeleteCount(0),
		m_alphaColor(1.0f),
		m_HP(2.0f),
		m_isDead(false)
	{
	}

	Dirt::~Dirt()
	{
	}

	void Dirt::OnCreate()
	{
		AddTag(L"Dirt");
		SetIsEditorSave(true);

		m_trans = AddComponent<Transform>();
		m_trans->SetScale(m_objectParam.GetScale());
		m_trans->SetQuaternion(m_objectParam.GetQuaternion());
		m_trans->SetPosition(m_objectParam.GetPosition());

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(true);
		col->SetAfterCollision(AfterCollision::None);
	}

	void Dirt::OnUpdate()
	{
		if(m_HP <= 0.0f)
		{
			GetStage()->RemoveGameObject<Dirt>(GetThis<Dirt>());
		}
	}

	void Dirt::OnUpdate2()
	{

	}

	void Dirt::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
	}
}