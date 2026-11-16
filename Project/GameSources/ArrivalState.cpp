/*!
@file ArrivalState.cpp
@brief 
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
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
	}

	void ArrivalState::Exit(const shared_ptr<EnemyBase>& Obj)
	{

	}
}
//end basecross
