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
		m_scale(Vec3(1.0f)),
		m_modelScale(Vec3(0.5f)),
		m_isCountedThisFrame(false)
	{

	}

	TrampolineBubbles::~TrampolineBubbles()
	{

	}

	void TrampolineBubbles::OnCreate()
	{
		AddTag(L"TranmpolineBase");
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos.x, m_pos.y - 0.5f, m_pos.z);
		m_trans->SetScale(m_scale);

		// 透明化処理
		SetAlphaActive(true);

		m_chargeDraw = AddComponent<PNTStaticDraw>();
		m_chargeDraw->SetMeshResource(L"M_Bubble");
		m_chargeDraw->SetTextureResource(L"T_Bubble");
		m_chargeDraw->SetDrawActive(true);
		m_chargeDraw->SetDiffuse(Col4(1.0f,1.0f,1.0f, 0.3f));

		// インスタンス描画用
		m_activeDraw = AddComponent<PNTStaticInstanceDraw>();
		m_activeDraw->SetMeshResource(L"M_Bubble");
		m_activeDraw->SetTextureResource(L"T_Bubble");
		m_activeDraw->SetDrawActive(false);
		m_activeDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));

		m_col = AddComponent<CollisionSphere>();
		m_col->SetAfterCollision(AfterCollision::None);
		m_col->SetDrawActive(false);

		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(m_modelScale),
			Vec3(0.0f,  0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);
		
		m_chargeDraw->SetMeshToTransformMatrix(spanMat);
	}

	void TrampolineBubbles::OnUpdate()
	{
		if (m_bubbleCount >= 2)
		{
			m_chargeDraw->SetDrawActive(false);
			CreateActiveInstances();
		}

		if (m_bubbleCount >= 4 && !m_isTrampolineActive)
		{
			m_isTrampolineActive = true;
			AddTag(L"TrampolineBubbles");
		}

	}

	void TrampolineBubbles::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Bubble"))
		{
			if (!m_isTrampolineActive && !m_isCountedThisFrame)
			{
				m_bubbleCount++;
				m_isCountedThisFrame = true;
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

		int index = 0;

		for (int z = 0; z < gridCount; z++)
		{
			for (int x = 0; x < gridCount; x++)
			{
				if (index >= m_bubbleCount) break;

				Mat4x4 mat;
				Vec3 instancePos = m_pos + Vec3(startOffsetX + (spacing * x), -1.0f, startOffsetZ + (spacing * z));
				mat.affineTransformation(
					Vec3(m_modelScale),
					Vec3(0.0f),
					Quat(),
					instancePos
				);
				m_activeDraw->AddMatrix(mat);

				index++;
			}
		}

		m_activeDraw->SetDrawActive(true);
	}
}
