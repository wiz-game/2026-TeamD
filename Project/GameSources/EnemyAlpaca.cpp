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
		m_transform = AddComponent<Transform>();
		m_transform->SetPosition(m_objectParam.GetPosition());
		m_transform->SetScale(m_objectParam.GetScale());
		m_transform->SetQuaternion(m_objectParam.GetQuaternion());

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"M_Alpaca");
		drawComp->SetTextureResource(L"T_Alpaca");
		drawComp->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();

		m_EnemyHP = 10;

		m_eStateMachine.reset(new StateMachine<EnemyAlpaca>(GetThis<EnemyAlpaca>()));
		m_eStateMachine->ChangeState(IdleState::Instance());
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		m_eStateMachine->Update();
		//m_eStateMachine->ChangeState()
		Died(GetThis<EnemyAlpaca>());
	}

	// 索敵範囲
	void EnemyAlpaca::DetectionRange()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (stage == nullptr)
		{
			return;
		}

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player == nullptr)
		{
			return;
		}
		auto playerPos = player->GetComponent<Transform>()->GetPosition();

		auto transPos = m_transform->GetPosition();

		auto distancePos_X = playerPos.x - transPos.x;
		auto distancePos_Y = playerPos.y - transPos.y;
		auto distancePos_Z = playerPos.z - transPos.z;

		//float distance = sqrt((distancePos_X * distancePos_X) + (distancePos_Y * distancePos_Y) + (distancePos_Z + ))
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
	}

	void IdleState::Exit(const shared_ptr<EnemyAlpaca>& Obj)
	{

	}
}
//end basecross
