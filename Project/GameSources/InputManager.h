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
		void PushLThumb();
		void PushRThumb();

		// Pressed
		void PressedA();
		void PressedB();
		void PressedStart();

		// GameMode：Editer用の入力関数

	};
}
