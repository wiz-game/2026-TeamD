/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross 
{
	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<Move> m_move;

		float m_angleY; // Y軸中心のカメラの回り込み

		Vec3 m_Position;
		bool m_isTargetMode;
		// 重力の処理
		float m_Velocity;
		float m_Gravity;
		// ジャンプの処理
		float m_JumpPower;
		bool m_isJumping; // 現在ジャンプしているかどうか
		// カメラの傾き具合
		float m_angleX;
	public :
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_Velocity(0.0f),
			m_Position(0.0f, 1.0f, 0.0f),
			m_isJumping(false),
			m_JumpPower(6.0f),
			m_Gravity(9.8f),
			m_isTargetMode(true)
		{
		}

		void OnCreate() override; // 初期設定用の関数(UnityのStartメソッドに相当)
		void OnUpdate() override; // 毎フレーム実行される関数(UnityのUpdateメソッドに相当)
		void Jump();	// ジャンプ
		void LaunchofBubble();	// 泡の発射
		void Camera();		// カメラ移動

		// --- 当たり判定 ---
		void OnCollisionEnter(shared_ptr<GameObject>& Other);	//	当たり判定
		void OnCollisionExecute(shared_ptr<GameObject>& Other);
		void OnCollisionExit(shared_ptr<GameObject>& Other);
		// ------------------

		// ジャンプのゲッターセッター
		bool GetIsJump()
		{
			return m_isJumping;
		}
		void SetIsJump(bool j)
		{
			m_isJumping = j;
		}

		// 照準モードのゲッターセッター
		bool GetTargetMode()
		{
			return m_isTargetMode;
		}
		void SetTargetMode(bool tm)
		{
			m_isTargetMode = tm;
		}
	};
}
//end basecross

