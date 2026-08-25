/*!
@file Player.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "PNTDXModelDraw.h"

namespace basecross 
{
	enum class PlayerState
	{
		Default,
		PowerUp,
		Dead
	};

	// GameObjectクラスを継承した「Player」クラスを定義
	class Player : public GameObject // GameObjectクラスの継承【必須】
	{
		PlayerState m_playerState;
		std::shared_ptr<Transform> m_transform; // トランスフォームはよく使うのでメンバにしておく
		std::shared_ptr<PNTDXModelDraw> m_draw; // ドローコンポーネント
		std::shared_ptr<PNTBoneModelDraw> m_pntDraw; // ドローコンポーネント
		std::shared_ptr<Move> m_move;
		std::shared_ptr<Gravity> m_gravity;

		std::shared_ptr<Bubble> m_pBubble;

		float m_stickRX; // X軸中心のカメラの回り込み
		float m_stickRY; // Y軸中心のカメラの回り込み

		float m_PlayerHP;

		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		Vec3 m_VecJumpPower;
		bool m_isTargetMode;
		// 重力の処理
		float m_Velocity;
		float m_Gravity;
		// ジャンプの処理
		float m_JumpPower;
		bool m_isJumping; // 現在ジャンプしているかどうか

		bool m_Bresing;

		bool m_isDead;
		
		float m_Attack;

		// 強化状態
		bool m_BubblePowerCoolDown;		// クールダウン
		float m_initCoolDown;

		bool m_BubblePowerScale;		// 泡の大きさ
		bool m_BubblePowerUnbreaking;	// 耐久力
		bool m_BubblePowerLaunchRate;	// 発射レート
		bool m_BubblePowerBulletSpeed;	// 弾速
		
		// 泡を吐いたクールタイムを格納するための変数
		// クールダウンは"LaunchofBubble()"の中にローカル変数として入っているので、そこを変えてださい
		float m_cooldown;

		bool m_iseatSoap;

		// プレイヤーがパワーアップしたか
		bool m_isPlayerPowerUp;
		// カウント時間
		Timer m_timer;

		wstring nowAnimation;

		bool m_isBubbleAnimationEnd;
		bool m_isMoveStop;
		bool m_isStartStop;

		EffectHandle m_effectHandle;
	public :
		// ステージを引数にしたコンストラクタ【必須】
		Player(const std::shared_ptr<Stage>& stage, const Vec3& position) :
			GameObject(stage), // ステージをGameObjectに渡す【必須】
			m_Velocity(10.0f),
			m_Position(position),
			m_Rotation(0.0f, 1.5f, 0.0f),
			m_Scale(1.0f, 1.5f, 1.5f),
			m_VecJumpPower(0.0f, 9.8f, 0.0f),
			m_isJumping(false),
			m_JumpPower(6.0f),
			m_Gravity(9.8f),
			m_isTargetMode(false),
			m_stickRY(2.0f),
			m_stickRX(0.0f),
			m_Bresing(false),
			m_cooldown(0.0f),
			m_PlayerHP(10.0f),
			m_BubblePowerCoolDown(false),
			m_BubblePowerScale(false),
			m_BubblePowerUnbreaking(false),
			m_BubblePowerLaunchRate(false),
			m_BubblePowerBulletSpeed(false),
			m_Attack(2),
			// クールダウンの初期値
			m_initCoolDown(0.6),
			m_iseatSoap(false),
			m_isDead(false),
			m_isPlayerPowerUp(false),
			m_playerState(PlayerState::Default),
			m_timer(0.0f),
			nowAnimation(L"Idel"),
			m_isBubbleAnimationEnd(true),
			m_isMoveStop(true),
			m_isStartStop(true)
		{
		}

		void OnCreate() override; // 初期設定用の関数(UnityのStartメソッドに相当)
		void OnUpdate() override; // 毎フレーム実行される関数(UnityのUpdateメソッドに相当)
		void Jump();	// ジャンプ
		void LaunchofBubble();	// 泡の発射
		void DebugString();
		void ReSpawn();
		void DirtDamage();
		void PlayerDied();

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

		// クールダウンのセッター
		void SetCoolDown(bool sed)
		{
			m_BubblePowerCoolDown = sed;
		}

		bool GetEatSoap()
		{
			return m_iseatSoap;
		}

		shared_ptr<Bubble> GetHaveBubble()
		{
			return m_pBubble;
		}

		float GetAttack()
		{
			return m_Attack;
		}

		bool GetPlayerPowerUp()
		{
			return m_isPlayerPowerUp;
		}

		void SetBresing(bool flag)
		{
			m_Bresing = flag;
		}

		bool GetBresing()
		{
			return m_Bresing;
		}

		void SetCoolDown(float cd)
		{
			m_cooldown = cd;
		}

		bool GetDeadFlag()
		{
			return m_isDead;
		}

		void SetBubbleAnimationEndFlag(bool flag)
		{
			m_isBubbleAnimationEnd = flag;
		}

		bool GetBubbleAnimationEndFlag()
		{
			return m_isBubbleAnimationEnd;
		}

		void SetMoveStopFlag(bool flag)
		{
			m_isMoveStop = flag;
		}

		bool GetMoveStopFlag()
		{
			return m_isMoveStop;
		}

		void CreateBubble();

		void SetPlayerState(PlayerState state);
		void EnterPlayerState(PlayerState state);
		void ExitPlayerState(PlayerState state);

		void PlayerAnimation();
		void PlayerChangeAnimation(const wstring& newAnim,bool forceRestart = false);
		void OnMoveInput();
		void OnRTriggerInput();
		void OnRTriggerRelese();
		void PlayGameAnimation();
		void BubbleEffect();
	};
}
//end basecross

