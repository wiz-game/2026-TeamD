#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TrampolineBubbles::TrampolineBubbles(const shared_ptr<Stage>& stage,const Vec3& pos) :
		GameObject(stage),
		m_isTrampolineActive(false),
		m_isInstanceCreated(false),
		m_bubbleCount(0),
		m_pos(pos),
		m_scale(Vec3(0.4))
	{

	}

	TrampolineBubbles::~TrampolineBubbles()
	{

	}

	void TrampolineBubbles::OnCreate()
	{
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetScale(m_scale);

		m_chargeDraw = AddComponent<PNTStaticDraw>();
		m_chargeDraw->SetMeshResource(L"M_Bubble");
		m_chargeDraw->SetTextureResource(L"T_Bubble");
		m_chargeDraw->SetDrawActive(true);

		// インスタンス描画用
		m_activeDraw = AddComponent<PNTStaticInstanceDraw>();
		m_activeDraw->SetMeshResource(L"M_Bubble");
		m_activeDraw->SetTextureResource(L"T_Bubble");
		m_activeDraw->SetDrawActive(false);

		m_col = AddComponent<CollisionSphere>();
		m_col->SetDrawActive(false);
		m_col->SetAfterCollision(AfterCollision::None);

		AddTag(L"TrampolineBubbles");
	}

	void TrampolineBubbles::OnUpdate()
	{
		if (m_bubbleCount >= 5 && !m_isTrampolineActive)
		{
			m_isTrampolineActive = true;
			m_chargeDraw->SetDrawActive(false);
		}

		if (m_isTrampolineActive && !m_isInstanceCreated)
		{
			CreateActiveInstances();
		}
	}

	void TrampolineBubbles::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Bubble"))
		{
			if (!m_isTrampolineActive)
			{
				m_bubbleCount++;
			}

			auto bubble = dynamic_pointer_cast<Bubble>(Other);
			if (bubble)
			{
				GetStage()->RemoveGameObject<Bubble>(bubble);
			}
		}

		if (Other->FindTag(L"Player"))
		{
			auto player = dynamic_pointer_cast<Player>(Other);

			if (m_isTrampolineActive && player->GetIsJump())
			{
				GetStage()->RemoveGameObject<TrampolineBubbles>(GetThis<TrampolineBubbles>());
			}
		}
	}

	void TrampolineBubbles::CreateActiveInstances()
	{
		m_activeDraw->ClearMatrixVec();

		const int gridCount = 2;
		const float spacing = 0.8f;
		const float startOffsetX = -((gridCount - 1) * spacing) * 0.5f;
		const float startOffsetZ = -((gridCount - 1) * spacing) * 0.5f;

		for (int z = 0; z < gridCount; z++)
		{
			for (int x = 0; x < gridCount; x++)
			{
				Mat4x4 mat;
				Vec3 instancePos = m_pos + Vec3(startOffsetX + (spacing * x), -0.3, startOffsetZ + (spacing * z));
				mat.affineTransformation(
					Vec3(m_scale),
					Vec3(0.0f),
					Quat(),
					instancePos
				);
				m_activeDraw->AddMatrix(mat);
			}
		}

		m_activeDraw->SetDrawActive(true);
		m_isInstanceCreated = true;
	}
}
