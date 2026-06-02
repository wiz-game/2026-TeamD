#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bubble::Bubble(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& parent,const Vec3& scale, const float& initialVelocity, const float& HP, const bool& isPowerUp) :
		GameObject(stage),
		m_parent(parent),
		m_speedRatio(0.0f),
		m_initialVelocity(initialVelocity),
		m_currentVelocity(m_initialVelocity),
		m_upwardVelocity(0.0f),
		m_isTimeStart(false),
		m_limitTime(0.75f),
		m_isSpawnedTrampoline(false),
		m_isHit(false),
		m_scale(scale),
		m_pos(Vec3(0.0f)),
		m_moveTime(0.0f),
		m_moveTimeLimit(3.0f),
		m_isShoot(false),
		m_isBubbleMove(false),
		m_HP(HP),
		m_isRideBubble(false),
		m_isPowerUp(isPowerUp)
	{

	}

	Bubble::~Bubble()
	{

	}

	unordered_map<BubbleAbility, bool> Bubble::m_unlockedAvilities;
	
	void Bubble::OnCreate()
	{		
		AddTag(L"Bubble");

		// 親の情報の取得
		auto parentLock = m_parent.lock();
		m_parentForward = parentLock->GetComponent<Transform>()->GetForward();
		m_parentForward.normalize();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();
		
		m_trans = GetComponent<Transform>();

		float randPosX = static_cast<float>((rand() % 100) - 50) * 0.01f;
		float randPosZ = static_cast<float>((rand() % 100) - 50) * 0.01f;

		m_pos = Vec3(parentPos.x + randPosX, parentPos.y + 1.0f, parentPos.z + randPosZ) + m_parentForward * 1.25f;

		m_trans->SetPosition(m_pos);
		m_trans->SetScale(Vec3(m_scale));
		// m_trans->SetQuaternion()

		m_col = AddComponent<CollisionSphere>();
		// m_col->SetDrawActive(true);
		m_col->SetAfterCollision(AfterCollision::None);

		// 透明化処理
		SetAlphaActive(true);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"M_Bubble");
		m_draw->SetTextureResource(L"T_Bubble");
		m_draw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));
        
		// インスタンス描画用
		m_activeDraw = AddComponent<PNTStaticInstanceDraw>();
		m_activeDraw->SetMeshResource(L"M_Bubble");
		m_activeDraw->SetTextureResource(L"T_Bubble");
		m_activeDraw->SetDrawActive(false);
		m_activeDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.45f,  0.45f,   0.45f),
			Vec3(0.0f,  0.0f,   0.0f),
			Vec3(0.0f, XM_PI,   0.0f),
			Vec3(0.0f, -0.4f,   0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
	}

	void Bubble::OnUpdate()
	{
		BubbleMove();

		if(m_HP <= 0.0f)
		{
			GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
		}

		if (m_isRideBubble)
		{
			CreateActiveInstances();
		}
	}
	
	void Bubble::ShootBubble()
	{
		m_forward = GetCameraForward();
		m_isShoot = true;
		m_isBubbleMove = false;
	}

	void Bubble::BubbleMove()
	{
		auto& app = App::GetApp();
		auto elapsed = app->GetElapsedTime();
		auto pos = m_trans->GetPosition();
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto camera = stage->GetView()->GetTargetCamera();
		auto myCamera = dynamic_pointer_cast<MyCamera>(camera);
		bool isAiming = myCamera->GetIsAiming();
		float decelerationStartRatio = 0.2f;

		// 現在速度が0になるまで
		if (m_moveTime < m_moveTimeLimit)
		{
			m_moveTime += elapsed;

			// 0.0f -> 1.0f
			float t = m_moveTime / m_moveTimeLimit;
			t = clamp(t, 0.0f, 1.0f);

			// 1.0f -> 0.0f
			m_speedRatio = 1.0f - t;

			// 現在速度
			m_currentVelocity = m_initialVelocity * m_speedRatio;

			if(m_isShoot && !m_isBubbleMove)
			{
				m_moveDir = isAiming ? m_forward : m_parentForward;
				m_isBubbleMove = true;
			}

			if (m_isShoot)
			{
				m_pos += m_moveDir * (m_currentVelocity * elapsed);
			}
		}

		// 現在の速度の割合が始めるぐらいの割合になったら上昇を始める
		if (m_speedRatio < decelerationStartRatio)
		{
			m_isTimeStart = true;

			// 0.5 -> 0.0fに向かうほど t: 0 -> 1
			auto t = (decelerationStartRatio - m_speedRatio) / decelerationStartRatio;
			t = clamp(t,0.0f, 1.0f);

			// 1から2までの補間係数
			m_upwardVelocity = 1.0f + (1.25f - 1.0f) * t;

			m_pos.y += m_upwardVelocity * elapsed;
		}

		// 時間制限の開始
		if (m_isTimeStart)
		{
			m_limitTime -= 1.0f * elapsed;
		}

		if (m_limitTime < 0.0f)
		{
			GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
			return;
		}

		m_trans->SetPosition(m_pos);
	}

	void Bubble::ApplyAblity(BubbleAbility ability)
	{
		if (!CanUseAbility(ability)) return;

		switch (ability)
		{
		case BubbleAbility::RideBubble:
			RemoveComponent<CollisionSphere>();
			AddComponent<CollisionObb>();
			m_draw->SetDrawActive(false);
			//CreateActiveInstances();
			m_trans->SetScale(Vec3(m_scale * 3.5f));
			m_col->SetAfterCollision(AfterCollision::Auto);
			m_isRideBubble = true;
			break;

		case BubbleAbility::TranpolineBubble:
			// トランポリン挙動
			m_isTranpolineBubble = true;
			break;

		default:
			break;
		}
	}

	Vec3 Bubble::GetCameraForward()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto camera = stage->GetView()->GetTargetCamera();
		auto myCamera = dynamic_pointer_cast<MyCamera>(camera);

		Vec3 at = myCamera->GetAt();		
		Vec3 eye = myCamera->GetEye();
		auto forward = at - eye;
		forward.normalize();
	
		return forward;
	}

	void Bubble::ResolveCounteract(Bubble& bubble, Dirt& dirt)
	{
		float bubbleHP = bubble.GetBubbleHP();
		float dirtHP = dirt.GetDirtHP();

		bubble.SetBubbleHP(max(0.0f, bubbleHP - dirtHP));
		dirt.SetDirtHP(max(0.0f, dirtHP - bubbleHP));
	}

	void Bubble::BubbleAddAblity(BubbleAbility ability)
	{		
		if (m_unlockedAvilities[ability])return;
		UnlockAbility(ability);
	}

	void Bubble::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Dirt"))
		{
			auto dirt = dynamic_pointer_cast<Dirt>(Other);
			ResolveCounteract(*this, *dirt);
		}

		if (Other->FindTag(L"Enemy"))
		{
			auto enemy = dynamic_pointer_cast<EnemyAlpaca>(Other);
			auto decreasehp = enemy->GetEnemyAlpacaHP();
			GameManager::Instance().AddDebugStr(L"EnemyHP",decreasehp);
			m_HP -= decreasehp;
			m_HP = max(0.0f, m_HP);
		}

		if (!Other->FindTag(L"Ground")) return;

		if (m_isTranpolineBubble)
		{
			if (m_isSpawnedTrampoline) return;
			if (m_isHit) return;
			m_isHit = true;

			auto range = 1.5f;
			auto stage = GetStage();
			auto ground = dynamic_pointer_cast<Ground>(Other);
			auto myPos = m_trans->GetPosition();
			auto objvec = stage->GetGameObjectVec();

			bool found = false;

			// 地面に当たった瞬間近くのトランポリン泡を探す
			for (auto& obj : objvec)
			{
				if (!obj->FindTag(L"TrampolineBase")) continue;
				auto tramp = dynamic_pointer_cast<TrampolineBubbles>(obj);
				if (tramp == nullptr) continue;
				auto trampTrans = tramp->GetComponent<Transform>();
				if (trampTrans == nullptr) continue;
				
				float dist = (myPos - trampTrans->GetPosition()).length();

				if (dist < range)
				{
					found = true;
					tramp->AddbubbleCount();
					break;
				}
			}
			if (!found && ground)
			{
				m_isSpawnedTrampoline = true;
				auto bubblePos = m_trans->GetPosition();
				GetStage()->AddGameObject<TrampolineBubbles>(Vec3(bubblePos));
			}
		}
		else
		{
			GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
		}
	}

	void Bubble::CreateActiveInstances()
	{
		m_activeDraw->ClearMatrixVec();

		const int clusterCount = 3;
		const float clusterSpacing = 3.0f;

		const Vec3 offsets[] =
		{
			Vec3(0.0f,  0.1f,  0.0f),   // 中心
			Vec3(1.0f,  0.1f,  0.5f),   // 右
			Vec3(-1.0f, 0.1f, - 0.75f),  // 左
			Vec3(-1.0f,  0.1f,  0.75f),  // 前
			Vec3(1.0f,  0.1f,  -0.5f),  // 後
		};

		const int bubbleCount = sizeof(offsets) / sizeof(offsets[0]);

		for (int i = 0; i < bubbleCount; i++)
		{
			Mat4x4 mat;

			Vec3 instancePos = m_pos + offsets[i];

			mat.affineTransformation(
				Vec3(0.55f, 0.55f, 0.55f),
				Vec3(0.0f),
				Quat(),
				instancePos
			);

			m_activeDraw->AddMatrix(mat);
		}

		m_activeDraw->SetDrawActive(true);
	}

	ViewBubble::ViewBubble(const shared_ptr<Stage>& stage, const vector<Vec3> *vertices) :
		GameObject(stage),
		m_vertices(vertices)
	{

	}

	ViewBubble::~ViewBubble()
	{
	}

	void ViewBubble::OnCreate()
	{
		m_trans = GetComponent<Transform>();
		m_trans->SetScale(Vec3(0.1f));

		m_draw = AddComponent<PNTStaticInstanceDraw>();
		m_draw->SetMeshResource(L"M_Bubble");
		m_draw->SetTextureResource(L"T_Bubble");
	}

	void ViewBubble::OnUpdate()
	{		
		CreateInstance();
	}

	void ViewBubble::CreateInstance()
	{		
		m_draw->ClearMatrixVec();

		for (const auto& pos : *m_vertices)
		{
			Mat4x4 mat;
			mat.affineTransformation(
				Vec3(0.1f),
				Vec3(0.0f),
				Quat(0.0f),
				pos
			);
			m_draw->AddMatrix(mat);
		}
	}
}
