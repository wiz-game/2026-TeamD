/*!
@file ArrivalState.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class ArrivalState : public ObjState<EnemyBase>
	//--------------------------------------------------------------------------------------
	class ArrivalState : public ObjState<EnemyBase>
	{
		ArrivalState() {};
	public:
		static shared_ptr<ArrivalState> Instance();
		virtual void Enter(const shared_ptr<EnemyBase>& obj) override;
		virtual void Execute(const shared_ptr<EnemyBase>& obj) override;
		virtual void Exit(const shared_ptr<EnemyBase>& obj) override;
	};
}
//end basecross
