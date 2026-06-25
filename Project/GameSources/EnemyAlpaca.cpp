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
		m_transform->SetRotation(0.0f, 0.0f, 0.0f);

		m_draw = AddComponent<PNTBoneModelDraw>();
		m_draw->SetMeshResource(L"DoroPaka");
		m_draw->SetTextureResource(L"T_DoroPaka_Body");
		m_draw->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();
		obb->AddExcludeCollisionTag(L"Enemy");

		//m_gravity = AddComponent<Gravity>();

		m_EnemyHP = 10;

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"DoroPaka");
		shadowComp->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat);
		
		m_eStateMachine.reset(new StateMachine<EnemyAlpaca>(GetThis<EnemyAlpaca>()));
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

		Died(GetThis<EnemyAlpaca>());
		//m_eStateMachine->Update();

		//DropDirt(GetThis<EnemyAlpaca>());
		DebugString();
	}

	// ステートマシンの処理
	shared_ptr<IdleState> IdleState::Instance()
	{
		static shared_ptr<IdleState> instance(new IdleState);
		return instance;
	}

	void IdleState::Enter(const shared_ptr<EnemyAlpaca>& Obj)
	{
		
	}

	void IdleState::Execute(const shared_ptr<EnemyAlpaca>& Obj)
	{
		Obj->MazeWandering(Obj);
		Obj->DetectionRange(Obj);

		if (Obj->GetDetection() == true)
		{
			Obj->m_eStateMachine->ChangeState(AngryState::Instance());
		}
	}

	void IdleState::Exit(const shared_ptr<EnemyAlpaca>& Obj)
	{

	}

	// ステートマシンの処理
	shared_ptr<AngryState> AngryState::Instance()
	{
		static shared_ptr<AngryState> instance(new AngryState);
		return instance;
	}

	void AngryState::Enter(const shared_ptr<EnemyAlpaca>& Obj)
	{

	}

	void AngryState::Execute(const shared_ptr<EnemyAlpaca>& Obj)
	{
		Obj->Tracking(Obj,3.0f);
		// Playerの情報を取得する
		auto player = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetSharedGameObject<Player>(L"Player");
		if (!player)
		{
			return;
		}
		auto playerComp = player->GetComponent<Transform>();
		auto playerPos = playerComp->GetPosition();
		// 自身の位置を取得する
		auto myComp = Obj->GetComponent<Transform>();
		auto myPos = myComp->GetPosition();
		// プレイヤーと自身の距離を計算する
		float distancePosX = playerPos.x - myPos.x;
		float distancePosY = playerPos.y - myPos.y;
		float distancePosZ = playerPos.z - myPos.z;
		float distance = sqrt((distancePosX * distancePosX) + (distancePosY * distancePosY) + (distancePosZ * distancePosZ));

		float distanceRange = 7.0f;

		// 索敵外から出たら徘徊に戻る
		//if (distance >= distanceRange)
		if(!Obj->GetDetection())
		{
			Obj->m_eStateMachine->ChangeState(IdleState::Instance());
		}
	}

	void AngryState::Exit(const shared_ptr<EnemyAlpaca>& Obj)
	{

	}
}
//end basecross
