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

		float m_Speed;

		float m_rotToHeadLeap;

		// 座標を格納するための変数
		Vec3 m_Point1Position;
		Vec3 m_Point2Position;
		Vec3 m_Point3Position;

		enum Point
		{
			Point0,
			Point1,
			Point2,
			Point3,

			Number
		}
		m_NumPoint;

		//enum Vec3 

		enum ENUM_RANDOMROT
		{
			RandomRotL,
			RandomRotR,

			RandomRotNum,
		}m_NumRandRot;

	protected:
		float m_EnemyHP;
		bool m_Detection;

		float m_LaunchofDirtCoolDown;
		float m_InitCoolDown;

		bool m_isRotated;

		float m_rotationSpeed;
		float m_rotY;

		float m_startRotY = 0.0f;

		// 左が行けるか
		bool m_canGoLeft;
		// 右が行けるか
		bool m_canGoRight;

		bool m_canGoForward;

		float m_targetRotY;

		// レイの大きさ
		float m_rayRange;

		// 壁を回避中かどうか
		bool m_isAvoiding;
		float m_avoidTimer;
		float m_InitavoidTimer;

		Vec3 m_targetVec;
	public:
		EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam);

		~EnemyBase()
		{
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//void Move(const shared_ptr<GameObject>& gameObject, float speed);
		//void CircleMove(const shared_ptr<GameObject>& gameObject, float radius, float moveSpeed);
		void PointMove(const shared_ptr<GameObject>& gameObject,float speed);

		void DebugString();

		void Died(const shared_ptr<GameObject>& gameObject);
		void DetectionRange(const shared_ptr<GameObject>& gameObject);
		//void Stalker(const shared_ptr<GameObject>& gameObject, float stalkerSpeed);
		//void FunctionGravity(const shared_ptr<GameObject>& gameObject);
		//void DropDirt(const shared_ptr<GameObject>& gameObject);
		void MazeWandering(const shared_ptr<GameObject>& gameObject);
		//void aStar(const shared_ptr<GameObject>& gameObject);
		void Tracking(const shared_ptr<GameObject>& gameObject,float speed);

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