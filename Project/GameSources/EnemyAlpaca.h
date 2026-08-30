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

		int m_Num;

		EffectHandle m_effectHandle;
		bool m_isStanEffectDraw;
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
		//unique_ptr<StateMachine<EnemyAlpaca>> m_eStateMachine;

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		void DebugString();

		// --- 当たり判定 ---
		void OnCollisionEnter(shared_ptr<GameObject>& Other);	//	当たり判定
		void OnCollisionExecute(shared_ptr<GameObject>& Other);
		void OnCollisionExit(shared_ptr<GameObject>& Other);
		// ------------------
	};

	class IdleState : public ObjState<EnemyBase>
	{
		IdleState() {};
	public:
		static shared_ptr<IdleState> Instance();
		virtual void Enter(const shared_ptr<EnemyBase>& obj) override;
		virtual void Execute(const shared_ptr<EnemyBase>& obj) override;
		virtual void Exit(const shared_ptr<EnemyBase>& obj) override;
	};

	class AngryState : public ObjState<EnemyBase>
	{
		AngryState() {};
	public:
		static shared_ptr<AngryState> Instance();
		virtual void Enter(const shared_ptr<EnemyBase>& obj) override;
		virtual void Execute(const shared_ptr<EnemyBase>& obj) override;
		virtual void Exit(const shared_ptr<EnemyBase>& obj) override;
	};

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
