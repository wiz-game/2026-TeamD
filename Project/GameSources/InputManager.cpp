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
			// オブジェクト選択とギズモの選択
			if (m_key.m_bPressedKeyTbl[VK_LBUTTON] && !m_key.m_bPushKeyTbl[VK_TAB])
			{
				PressedLMouseButton();
			}

			// ギズモによる操作
			if (m_key.m_MouseClientPoint != m_beforeMouseClientPoint && 
				m_key.m_bPushKeyTbl[VK_LBUTTON] && !m_key.m_bPressedKeyTbl[VK_LBUTTON])
			{
				ObjectOperation();
			}
			
			// ギズモの選択解除
			if (m_key.m_bUpKeyTbl[VK_LBUTTON])
			{
				ObjectOperationEnd();
			}

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

			// カメラ距離調整
			if (m_wheelDelta != m_beforeWheelDelta)
			{
				WheelCameraDistance();
			}

			// 生成オブジェクト切替
			if (m_key.m_bPressedKeyTbl['Y'] || m_key.m_bPressedKeyTbl['U'])
			{
				ChangeObject();
			}

			// ステージ保存
			if (m_key.m_bPressedKeyTbl['S'] && m_key.m_bPushKeyTbl[VK_LCONTROL])
			{
				SaveStage();
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

			// サイズへ切替
			if (m_key.m_bPressedKeyTbl['E'])
			{
				PressedEKey();
			}

			// 回転へ切替
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
		if (m_key.m_bPressedKeyTbl['C'] && GameManager::Instance().GetIsDebug())
		{
			PressedCKey();
		}

		// マウスポイントの更新
		m_beforeMouseClientPoint = m_key.m_MouseClientPoint;
		m_beforeWheelDelta = m_wheelDelta;
	}

	void InputManager::Moves()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		stage->GetSharedGameObject<Player>(L"Player")->GetComponent<Move>()
			->VectorMove(Vec3(m_pad.fThumbLX * m_MoveSpeed, 0.0f, m_pad.fThumbLY * m_MoveSpeed));
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

	void InputManager::ObjectOperation()
	{
		auto mousePoint = Point2D<int>
			(
				m_key.m_MouseClientPoint.x - m_beforeMouseClientPoint.x,
				m_key.m_MouseClientPoint.y - m_beforeMouseClientPoint.y
			);
		StageEditor::Instance().ObjectOperation(mousePoint);
	}

	void InputManager::ObjectOperationEnd()
	{
		StageEditor::Instance().DeselectGizmo();
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
		auto mousePoint = Point2D<int>
			(
				m_key.m_MouseClientPoint.x - m_beforeMouseClientPoint.x,
				m_key.m_MouseClientPoint.y - m_beforeMouseClientPoint.y
			);
		GetMyCamera()->CameraFixedViewPointMove(mousePoint);
	}

	void InputManager::WheelCameraDistance()
	{
		auto wheelDelta = m_wheelDelta - m_beforeWheelDelta;
		GetMyCamera()->WheelCameraDistance(wheelDelta);
	}

	void InputManager::ChangeObject()
	{
		StageEditor::Instance().ChangeObject(m_key.m_bPressedKeyTbl['Y'], m_key.m_bPressedKeyTbl['U']);
	}

	void InputManager::SaveStage()
	{
		StageEditor::Instance().WriteStageData();
	}

	void InputManager::PressedLMouseButton()
	{
		StageEditor::Instance().PressedLMouseButton(m_key.m_MouseClientPoint);
	}

	void InputManager::PressedDelete()
	{
		StageEditor::Instance().PressedDelete();
	}

	void InputManager::PressedQKey()
	{
		StageEditor::Instance().DeselectObj();
		StageEditor::Instance().GizmoDrawActive(false);
	}

	void InputManager::PressedWKey()
	{
		StageEditor::Instance().SelectEditorMode(ENUM_EditorMode::Position);
	}

	void InputManager::PressedEKey()
	{
		StageEditor::Instance().SelectEditorMode(ENUM_EditorMode::Scale);
	}

	void InputManager::PressedRKey()
	{
		StageEditor::Instance().SelectEditorMode(ENUM_EditorMode::Quaternion);
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