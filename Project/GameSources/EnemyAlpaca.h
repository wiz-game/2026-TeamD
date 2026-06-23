/*!
@file EnemyAlpaca.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross 
{


	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public EnemyBase;
	//--------------------------------------------------------------------------------------
	class EnemyAlpaca : public EnemyBase
	{
	private:
		//Vec3 m_Position;
		//Vec3 m_Scale;
		//Vec3 m_Rotation;

		std::shared_ptr<Transform> m_transform;
		std::shared_ptr<Gravity> m_gravity;
		std::shared_ptr<PNTBoneModelDraw> m_draw;

		float m_Speed;
	public:
		// 構築と破棄
		EnemyAlpaca(const shared_ptr<Stage>& stage,const STRUCT_ObjectParam& objectParam) :
			EnemyBase(stage,objectParam),
			m_Speed(1.0f)
		{
		}
		virtual ~EnemyAlpaca()
		{
		}

		// ステートマシン
		unique_ptr<StateMachine<EnemyAlpaca>> m_eStateMachine;

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画
		
		// 体力のゲッター
		float GetEnemyAlpacaHP()
		{
			return m_EnemyHP;
		}
	};

	class IdleState : public ObjState<EnemyAlpaca>
	{
		IdleState() {};
	public:
		static shared_ptr<IdleState> Instance();
		virtual void Enter(const shared_ptr<EnemyAlpaca>& obj) override;
		virtual void Execute(const shared_ptr<EnemyAlpaca>& obj) override;
		virtual void Exit(const shared_ptr<EnemyAlpaca>& obj) override;
	};

	class AngryState : public ObjState<EnemyAlpaca>
	{
		AngryState() {};
	public:
		static shared_ptr<AngryState> Instance();
		virtual void Enter(const shared_ptr<EnemyAlpaca>& obj) override;
		virtual void Execute(const shared_ptr<EnemyAlpaca>& obj) override;
		virtual void Exit(const shared_ptr<EnemyAlpaca>& obj) override;
	};
}
//end basecross
