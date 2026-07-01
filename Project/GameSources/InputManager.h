/*!
@file InputManager.h
@brief 入力マネージャー
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class InputManager
	{
	private:
		CONTROLER_STATE m_pad = CONTROLER_STATE();
		KEYBOARD_STATE m_key = KEYBOARD_STATE();

		const float STACK_DEADZONE_L = 0.0f;
		const float STACK_DEADZONE_R = 0.0f;

		const char LEFT_TRIGGER_DEADZONE = 125;
		const char RIGHT_TRIGGER_DEADZONE = 125;
		
		Point2D<int> m_beforeMouseClientPoint = Point2D<int>();
		int m_beforeWheelDelta = 0;
		int m_wheelDelta = 0;

		// 移動速度
		float m_MoveSpeed = 0.6f;

		// 時間
		Timer m_timer = 0.0f;
		// アニメーション再生で止まる時間
		Timer m_moveStopTimer = 0.0f;
		// 泡の発射レート
		Timer m_bubbleRateTimer = 0.0f;
		// エフェクトを表示するか
		bool  m_isEffectDraw = false;
		// 左右どちらかの判定
		bool  m_isRight = false;

		// コントローラーの入力のオンオフ
		bool m_isInputEnabled = true;

	public:
		static InputManager& Instance()
		{
			static InputManager instance;
			return instance;
		}

		void Initialize();
		void Update();

		void SetWheelDelta(int wheelDelta) { m_wheelDelta += wheelDelta; }
		void ResetWheelDelta() { m_wheelDelta = 0; }

		// コントローラーの入力切り替え
		void SetInputEnabled(bool enabled) { m_isInputEnabled = enabled; }

		// 移動速度のゲッターセッター
		float GetMoveSpeed()
		{
			return m_MoveSpeed;
		}
		void SetMoveSpeed(float ms)
		{
			m_MoveSpeed = ms;
		}
	private:
		InputManager() {}
		virtual ~InputManager() {}

		shared_ptr<MyCamera> GetMyCamera() const
		{
			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			if (!stage) return nullptr;

			auto camera = stage->GetView()->GetTargetCamera();
			if (!camera) return nullptr;

			return dynamic_pointer_cast<MyCamera>(camera);
		}

		// GameMode：Title用の入力関数
		void GameStart();

		// GameMode：Select用の入力関数
		void StageStart();
		void ReturnTitle();
		void ChangeSelectGameStage();

		// GameMode：Play用の入力関数
		// 移動入力
		void Moves();

		// 視点移動入力
		void MoveCamera();

		// Push
		void PushLTrigger();
		void PushRTrigger();

		// Pressed
		void PressedA();
		void PressedB();
		void PressedStart();
		void PressedRButton();
		void PressedLButton();

		// Released
		void ReleasedLTrigger();

		// GameMode：Menu用の入力関数
		void ReturnGame();
		void EnterSetting();
		void EnterHowtoplay();
		void ReturnOneMenu();
		void MoveMenuCursor();
		void PressedAMenu();
		void ReleasedAMenu();

		// GameMode：Editer用の入力関数
		// ギズモによる操作
		void ObjectOperation();
		void ObjectOperationEnd(); // ギズモの選択解除

		// 焦点固定視点移動
		void FocusFixedViewPointMove();
		
		// カメラ位置固定視点移動
		void CameraFixedViewPointMove();

		// マウスホイールによるカメラ距離の操作
		void WheelCameraDistance();

		// 生成オブジェクト切替
		void ChangeObject();

		// ステージ保存
		void SaveStage();

		// コピー
		void SelectCopy();

		// Pressed
		void PressedLMouseButton();
		void PressedDelete();
		void PressedQKey();
		void PressedWKey();
		void PressedEKey();
		void PressedRKey();
		void PressedFKey();
		void PressedCKey();
		void PressedVKey();

		void IdelAnimation();
		void RTriggerRelse();

	};
}
