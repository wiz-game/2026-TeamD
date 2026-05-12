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
		// ランダムに徘徊させる変数
		float m_RandRotation;
		float m_angle;
	public:
		EnemyBase(const shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_InitialWanderingTime(0.0f),
			m_RandRotation(0.0f),
			m_WanderSpeed(1.0f),
			m_isWandering(false),
			m_InitialStandTime(0.0f),
			m_isStand(true),
			m_angle(0.0f)
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

		void CircleMove
		(
			const shared_ptr<GameObject>& gameObject,
			float radius,		// 回転半径
			float moveSpeed		// 回転速度
		);

		void DebugString();
		void DebugDraw();
	};
}