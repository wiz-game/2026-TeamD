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

		// 当たり判定のコンポーネント
		auto obb = AddComponent<CollisionObb>();

		// 重力のコンポーネント
		auto gravity = AddComponent<Gravity>();

		// バブルのコンポーネント
		auto fbComp = AddComponent<FurBubble>(GetThis<Player>());
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		//Jump();
		LaunchofBubble();
		Camera();
		DebugString();
		ReSpawn();
	}

	void Player::Jump()
	{
		// 自身の位置情報を取得する
		auto transPos = m_transform->GetPosition();
		auto device = App::GetApp()->GetInputDevice();
		// コントローラの情報を取得する
		auto control = device.GetControlerVec();

		// コントローラの1台目が接続されているとき
		if (control[0].bConnected)
		{
			// 1台目のコントローラのAボタンが押されたら または ジャンプしていなかったら
			if ((control[0].wPressedButtons & XINPUT_GAMEPAD_A) && m_isJumping == false)
			{
				m_Velocity = m_JumpPower;
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
		if (!stage)
		{
			return;
		}
		auto camera = stage->GetView()->GetTargetCamera();

		// 自分自身（プレイヤー）の座標を直接取得する
		auto targetPos = m_transform->GetPosition();

		if (control.wButtons & XINPUT_GAMEPAD_X && m_isTargetMode == false)
		{
			m_isTargetMode = true;
		}

		// 右スティックの傾きに応じて回り込ませる
		m_stickRX += control.fThumbRX * App::GetApp()->GetElapsedTime();
		m_stickRY += control.fThumbRY * App::GetApp()->GetElapsedTime();
		
		// カメラの傾きの上限
		const float MAXRY = 3.0f, MINRY = -0.5f;

		if (m_stickRY >= MAXRY)
		{
			m_stickRY = MAXRY;
		}
		else if (m_stickRY <= MINRY)
		{
			m_stickRY = MINRY;
		}

		if (m_isTargetMode == false)
		{
			// 傾き具合
			float slope = 5.5f;

			// カメラの位置の高さ
			float eyeY = 5.0f;

			// カメラの注視点(At)とカメラの位置(Eye)を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3(cosf(m_stickRX) * slope, m_stickRY * 2.0f, sinf(m_stickRX) * slope);
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
			float cameraMoveSpeed = 2.0f;

			// カメラの注視点(At)とカメラの位置(Eye)を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f) * 0.25f;
			Vec3 eye = targetPos + Vec3
			(
				cosf(m_stickRX) * cameraMoveSpeed * 0.25f,
				m_stickRY,
				sinf(m_stickRX) * cameraMoveSpeed * 0.25f
			);

			Vec3 forward = at - eye;

			float angle = atan2f(forward.x, forward.z);
			m_transform->SetRotation(0.0f, angle, 0.0f);

			at += Vec3(forward.z, 0.0f, -forward.x) * 0.25;
			eye += Vec3(forward.z, 0.0f, -forward.x) * 0.25;

			// カメラに設定を反映
			camera->SetAt(at);
			camera->SetEye(eye);
		}
	}

	// デバッグ用の文字列
	void Player::DebugString()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss;
		wss << L"CameraAngle：" << GetStickRY();
		scene->SetDebugString(wss.str());
	}

	void Player::ReSpawn()
	{
		float fallPosition = -20.0f;
		if (m_Position.y >= fallPosition)
		{
			m_Position = Vec3(0.0f, 80.0f, 0.0f);
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

