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
	private:
		InputManager() {}
		virtual ~InputManager() {}
	public:
		static InputManager& Instance()
		{
			static InputManager instance;
			return instance;
		}

		void Initialize();
		void Update();

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

		// GameMode：Editer用の入力関数
		// 焦点固定視点移動
		void FocusFixedViewPointMove();
		
		// カメラ位置固定視点移動
		void CameraFixedViewPointMove();
		
		// Pressed
		void PressedLMouseButton();
		void PressedDelete();
		void PressedQKey();
		void PressedWKey();
		void PressedEKey();
		void PressedRKey();
		void PressedYKey();
		void PressedUKey();
	};
}
