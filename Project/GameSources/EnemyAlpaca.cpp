/*!
@file EnemyAlpaca.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

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

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DoroPaka");
		drawComp->SetTextureResource(L"T_DoroPaka_Body");
		drawComp->SetDrawActive(true);

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

		drawComp->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat);
		
		m_eStateMachine.reset(new StateMachine<EnemyAlpaca>(GetThis<EnemyAlpaca>()));
		m_eStateMachine->ChangeState(IdleState::Instance());
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		m_eStateMachine->Update();

		Died(GetThis<EnemyAlpaca>());
		DetectionRange(GetThis<EnemyAlpaca>());
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
		Obj->Stalker(Obj,3.0f);
		// 索敵外から出たら徘徊に戻る
		if (Obj->GetDetection() == false)
		{
			Obj->m_eStateMachine->ChangeState(IdleState::Instance());
		}
	}

	void AngryState::Exit(const shared_ptr<EnemyAlpaca>& Obj)
	{

	}
}
//end basecross
