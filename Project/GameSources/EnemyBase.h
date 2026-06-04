/*!
@file EnemyBase.h
@brief 敵の基底クラス
@copyright 村岡
*/

#pragma once
#include "stdafx.h"
#include "ItemBase.h"

namespace basecross
{
	class EnemyBase : public GameObject
	{
	private:
		std::shared_ptr<Transform> m_transform;
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
		// 初期値のポジションを格納するための変数
		Vec3 m_InitialPosition;
		Vec3 m_TargetPosition;

		// 1度だけ原点として記憶するやつ
		bool m_isFirstTime;


		enum Point
		{
			Point0,
			Point1,
			Point2,
			Point3,

			Number
		}
		m_NumPoint;

	protected:
		float m_EnemyHP;
		bool m_Detection;

		float m_LaunchofDirtCoolDown;
		float m_InitCoolDown;

		bool m_isRotated;

		float m_rotationSpeed;
		float m_rotY;

		float m_startRotY = 0.0f;
	public:
		EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam) :
			GameObject(stage, objectParam),
			m_InitialWanderingTime(0.0f),
			m_RandRotation(0.0f),
			m_WanderSpeed(1.0f),
			m_isWandering(false),
			m_InitialStandTime(0.0f),
			m_isStand(true),
			m_angle(0.0f),
			m_TargetPosition(),
			m_isFirstTime(true),
			m_Detection(false),
			m_LaunchofDirtCoolDown(0.0f),
			m_InitCoolDown(3.0f),
			m_isRotated(false),
			m_rotationSpeed(2.0f),
			m_rotY(0.0f)
			//m_EnemyHP(hp)
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

		void PointMove
		(
			const shared_ptr<GameObject>& gameObject,
			float speed
		);

		void DebugString();

		void Died(const shared_ptr<GameObject>& gameObject);
		void DetectionRange(const shared_ptr<GameObject>& gameObject);
		void Stalker(const shared_ptr<GameObject>& gameObject, float stalkerSpeed);
		void FunctionGravity(const shared_ptr<GameObject>& gameObject);
		void DropDirt(const shared_ptr<GameObject>& gameObject);
		void MazeWandering(const shared_ptr<GameObject>& gameObject);
		void aStar(const shared_ptr<GameObject>& gameObject);

		// ゲッターセッター関数
		bool GetDetection()
		{
			return m_Detection;
		}

		bool GetRotation()
		{
			return m_isRotated;
		}

		// --- 当たり判定 ---
		void OnCollisionEnter(shared_ptr<GameObject>& Other);	//	当たり判定
		void OnCollisionExecute(shared_ptr<GameObject>& Other);
		void OnCollisionExit(shared_ptr<GameObject>& Other);
		// ------------------
	};
}