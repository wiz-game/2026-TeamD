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
		auto fbComp = AddComponent<FurBubble>(GetStage());
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		ReSpawn();
		Jump();
		LaunchofBubble();
		//Camera();
		//DebugString();
		//TargetCamera();
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

	// デバッグ用の文字列
	void Player::DebugString()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		wstringstream wss;
		wss << L"CameraAngle：" << GetStickRY() << endl;

		auto transPos = m_transform->GetPosition();
		wss << L"PlayerPosition.y：" << transPos.y;
		scene->SetDebugString(wss.str());
	}

	void Player::ReSpawn()
	{
		// 落ちるときの位置
		float fallPosition = 0.0f;
		// リスポーンする位置
		float reSpawnPosition = 80.0f;

		auto transPos = m_transform->GetPosition();
		if (transPos.y <= fallPosition)
		{
			transPos.y = reSpawnPosition;
			m_transform->SetPosition(0.0f, transPos.y, 0.0f);
		}
	}

	// --- 当たり判定 ---
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		auto transPos = m_transform->GetPosition();
		// バブル
		if (Other->FindTag(L"Bubble") || Other->FindTag(L"Ground"))
		{
			m_isJumping = false;
			m_Velocity = 0.0f;
		}
	}

	void Player::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{

	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}
}
//end basecross

