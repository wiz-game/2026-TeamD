/*!
@file EnemyAlpaca.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include <cmath>

namespace basecross 
{

	// 初期化
	void EnemyAlpaca::OnCreate()
	{
		AddTag(L"Enemy");
		//AddTag(L"Fade");
		SetIsEditorSave(true);

		m_transform = AddComponent<Transform>();
		m_transform->SetPosition(m_objectParam.GetPosition());
		m_transform->SetScale(m_objectParam.GetScale());
		m_transform->SetQuaternion(m_objectParam.GetQuaternion());

		m_draw = AddComponent<PNTBoneModelDraw>();
		m_draw->SetMeshResource(L"DoroPaka");
		m_draw->SetTextureResource(L"T_DoroPaka_Body");
		m_draw->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();
		obb->AddExcludeCollisionTag(L"Enemy");

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"DoroPaka");
		shadowComp->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.1f, 0.1f, 0.1f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat);
		
		m_eStateMachine.reset(new StateMachine<EnemyBase>(GetThis<EnemyBase>()));
		m_eStateMachine->ChangeState(IdleState::Instance());

		auto loopFlag = true;
		m_draw->AddAnimation(L"Idle", 0, 65, loopFlag, 60.0f);
		m_draw->AddAnimation(L"Walk", 70, 80, loopFlag, 60.0f);
		m_draw->AddAnimation(L"GameOver", 190, 60, !loopFlag, 60.0f);

		m_draw->ChangeCurrentAnimation(L"Walk");
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		m_eStateMachine->Update();
		m_draw->UpdateAnimation(App::GetApp()->GetElapsedTime());
		DebugString();
		Stun();

		if (!GetContatofBubble())
		{
			if (m_draw->GetCurrentAnimation() != L"Walk")
			{
				m_draw->ChangeCurrentAnimation(L"Walk");
			}
			if (m_effectHandle)
			{
				EffectManager::Instance().StopEffect(m_effectHandle);
				m_isStanEffectDraw = false;
			}
		}
	}

	void EnemyAlpaca::DebugString()
	{
		GameManager::Instance().AddDebugStr(L"StunTimer", GetStunTime());
		//GameManager::AddDebugLog(L"", m_Num);
	}


	void EnemyAlpaca::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		// 共通の当たり判定・フラグ更新を走らせる
		OnCollisionExecute(Other);

		auto bubble = dynamic_pointer_cast<Bubble>(Other);
		if (bubble)
		{
			if (bubble->GetBubblePowerUp())
			{
				if (!m_isStanEffectDraw)
				{
					SoundManager::Instance().PlaySE(L"Stan_SE");
					m_draw->ChangeCurrentAnimation(L"GameOver");
					auto pos = GetComponent<Transform>()->GetPosition();
					m_effectHandle = EffectManager::Instance().PlayEffect(L"Stan", Vec3(pos.x, pos.y + 2.0f, pos.z));
					EffectManager::Instance().SetScale(m_effectHandle, Vec3(0.4f));
					m_isStanEffectDraw = true;
				}
				SetIsContactOfBubbleetStun(true);
			}
		}
	}

	void EnemyAlpaca::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{
		if (!Other->FindTag(L"Wall"))
		{
			return;
		}

		SetCanGoForward(true);
		SetCanGoLeft(true);
		SetCanGoRight(true);

		// 引数の情報を取得する
		auto transComp = GetThis<EnemyAlpaca>()->GetComponent<Transform>();
		auto transPos = transComp->GetPosition();

		float startReyPos_Y = 0.3f;
		Vec3 startReyPos = Vec3(transPos.x, transPos.y + startReyPos_Y, transPos.z);

		float startRange = 0.15f;

		// レイ（スタート）
		Vec3 startFSp = CalculateEndPointRayAngle(startReyPos, m_rotY, 0.0f, startRange);       // 正面
		Vec3 startRSp = CalculateEndPointRayAngle(startReyPos, m_rotY, 90.0f, startRange);   // 右
		Vec3 startLSp = CalculateEndPointRayAngle(startReyPos, m_rotY, -90.0f, startRange);  // 左

		// レイ（エンド）
		Vec3 endFSp = CalculateEndPointRayAngle(startReyPos, m_rotY, 0, m_rayRange);       // 正面
		Vec3 endRSp = CalculateEndPointRayAngle(startReyPos, m_rotY, 90.0f, m_rayRange);   // 右
		Vec3 endLSp = CalculateEndPointRayAngle(startReyPos, m_rotY, -90.0f, m_rayRange);  // 左

		// 全てのゲームオブジェクトを探す
		for (auto& obj : GetStage()->GetGameObjectVec())
		{
			if (!obj->FindTag(L"Wall"))
			{
				continue;
			}

			auto objDrawComp = obj->GetComponent<PNTStaticDraw>(false);

			Vec3 hitPos;
			// 正面
			if (IsWallHit(objDrawComp, startFSp, endFSp, transPos))
			{
				Vec3 hitPos;
				// 正面
				if (IsWallHit(objDrawComp, startFSp, endFSp, transPos))
				{
					SetCanGoForward(false);
				}

				// 左
				if (IsWallHit(objDrawComp, startLSp, endLSp, transPos))
				{
					SetCanGoForward(false);
				}

				// 右
				if (IsWallHit(objDrawComp, startRSp, endRSp, transPos))
				{
					SetCanGoRight(false);
				}
				SetCanGoForward(false);
			}

			// 左
			if (IsWallHit(objDrawComp, startLSp, endLSp, transPos))
			{
				SetCanGoLeft(false);
			}

			// 右
			if (IsWallHit(objDrawComp, startRSp, endRSp, transPos))
			{
				SetCanGoRight(false);
			}

		}
	}

	void EnemyAlpaca::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}
}
//end basecross
