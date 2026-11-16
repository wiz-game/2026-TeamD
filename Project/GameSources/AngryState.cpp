/*!
@file Foo.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
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
		Obj->Tracking(Obj);
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
		auto distancePos = playerPos - myPos;
		distancePos.y = 0.0f;

		float distanceRange = 7.0f;
		// 索敵外から出たら徘徊に戻る
		if (distancePos.length() >= distanceRange)
		{
			Obj->SetDetection(false);
		}

		if (Obj->GetDetection() == false)
		{
			Obj->m_eStateMachine->ChangeState(IdleState::Instance());
		}
	}

	void AngryState::Exit(const shared_ptr<EnemyBase>& Obj)
	{
	}
}
//end basecross
