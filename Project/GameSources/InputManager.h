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
	public:
		static InputManager& Instance()
		{
			static InputManager instance;
			return instance;
		}

		void Initialize();
		void Update();
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

		// Released
		void ReleasedLTrigger();

		// GameMode：Editer用の入力関数
		// 焦点固定視点移動
		void FocusFixedViewPointMove();
		
		// カメラ位置固定視点移動
		void CameraFixedViewPointMove();
		
		// 生成オブジェクト切替
		void ChangeObject();

		// Pressed
		void PressedLMouseButton();
		void PressedDelete();
		void PressedQKey();
		void PressedWKey();
		void PressedEKey();
		void PressedRKey();
		void PressedFKey();
		void PressedCKey();
	};
}
