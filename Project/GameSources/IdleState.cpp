/*!
@file IdleState.cpp
@brief 索敵ステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
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

		if (Obj->GetDetection() == true)
		{
			Obj->m_eStateMachine->ChangeState(AngryState::Instance());
		}
	}

	void IdleState::Exit(const shared_ptr<EnemyBase>& Obj)
	{

	}
}