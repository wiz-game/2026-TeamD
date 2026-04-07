/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// プレイヤーの初期設定
	void Player::OnCreate()
	{
		// トランスフォームコンポーネントを取得しておく
		m_transform = GetComponent<Transform>();
		m_transform->SetPosition(m_Position);

		auto moveComp = AddComponent<Move>();

		// ドローコンポーネントを追加
		m_draw = AddComponent<PNTDXModelDraw>();
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDrawActive(true);
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		// アプリケーションオブジェクトを取得
		auto& app = App::GetApp();

		// 前回からの経過時間：デルタタイムを取得する
		float delta = app->GetElapsedTime();
		Jump();
	}

	void Player::Jump()
	{
		auto device = App::GetApp()->GetInputDevice();
		auto control = device.GetControlerVec();
		auto transPos = m_transform->GetPosition();
		// 重力は常に下に向き続けている
		//m_Velocity -= App::GetApp()->GetElapsedTime();

		if (control[0].bConnected)
		{
			if (control[0].wPressedButtons & XINPUT_GAMEPAD_A)
			{
				if (m_isJumping == false)
				{
					m_Velocity = m_JumpPower;
					//m_Position.y += m_JumpPower * App::GetApp()->GetElapsedTime();
					m_isJumping = true;
				}
			}
		}

		transPos.y += m_Velocity * App::GetApp()->GetElapsedTime();

		// ジャンプしているとき
		if (m_isJumping)
		{
			m_Velocity -= m_Gravity * App::GetApp()->GetElapsedTime();
			float groundY = -0.1f;
			if (transPos.y <= groundY)
			{
				m_Velocity = 0.0f;
				transPos.y = 0.0f;
				m_isJumping = false;
			}
		}

		m_transform->SetPosition(transPos);
	}
}
//end basecross

