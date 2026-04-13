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

		m_move = AddComponent<Move>();

		// ドローコンポーネントを追加
		m_draw = AddComponent<PNTDXModelDraw>();
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetDrawActive(true);

		// 当たり判定
		auto obb = AddComponent<CollisionObb>();
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
			float groundY = 0.5f;

			// 足場の代わりに、地面(-0.1f)に着地したら
			if (transPos.y <= groundY)
			{
				m_Velocity = 0.0f;
				transPos.y = groundY;
				m_isJumping = false;
			}
		}

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

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (Other->FindTag(L"Bubble"))
		{
			m_Velocity = m_JumpPower * 1.5f;
			m_isJumping = true;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		//if (Other->FindTag(L"Bubble"))
		//{
		//	m_isJumping = true;
		//}
	}}
//end basecross

