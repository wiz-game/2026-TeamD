#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void InputManager::Initialize()
	{
		m_key = App::GetApp()->GetInputDevice().GetKeyState();
		m_beforeMouseClientPoint = m_key.m_MouseClientPoint;
	}

	void InputManager::Update()
	{
		m_pad = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		m_key = App::GetApp()->GetInputDevice().GetKeyState();
		
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
			if (m_pad.bLeftTrigger > LEFT_TRIGGER_DEADZONE)
			{
				PushLTrigger();
			}
			// 照準解除
			else if (m_pad.bLeftTrigger <= LEFT_TRIGGER_DEADZONE)
			{
				ReleasedLTrigger();
			}

			// 泡発射
			if (m_pad.bRightTrigger > RIGHT_TRIGGER_DEADZONE)
			{
				PushRTrigger();
			}

			// ポーズメニュー
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				PressedStart();
			}
			break;
		case ENUM_GameMode::Editor:
			// 焦点固定視点移動
			if ((m_key.m_MouseClientPoint != m_beforeMouseClientPoint) &&
				m_key.m_bPushKeyTbl[VK_TAB] && m_key.m_bPushKeyTbl[VK_LBUTTON])
			{
				FocusFixedViewPointMove();
			}

			// カメラ位置固定視点移動
			if ((m_key.m_MouseClientPoint != m_beforeMouseClientPoint) &&
				m_key.m_bPushKeyTbl[VK_RBUTTON])
			{
				CameraFixedViewPointMove();
			}

			// 生成オブジェクト切替
			if (m_key.m_bPressedKeyTbl['Y'] || m_key.m_bPressedKeyTbl['U'])
			{
				ChangeObject();
			}
			
			// オブジェクト選択
			if (m_key.m_bPressedKeyTbl[VK_LBUTTON] && !m_key.m_bPushKeyTbl[VK_TAB])
			{
				PressedLMouseButton();
			}

			// オブジェクト削除
			if (m_key.m_bPressedKeyTbl[VK_DELETE])
			{
				PressedDelete();
			}

			// 選択解除
			if (m_key.m_bPressedKeyTbl['Q'])
			{
				PressedQKey();
			}

			// 座標移動へ切替
			if (m_key.m_bPressedKeyTbl['W'])
			{
				PressedWKey();
			}

			// 回転へ切替
			if (m_key.m_bPressedKeyTbl['E'])
			{
				PressedEKey();
			}

			// サイズへ切替
			if (m_key.m_bPressedKeyTbl['R'])
			{
				PressedRKey();
			}

			// オブジェクトを生成
			if (m_key.m_bPressedKeyTbl['F'])
			{
				PressedFKey();
			}
			break;
		}

		// エディターモード切替
		if (m_key.m_bPressedKeyTbl['C'])
		{
			PressedCKey();
		}

		// マウスポイントの更新
		m_beforeMouseClientPoint = m_key.m_MouseClientPoint;
	}

	void InputManager::Moves()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		float moveSpeed = 1.8f;
		if (!stage) return;

		stage->GetSharedGameObject<Player>(L"Player")->GetComponent<Move>()
			->VectorMove(Vec3(m_pad.fThumbLX * moveSpeed, 0.0f, m_pad.fThumbLY * moveSpeed));
	}

	void InputManager::MoveCamera()
	{
		GetMyCamera()->PlayModeMove
		(
			m_pad.fThumbRX,
			m_pad.fThumbRY,
			m_pad.fThumbLX,
			m_pad.fThumbLY
		);
	}

	void InputManager::PushLTrigger()
	{
		GetMyCamera()->SetIsAiming(true);
	}

	void InputManager::PushRTrigger()
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

	void InputManager::ReleasedLTrigger()
	{
		GetMyCamera()->SetIsAiming(false);
	}

	void InputManager::FocusFixedViewPointMove()
	{
		auto mousePoint = Point2D<int>
		(
			m_key.m_MouseClientPoint.x - m_beforeMouseClientPoint.x, 
			m_key.m_MouseClientPoint.y - m_beforeMouseClientPoint.y
		);
		GetMyCamera()->FocusFixedViewPointMove(mousePoint);
	}
	
	void InputManager::CameraFixedViewPointMove()
	{
		GetMyCamera()->CameraFixedViewPointMove(m_key.m_MouseClientPoint);
	}

	void InputManager::ChangeObject()
	{
	}

	void InputManager::PressedLMouseButton()
	{
		StageEditor::Instance().PressedLMouseButton(m_key.m_MouseClientPoint);
	}

	void InputManager::PressedDelete()
	{
	}

	void InputManager::PressedQKey()
	{
		StageEditor::Instance().DeselectObj();
	}

	void InputManager::PressedWKey()
	{
	}

	void InputManager::PressedEKey()
	{
	}

	void InputManager::PressedRKey()
	{
	}

	void InputManager::PressedFKey()
	{
		StageEditor::Instance().AddGameObject();
	}

	void InputManager::PressedCKey()
	{
		if (GameManager::Instance().GetGameMode() == ENUM_GameMode::Play)
		{
			GameManager::Instance().SetGameMode(ENUM_GameMode::Editor);
		}
		else
		{
			GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
		}
	}
}