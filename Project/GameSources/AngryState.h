/*!
@file AngryState.h
@brief 追跡ステート
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class AngryState : public ObjState<EnemyBase>
	//--------------------------------------------------------------------------------------
	class AngryState : public ObjState<EnemyBase>
	{
		AngryState() {};
	public:
		static shared_ptr<AngryState> Instance();
		virtual void Enter(const shared_ptr<EnemyBase>& obj) override;
		virtual void Execute(const shared_ptr<EnemyBase>& obj) override;
		virtual void Exit(const shared_ptr<EnemyBase>& obj) override;
	};
}
//end basecross
