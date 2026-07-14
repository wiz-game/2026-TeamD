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

		m_draw->ChangeCurrentAnimation(L"Walk");
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		m_eStateMachine->Update();
		m_draw->UpdateAnimation(App::GetApp()->GetElapsedTime());
	}

	void EnemyAlpaca::DebugString()
	{
		//GameManager::AddDebugLog(L"", m_Num);
	}

	// ステートマシンの処理
	shared_ptr<IdleState> IdleState::Instance()
	{
		static shared_ptr<IdleState> instance(new IdleState);
		return instance;
	}

	void IdleState::Enter(const shared_ptr<EnemyBase>& Obj)
	{
		
	}

	void IdleState::Execute(const shared_ptr<EnemyBase>& Obj)
	{
		Obj->MazeWandering(Obj);
		Obj->DetectionRange(Obj);

		//if (Obj->GetDetection() == true)
		{
		//	Obj->m_eStateMachine->ChangeState(AngryState::Instance());
		}
	}

	void IdleState::Exit(const shared_ptr<EnemyBase>& Obj)
	{

	}

	// ステートマシンの処理
	shared_ptr<AngryState> AngryState::Instance()
	{
		static shared_ptr<AngryState> instance(new AngryState);
		return instance;
	}

	void AngryState::Enter(const shared_ptr<EnemyBase>& Obj)
	{

	}

	void AngryState::Execute(const shared_ptr<EnemyBase>& Obj)
	{
		Obj->Tracking(Obj,3.0f);
		Obj->DetectionRange(Obj);

		if(!Obj->GetDetection())
		{
			Obj->m_eStateMachine->ChangeState(ArrivalState::Instance());
		}
	}

	void AngryState::Exit(const shared_ptr<EnemyBase>& Obj)
	{

	}

	// ステートマシンの処理
	shared_ptr<ArrivalState> ArrivalState::Instance()
	{
		static shared_ptr<ArrivalState> instance(new ArrivalState);
		return instance;
	}

	void ArrivalState::Enter(const shared_ptr<EnemyBase>& Obj)
	{

	}

	void ArrivalState::Execute(const shared_ptr<EnemyBase>& Obj)
	{
		Obj->EstimatedPlayerLocation(Obj, 3.0f);
	}

	void ArrivalState::Exit(const shared_ptr<EnemyBase>& Obj)
	{

	}

	void EnemyAlpaca::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		// 引数の情報を取得する
		auto transComp = GetThis<EnemyAlpaca>()->GetComponent<Transform>();
		auto transPos = transComp->GetPosition();

		float startReyPos_Y = 0.3f;
		Vec3 startReyPos = Vec3(transPos.x, transPos.y + startReyPos_Y, transPos.z);

		// レイ
		Vec3 endFSp = CalculateEndPointRayAngle(transPos, m_rotY, 0);       // 正面
		Vec3 endRSp = CalculateEndPointRayAngle(transPos, m_rotY, 90.0f);   // 右
		Vec3 endLSp = CalculateEndPointRayAngle(transPos, m_rotY, -90.0f);  // 左

		float minT = 1.0f;

		// 壁に触れているので、正面は行けなくなった
		if (Other->FindTag(L"Wall"))
		{
			m_canGoForward = false;
		}

		// 全てのゲームオブジェクトを探す
		for (auto& obj : GetStage()->GetGameObjectVec())
		{
			// 自身が引数のgameObjectと等しければ次にいく
			if (!obj->FindTag(L"Wall"))
			{
				continue;
			}

			auto objDrawComp = obj->GetComponent<PNTStaticDraw>(false);
			if (objDrawComp)
			{
				if (!m_isRotated && !m_canGoForward)
				{
					// 左
					if (IsWallHit(objDrawComp, startReyPos, endLSp, transPos))
					{
						m_canGoLeft = false;
					}

					// 右
					if (IsWallHit(objDrawComp, startReyPos, endRSp, transPos))
					{
						m_canGoRight = false;
					}
				}
			}
		}
	}

	void EnemyAlpaca::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{

	}

	void EnemyAlpaca::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}

}
//end basecross
