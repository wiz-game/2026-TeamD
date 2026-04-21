#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void InputManager::Initialize()
	{
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
			//if (m_pad.fThumbRX > STACK_DEADZONE_R || m_pad.fThumbRX < -STACK_DEADZONE_R ||
			//	m_pad.fThumbRY > STACK_DEADZONE_R || m_pad.fThumbRY < -STACK_DEADZONE_R)
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
			if ((m_key.m_MouseClientPoint.x > 0 || m_key.m_MouseClientPoint.y > 0 &&
				m_key.m_bPressedKeyTbl[VK_TAB] && m_key.m_bPressedKeyTbl[VK_LBUTTON]))
			{
				FocusFixedViewPointMove();
			}

			// カメラ位置固定視点移動
			if ((m_key.m_MouseClientPoint.x > 0 || m_key.m_MouseClientPoint.y > 0) &&
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
			if (m_key.m_bPressedKeyTbl[VK_LBUTTON])
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
	}

	void InputManager::Moves()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage) return;

		stage->GetSharedGameObject<Player>(L"Player")->GetComponent<Move>()
			->VectorMove(Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY));
	}

	void InputManager::MoveCamera()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage)
		{
			return;
		}

		auto camera = stage->GetView()->GetTargetCamera();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (!player)
		{
			return;
		}

		auto playerComp = player->GetComponent<Transform>();
		auto targetPos = playerComp->GetPosition();
		auto targetMode = player->GetTargetMode();

		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			targetMode = !targetMode;
			player->SetTargetMode(targetMode);
		}

		static float stickRX = 0.0f;
		static float stickRY = 0.0f;
		stickRX	+= m_pad.fThumbRX * App::GetApp()->GetElapsedTime();
		stickRY += m_pad.fThumbRY * App::GetApp()->GetElapsedTime();

		// カメラの傾きの上限
		const float MAX_RY = 3.0f, MIN_RY = -0.5f;

		if (stickRY >= MAX_RY)
		{
			stickRY = MAX_RY;
		}
		else if (stickRY <= MIN_RY)
		{
			stickRY = MIN_RY;
		}

		if (targetMode == false)
		{
			// 傾き具合
			float slope = 5.5f;

			float stickLX = m_pad.fThumbLX;
			float stickLY = m_pad.fThumbLY;

			// カメラの注視点（At）とカメラの位置（Eye）を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3(cosf(stickRX) * slope, stickRY * 2.0f, sinf(stickRX) * slope);
			Vec3 forward = at - eye;

			float vectorx = 0.0f,vectorz = 0.0f;
			
			Vec3 forwardMove = Vec3(forward.z, 0.0f, -forward.x);

			// 前方移動
			vectorx += forward.x * stickLY;
			vectorz += forward.z * stickLY;

			// 左右移動
			vectorx += forwardMove.x * stickLX;
			vectorz += forwardMove.z * stickLX;

			// スティック入力がある場合、カメラではなくプレイヤー側を回転させる
			if (fabsf(stickLX) > 0.1f || fabsf(stickLY) > 0.1f)
			{
				float angle = atan2f(vectorx, vectorz);
				playerComp->SetRotation(0.0f, angle, 0.0f);
			}

			// カメラに設定を反映
			camera->SetAt(at);
			camera->SetEye(eye);
		}
		else if (targetMode == true)
		{
			float cameraMoveSpeed = 2.0f;

			// カメラの注視点（At）とカメラの位置（Eye）を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3
			(
				cosf(stickRX) * cameraMoveSpeed,
				stickRY,
				sinf(stickRX) * cameraMoveSpeed
			);
			Vec3 forward = at - eye;

			// ターゲット時はカメラの向きに合わせてプレイヤーを回転させる
			float angle = atan2f(forward.x, forward.z);
			playerComp->SetRotation(0.0f, angle, 0.0f);

			at += Vec3(forward.z, 0.0f, -forward.x) * 0.25f;
			eye += Vec3(forward.z, 0.0f, -forward.x) * 0.25f;

			// カメラに設定を反映
			camera->SetAt(at);
			camera->SetEye(eye);
		}
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
	}
	
	void InputManager::CameraFixedViewPointMove()
	{
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