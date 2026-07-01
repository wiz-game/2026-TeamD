/*!
@file EnemyAlpacaFixedMove.h
@brief 徘徊する敵キャラクター
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public EnemyBase;
	//--------------------------------------------------------------------------------------
	class EnemyAlpacaFixedMove : public EnemyBase
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
		EnemyAlpacaFixedMove(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam) :
			EnemyBase(stage, objectParam),
			m_Speed(4.0f)
		{}
		virtual ~EnemyAlpacaFixedMove()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		// 体力のゲッター
		float GetEnemyAlpacaHP()
		{
			return m_EnemyHP;
		}
	};
}
//end basecross
