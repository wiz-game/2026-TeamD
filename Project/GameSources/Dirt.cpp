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
		m_HP(20.0f),
		m_dirtCondition(DirtCondition::DirtMax)
	{
	}

	Dirt::~Dirt()
	{
		GameManager::Instance().SubDirt();
	}

	void Dirt::OnCreate()
	{
		AddTag(L"Dirt");
		AddTag(L"Wall");
		SetIsEditorSave(true);

		m_trans = AddComponent<Transform>();
		m_trans->SetScale(m_objectParam.GetScale());
		m_trans->SetQuaternion(m_objectParam.GetQuaternion());
		m_trans->SetPosition(m_objectParam.GetPosition());

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"AwaPaka_dorodoro");
		m_draw->SetTextureResource(L"T_AwaPaka_Gold_DoroDoro");
		m_draw->SetOwnShadowActive(true);

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(false);
		col->SetFixed(true);
		

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.1f, 0.1f, 0.1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,-0.5f, 0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
	}

	void Dirt::OnUpdate()
	{
		if (m_HP <= 10.0f && m_HP > 0.0f)
		{
			SetDirtState(DirtCondition::DirtHalf);
		}
		if (m_HP <= 0.0f)
		{
			SetDirtState(DirtCondition::DirtClean);
		}
	}

	void Dirt::OnUpdate2()
	{

	}

	void Dirt::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
	}

	void Dirt::SetDirtState(DirtCondition state)
	{
		if (m_dirtCondition == state) return;
		ExitDirtState(m_dirtCondition);
		m_dirtCondition = state;
		EnterDirtState(m_dirtCondition);
	}

	void Dirt::EnterDirtState(DirtCondition state)
	{
		switch (state)
		{
		case basecross::DirtCondition::DirtMax:
			m_draw->SetMeshResource(L"AwaPaka_dorodoro");
			m_draw->SetTextureResource(L"T_AwaPaka_Gold_DoroDoro");
			break;
		case basecross::DirtCondition::DirtHalf:
			m_draw->SetMeshResource(L"AwaPaka_doro");
			m_draw->SetTextureResource(L"T_AwaPaka_Gold_Doro");
			if (!m_effectPlyaerd)
			{
				EffectManager::Instance().PlayEffect(L"Clean", GetComponent<Transform>()->GetPosition());
				m_effectPlyaerd = true;
			}
			break;
		case basecross::DirtCondition::DirtClean:
			m_draw->SetMeshResource(L"AwaPaka_gold");
			m_draw->SetTextureResource(L"T_AwaPaka_Gold");
			if (!m_effectPlyaerd)
			{
				EffectHandle effHandle;
				effHandle = EffectManager::Instance().PlayEffect(L"Clean", GetComponent<Transform>()->GetPosition());
				m_effectPlyaerd = true;
			}
			break;
		default:
			break;
		}

	}

	void Dirt::ExitDirtState(DirtCondition state)
	{
		switch (state)
		{
		case basecross::DirtCondition::DirtMax:
			break;
		case basecross::DirtCondition::DirtHalf:
			m_effectPlyaerd = false;
			break;
		case basecross::DirtCondition::DirtClean:
			break;
		default:
			break;
		}

	}
}