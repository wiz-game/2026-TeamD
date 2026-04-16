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
		AddTag(L"Player");
		// トランスフォームコンポーネントを取得しておく
		m_transform = GetComponent<Transform>();
		m_transform->SetPosition(m_Position);
		m_transform->SetScale(0.3, 0.3, 0.3);

		m_move = AddComponent<Move>();

		// ドローコンポーネントを追加
		m_draw = AddComponent<PNTDXModelDraw>();
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"M_Alpaca");
		drawComp->SetTextureResource(L"T_Alpaca");
		drawComp->SetDrawActive(true);

		// 当たり判定
		auto obb = AddComponent<CollisionObb>();

		auto gravity = AddComponent<Gravity>();
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		Jump();
		LaunchofBubble();
		Camera();
	}

	void Player::Jump()
	{
		auto transPos = m_transform->GetPosition();
		auto device = App::GetApp()->GetInputDevice();
		auto control = device.GetControlerVec();

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

		m_transform->SetPosition(transPos);
	}

	void Player::LaunchofBubble()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto device = App::GetApp()->GetInputDevice();
		auto control = device.GetControlerVec();

		if (!stage)
		{
			return;
		}

		if (control[0].wPressedButtons & XINPUT_GAMEPAD_B)
		{
			stage->AddGameObject<Bubble>(GetThis<Player>());
		}
	}

	void Player::Camera()
	{
		// ゲームパッドオブジェクトを取得
		auto& control = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		// カメラオブジェクトの取得
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto camera = stage->GetView()->GetTargetCamera();

		// 自分自身（プレイヤー）の座標を直接取得する
		auto targetPos = m_transform->GetPosition();

		if (control.wPressedButtons & XINPUT_GAMEPAD_X)
		{
			m_isTargetMode = !m_isTargetMode;
		}

		if (m_isTargetMode == false)
		{
			// 傾き具合
			float slope = 5.5f;

			// カメラの位置の高さ
			float eyeY = 5.0f;

			// 右スティックの傾きに応じて回り込ませる
			m_angleY += control.fThumbRX * App::GetApp()->GetElapsedTime();

			// カメラの注視点(At)とカメラの位置(Eye)を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3(cosf(m_angleY) * slope, eyeY, sinf(m_angleY) * slope);
			Vec3 forward = at - eye;

			float vectorx = 0.0f;
			float vectorz = 0.0f;

			float vectorarrayx;
			float vectorarrayz;

			// スティックの情報を取得する
			float stickLX = control.fThumbLX;
			float stickLY = control.fThumbLY;

			// 向いている方向に行くようにする
			Vec3 forwardMove = Vec3(forward.z, 0.0f, -forward.x);

			// 前後移動
			vectorx += forward.x * stickLY;
			vectorz += forward.z * stickLY;

			// 左右移動
			vectorx += forwardMove.x * stickLX;
			vectorz += forwardMove.z * stickLX;

			vectorarrayx = vectorx;
			vectorarrayz = vectorz;
			
			if (fabsf(stickLX) > 0.1f || fabsf(stickLY) > 0.1f)
			{
				float angle = atan2f(vectorarrayx, vectorarrayz);
				m_transform->SetRotation(0.0f, angle, 0.0f);
			}
			// カメラに設定を反映
			camera->SetAt(at);
			camera->SetEye(eye);
		}
		else if(m_isTargetMode == true)
		{
			// 右スティックの傾きに応じて回り込ませる
			m_angleY += control.fThumbRX * App::GetApp()->GetElapsedTime();

			// カメラの注視点(At)とカメラの位置(Eye)を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3(cosf(m_angleY) * 2.0f, 2.0f, sinf(m_angleY) * 2.0f);

			Vec3 forward = at - eye;

			float angle = atan2f(forward.x, forward.z);
			m_transform->SetRotation(0.0f, angle, 0.0f);

			// カメラに設定を反映
			camera->SetAt(at);
			camera->SetEye(eye);
		}
	}


	// --- 当たり判定 ---
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		auto transPos = m_transform->GetPosition();
		// バブル
		if (Other->FindTag(L"Bubble"))
		{
			m_Velocity = m_JumpPower * 1.5f;
			m_isJumping = true;
		}

		// 床
		if (Other->FindTag(L"Ground"))
		{
			m_isJumping = false;
			m_Velocity = 0.0f;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}
}
//end basecross

