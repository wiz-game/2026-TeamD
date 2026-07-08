#include "stdafx.h"
#include "Project.h"
#include "InputManager.h"

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
		case ENUM_GameMode::Title:
			// Aを押してゲームスタート
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				GameStart();
			}
			break;
		case ENUM_GameMode::Select:
			// Aを押して選択されているステージへ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				StageStart();
			}

			// Bを押してタイトルへ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B)
			{
				ReturnTitle();
			}

			// 十字キー左右でステージ選択
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT ||
				m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				ChangeSelectGameStage();
			}
			break;
		case ENUM_GameMode::GameClear:
			// Aを押してリトライ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				StageStart();
			}
			// Bを押してタイトルへ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B)
			{
				ReturnTitle();
			}
			break;
		case ENUM_GameMode::GameOver:
			// Aを押してリトライ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				StageStart();
			}
			// Bを押してタイトルへ
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B)
			{
				ReturnTitle();
			}
			break;
		case ENUM_GameMode::Play:
			// プレイヤーの移動
			if ((m_pad.fThumbLX > STACK_DEADZONE_L || m_pad.fThumbLX < -STACK_DEADZONE_L ||
				m_pad.fThumbLY > STACK_DEADZONE_L || m_pad.fThumbLY < -STACK_DEADZONE_L))
			{
				Moves();
			}
			else
			{
				IdelAnimation();
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
			else
			{
				RTriggerRelse();
			}

			// ポーズメニュー
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				PressedStart();
			}

			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
				!(m_pad.bRightTrigger > RIGHT_TRIGGER_DEADZONE) &&
				!(m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
			{
				PressedRButton();
			}

			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER &&
				!(m_pad.bRightTrigger > RIGHT_TRIGGER_DEADZONE) &&
				!(m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
			{
				PressedLButton();
			}

			break;
		case ENUM_GameMode::Menu:
			// ゲームに戻る
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_START)
			{
				ReturnGame();
			}

			// 一つ戻る
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B)
			{
				ReturnGame();
			}

			// カーソルを移動
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP ||
				m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				MoveMenuCursor();
			}

			// 決定
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A)
			{
				PressedAMenu();
			}
			if (m_pad.wReleasedButtons & XINPUT_GAMEPAD_A)
			{
				ReleasedAMenu();
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

			// コピー
			if (m_key.m_bPressedKeyTbl['V'] && m_key.m_bPushKeyTbl[VK_LCONTROL])
			{
				SelectCopy();
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

		if (m_key.m_bPressedKeyTbl['V'])
		{
			PressedVKey();
		}

		// マウスポイントの更新
		m_beforeMouseClientPoint = m_key.m_MouseClientPoint;
		m_beforeWheelDelta = m_wheelDelta;
	}

	void InputManager::GameStart()
	{
		GameManager::Instance().SetGameModeAfterTransition(ENUM_GameMode::Play);
		auto startButton = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetSharedGameObject<UIBlinking>(L"StartButton");
		if (startButton) startButton->SetBlinkSpeed(5.0f);
	}

	void InputManager::StageStart()
	{
		GameManager::Instance().SetGameModeAfterTransition(ENUM_GameMode::Play);
	}

	void InputManager::ReturnTitle()
	{
		GameManager::Instance().SetGameModeAfterTransition(ENUM_GameMode::Title);
	}

	void InputManager::ChangeSelectGameStage()
	{
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			GameManager::Instance().ChangeSelectGameStage(-1);
		}
		else if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			GameManager::Instance().ChangeSelectGameStage(+1);
		}
	}

	void InputManager::Moves()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player->GetDeadFlag()) return;
		if (!player->GetBubbleAnimationEndFlag())return;
		if (player->GetMoveStopFlag())return;
		player->OnMoveInput();
	
		auto pos = player->GetComponent<Transform>()->GetPosition();
		auto forward = player->GetComponent<Transform>()->GetForward();
		auto right = player->GetComponent<Transform>()->GetRight();
		Vec3 effectPos = pos;
		float side = m_isRight ? 0.3f : -0.3f;
		effectPos += forward * 0.75f;
		effectPos += right * side;
		effectPos.y -= 0.68f;

		auto baseforward = Vec3(1.0f, 0.0f, 0.0f);
		Vec3 axis = baseforward;
		axis.cross(forward);
		axis.normalize();
		float dot = baseforward.dot(forward);
		dot = clamp(dot, -1.0f, 1.0f);
		float angle = acos(dot);
		Quat rot = Quat(axis, angle);

		player->GetComponent<Move>()
			->VectorMove(Vec3(m_pad.fThumbLX * m_MoveSpeed, 0.0f, m_pad.fThumbLY * m_MoveSpeed));


		if (!m_isEffectDraw)
		{
			m_timer = Timer(0.3f);
			m_timer.SetCounter();
			m_isEffectDraw = true;
		}

		if (m_isEffectDraw)
		{
			if (m_timer.TimeCount(App::GetApp()->GetElapsedTime(), false))
			{
				m_isRight = !m_isRight;
				EffectHandle effHandle;
				effHandle = EffectManager::Instance().PlayEffect(L"Smoke", Vec3(effectPos));
				EffectManager::Instance().SetScale(effHandle, Vec3(0.15f));
				EffectManager::Instance().SetRotationFromQuaternion(effHandle, rot);
				m_isEffectDraw = false;
			}
		}
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
		// 泡を吐く
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player->GetDeadFlag()) return;
		if (player->GetMoveStopFlag())return;
		// Rトリガーが押されたら、アニメーションを変更させる
		player->OnRTriggerInput();
		// 泡を生成する
		// player->CreateBubble();
	}

	void InputManager::PressedA()
	{
	}

	void InputManager::PressedB()
	{
		// GameManager::Instance().SetGameMode(ENUM_GameMode::Movie);
	}

	void InputManager::PressedStart()
	{
		SetInputEnabled(true);

		GameManager::Instance().SetGameMode(ENUM_GameMode::Menu);
	}

	void InputManager::PressedRButton()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		
		if (player->GetEatSoap())
		{
			player->CreateBubble();
			player->GetHaveBubble()->ApplyAblity(BubbleAbility::RideBubble);
		}
	}

	void InputManager::PressedLButton()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		
		if (player->GetEatSoap())
		{
			player->CreateBubble();
			player->GetHaveBubble()->ApplyAblity(BubbleAbility::TranpolineBubble);
		}
	}

	void InputManager::ReleasedLTrigger()
	{
		GetMyCamera()->SetIsAiming(false);
	}

	void InputManager::ReturnGame()
	{
		if (!m_isInputEnabled)return;

		switch (MenuManager::Instance().GetMenuMode())
		{
		case ENUM_MenuMode::MenuStart:
			GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
			break;

		case ENUM_MenuMode::Setting:
			MenuManager::Instance().SetMenuMode(ENUM_MenuMode::MenuStart);
			MenuManager::Instance().ChangeMenuMode();
			break;

		case ENUM_MenuMode::Howtoplay:
			MenuManager::Instance().SetMenuMode(ENUM_MenuMode::MenuStart);
			MenuManager::Instance().ChangeMenuMode();
			break;
		}
	}

	void InputManager::EnterSetting()
	{
		MenuManager::Instance().SetMenuMode(ENUM_MenuMode::Setting);
		MenuManager::Instance().ChangeMenuMode();
	}

	void InputManager::EnterHowtoplay()
	{
		MenuManager::Instance().SetMenuMode(ENUM_MenuMode::Howtoplay);
		MenuManager::Instance().ChangeMenuMode();
	}

	void InputManager::ReturnOneMenu()
	{
		
	}

	void InputManager::MoveMenuCursor()
	{
		if (!m_isInputEnabled)return;

		if (!(m_pad.wLastButtons & XINPUT_GAMEPAD_A))
		{
			switch (MenuManager::Instance().GetMenuMode())
			{
			case ENUM_MenuMode::Default:
				break;

			case ENUM_MenuMode::MenuStart:
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					MenuManager::Instance().ChangeSelectMenuMode(-1);
				}
				else if (m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					MenuManager::Instance().ChangeSelectMenuMode(+1);
				}
				break;

			case ENUM_MenuMode::Setting:
				break;

			case ENUM_MenuMode::Howtoplay:
				break;
			}
		}

	}

	void InputManager::PressedAMenu()
	{
		MenuManager::Instance().ChangeUISize(0.235f);
	}

	void InputManager::ReleasedAMenu()
	{
		switch (MenuManager::Instance().GetMenuUI())
		{
		case ENUM_MenuStart::Restart:
			MenuManager::Instance().ChangeUISize(0.25f);
			ReturnGame();
			break;

		case ENUM_MenuStart::Setting:
			MenuManager::Instance().ChangeUISize(0.25f);
			EnterSetting();
			break;

		case ENUM_MenuStart::Howtoplay:
			MenuManager::Instance().ChangeUISize(0.25f);
			EnterHowtoplay();
			break;

		case ENUM_MenuStart::Retitle:
			SetInputEnabled(false);
			MenuManager::Instance().ChangeUISize(0.25f);
			ReturnTitle();
			break;
		}

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

	void InputManager::SelectCopy()
	{
		StageEditor::Instance().SelectCopy();
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

	void InputManager::PressedVKey()
	{
		if (GameManager::Instance().GetGameMode() == ENUM_GameMode::Play)
		{
			GameManager::Instance().SetGameMode(ENUM_GameMode::PlayMovie);
		}
		else
		{
			GameManager::Instance().SetGameMode(ENUM_GameMode::Play);
		}
	}

	void InputManager::IdelAnimation()
	{
		// 押していたらだめ
		if (m_pad.bRightTrigger > RIGHT_TRIGGER_DEADZONE)return;

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player->GetDeadFlag()) return;
		if (!player->GetBubbleAnimationEndFlag()) return;
		wstring current = player->GetComponent<PNTBoneModelDraw>()->GetCurrentAnimation();
		auto currentAnimTime = player->GetComponent<PNTBoneModelDraw>()->GetCurrentAnimationTime();
		if (current == L"Bubble" && currentAnimTime >= 0.4f)
		{
			player->PlayerChangeAnimation(L"Idle", false);
		}
		if (current == L"Walk")
		{
			player->PlayerChangeAnimation(L"Idle", false);
		}
	}

	void InputManager::RTriggerRelse()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player->GetDeadFlag()) return;
		player->OnRTriggerRelese();
	}
}