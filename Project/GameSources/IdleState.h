/*!
@file Foo.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public GameObject;
	//--------------------------------------------------------------------------------------
	class IdleState : public ObjState<EnemyBase>
	{
		IdleState() {};
	public:
		static shared_ptr<IdleState> Instance();
		virtual void Enter(const shared_ptr<EnemyBase>& obj) override;
		virtual void Execute(const shared_ptr<EnemyBase>& obj) override;
		virtual void Exit(const shared_ptr<EnemyBase>& obj) override;
	};
}
//end basecross
