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

		std::shared_ptr<Bubble> m_pBubble;

		float m_stickRX; // X軸中心のカメラの回り込み
		float m_stickRY; // Y軸中心のカメラの回り込み

		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		bool m_isTargetMode;
		// 重力の処理
		float m_Velocity;
		float m_Gravity;
		// ジャンプの処理
		float m_JumpPower;
		bool m_isJumping; // 現在ジャンプしているかどうか

		bool m_Bresing;

		// 泡を吐いたクールタイムを格納するための変数
		// クールダウンは"LaunchofBubble()"の中にローカル変数として入っているので、そこを変えてださい
		float m_cooldown;
	public :
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_Velocity(0.0f),
			m_Position(0.0f, 60.2f, 0.0f),
			m_Rotation(0.0f, 1.5f, 0.0f),
			m_Scale(0.3f, 0.3f, 0.3f),
			m_isJumping(false),
			m_JumpPower(6.0f),
			m_Gravity(9.8f),
			m_isTargetMode(false),
			m_stickRY(2.0f),
			m_stickRX(0.0f),
			m_Bresing(false),
			m_cooldown(0.0f)
		{
		}

		void OnCreate() override; // 初期設定用の関数(UnityのStartメソッドに相当)
		void OnUpdate() override; // 毎フレーム実行される関数(UnityのUpdateメソッドに相当)
		void Jump();	// ジャンプ
		void LaunchofBubble();	// 泡の発射
		void DebugString();
		void ReSpawn();

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

		// デバッグ用のゲッターセッター
		float GetStickRY()
		{
			return m_stickRY;
		}
	};
}
//end basecross

