#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void InputManager::Initialize()
	{
		m_pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		m_key = App::GetApp()->GetInputDevice().GetKeyState();
	}

	void InputManager::Update()
	{
		switch (GameManager::Instance().GetGameMode())
		{
		default:
			break;
		case ENUM_GameMode::Play :
			// プレイヤーの移動
			if (m_pad.fThumbLX > STACK_DEADZONE_L || m_pad.fThumbLX < -STACK_DEADZONE_L ||
				m_pad.fThumbLY > STACK_DEADZONE_L || m_pad.fThumbLY < -STACK_DEADZONE_L)
			{
				Moves();
			}

			// 視点移動
			if (m_pad.fThumbRX > STACK_DEADZONE_R || m_pad.fThumbRX < -STACK_DEADZONE_R ||
				m_pad.fThumbRY > STACK_DEADZONE_R || m_pad.fThumbRY < -STACK_DEADZONE_R)
			{
				MoveCamera();
			}

			// ジャンプ/戻る
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B)
			{
				PressedB();
			}

			// 決定
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PressedA();
			}

			// 照準
			if (m_pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				PushLThumb();
			}

			// 泡発射
			if (m_pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				PushRThumb();
			}

			// ポーズメニュー
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				PressedStart();
			}
			break;
		case ENUM_GameMode::Editor:
			// 注視点固定視点移動
			// カメラ位置固定視点移動
			// オブジェクト選択
			// オブジェクト削除
			// 選択解除
			// 座標移動へ切替
			// 回転へ切替
			// サイズへ切替
			// 生成オブジェクト切替
			// エディターモード切替
			break;
		}
	}

	void InputManager::Moves()
	{

	}

	void InputManager::MoveCamera()
	{
	}

	void InputManager::PushLThumb()
	{
	}

	void InputManager::PushRThumb()
	{
	}

	void InputManager::PressedA()
	{
	}

	void InputManager::PressedB()
	{
	}

	void InputManager::PressedStart()
	{
	}
}