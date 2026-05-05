/*!
@file EnemyBase.h
@brief 敵の基底クラス
@copyright 村岡
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class EnemyBase : public GameObject
	{
	private:
		// 代入するためのメンバ変数
		// 徘徊時間
		float m_InitialWanderingTime;
		float m_WanderSpeed;
		bool m_isWandering;
		// 待機時間
		float m_InitialStandTime;
		bool m_isStand;

		// 徘徊の移動のやつ
		enum Wandering
		{
			move_x_puls,
			move_x_minus,
			move_z_puls,
			move_z_minus,

			Number
		}m_wandering;
	public:
		EnemyBase(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_InitialWanderingTime(0.0f),
			m_WanderSpeed(1.0f),
			m_isWandering(false),
			m_InitialStandTime(0.0f),
			m_isStand(true)
		{
		}

		~EnemyBase()
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void Move
		(
			const shared_ptr<GameObject>& gameObject,
			float speed
		);

		void DebugString();
	};
}