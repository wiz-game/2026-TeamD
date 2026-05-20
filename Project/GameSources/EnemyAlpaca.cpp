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

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"M_Alpaca");
		drawComp->SetTextureResource(L"T_Alpaca");
		drawComp->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();

		//m_gravity = AddComponent<Gravity>();

		m_EnemyHP = 10;

		m_eStateMachine.reset(new StateMachine<EnemyAlpaca>(GetThis<EnemyAlpaca>()));
		m_eStateMachine->ChangeState(IdleState::Instance());
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		m_eStateMachine->Update();

		Died(GetThis<EnemyAlpaca>());
		DetectionRange(GetThis<EnemyAlpaca>());

		m_eStateMachine->Update();

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
		Obj->PointMove(Obj, 1.0f);

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
		Obj->Stalker(Obj, 1.0f);
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
