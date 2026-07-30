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
		bool m_isWandering;
		// 待機時間
		float m_InitialStandTime;
		bool m_isStand;
		// ランダムに徘徊させる変数
		float m_RandRotation;
		// 初期値のポジションを格納するための変数
		Vec3 m_InitialPosition;
		Vec3 m_TargetPosition;

		// 1度だけ原点として記憶するやつ
		bool m_isFirstTime;

		float m_Speed;

		float m_rotToHeadLeap;

		float m_StunTime;
		float m_InitStunTime;
		bool m_isContactofBubble;

		// 壁に触れたかどうか
		bool m_isHitWall;

		float m_ExpectRange;

		enum Point
		{
			Point0,
			Point1,
			Point2,
			Point3,

			Number
		}
		m_NumPoint;

		enum ENUM_RANDOMROT
		{
			RandomRotL,
			RandomRotR,

			RandomRotNum,
		}m_NumRandRot;

	protected:
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
		float m_rayDistanceRange;

		// 壁を回避中かどうか
		bool m_isAvoiding;
		float m_avoidTimer;
		float m_InitavoidTimer;

		Vec3 m_targetVec;

		Vec3 m_closePlayerPos = Vec3();

		vector<Vec3> m_PointPositions;
	public:
		EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam);

		~EnemyBase()
		{
		}
		unique_ptr<StateMachine<EnemyBase>> m_eStateMachine;

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// 敵のAI
		// 指定した場所に移動する
		void PointMove(const shared_ptr<GameObject>& gameObject, float speed);
		void PointPosition(int number, const Vec3& pos);

		void DetectionRange(const shared_ptr<GameObject>& gameObject); // 索敵範囲
		void MazeWandering(const shared_ptr<GameObject>& gameObject); // 徘徊AI
		void Tracking(const shared_ptr<GameObject>& gameObject); // 追跡AI
		void EstimatedPlayerLocation(const shared_ptr<GameObject>& gameObject); // 推定プレイヤー位置に移動する

		// ヘルパー関数
		// angleは度数法で書いてください
		Vec3 CalculateEndPointRayAngle(const Vec3& startPos,const float& forwardAngle,const float& angle,float rayRange);
		const bool IsWallHit(const shared_ptr<PNTStaticDraw>& staticDraw, const Vec3& startPos, const Vec3& endPos, const Vec3& basePos);

		void Stun();

		// ゲッターセッター関数
		const bool GetDetection()
		{
			return m_Detection;
		}

		const bool GetRotation()
		{
			return m_isRotated;
		}

		const bool GetAvoiding()
		{
			return m_isAvoiding;
		}

		const bool GetIsHitWall()
		{
			return m_isHitWall;
		}

		const float GetStunTime()
		{
			return m_StunTime;
		}

		const float GetContatofBubble()
		{
			return m_isContactofBubble;
		}

		void SetIsContactOfBubbleetStun(bool Contact)
		{
			m_isContactofBubble = Contact;
		}

		void SetCanGoForward(bool scgf)
		{
			m_canGoForward = scgf;
		}

		void SetCanGoRight(bool scgr)
		{
			m_canGoRight = scgr;
		}

		void SetCanGoLeft(bool scgl)
		{
			m_canGoLeft = scgl;
		}

		void SetPointPosition(const Vec3& pos1,const Vec3& pos2,const Vec3& pos3)
		{
			PointPosition(1, pos1);
			PointPosition(2, pos2);
			PointPosition(3, pos3);
		}
	};
}