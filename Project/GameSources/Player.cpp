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
		m_transform->SetRotation(m_Rotation);
		m_transform->SetScale(m_Scale);

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

		auto fbComp = GetComponent<FurBubble>();
		//auto fbRot = fbComp->GetRo
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
				m_isJumping = true;
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

		float initCoolDown = 1.0f,ZERO = 0.0f;

		if (control[0].bRightTrigger && m_Bresing == false)
		{
			stage->AddGameObject<Bubble>(GetThis<Player>());
			m_Bresing = true;
			m_cooldown = initCoolDown;
		}

		if (m_Bresing == true)
		{
			m_cooldown -= App::GetApp()->GetElapsedTime();
			if (m_cooldown <= ZERO)
			{
				m_Bresing = false;
			}
		}

	}

	// デバッグ用の文字列
	void Player::DebugString()
	{
		GameManager::Instance().AddDebugStr(L"CameraAngle", GetStickRY());
		GameManager::Instance().AddDebugStr(L"PlayerPosition.x", m_transform->GetPosition().x);
		GameManager::Instance().AddDebugStr(L"PlayerPosition.y", m_transform->GetPosition().y);
		GameManager::Instance().AddDebugStr(L"PlayerPosition.z", m_transform->GetPosition().z);
		GameManager::Instance().AddDebugStr(L"PlayerRotation.x", m_transform->GetRotation().x);
		GameManager::Instance().AddDebugStr(L"PlayerRotation.y", m_transform->GetRotation().y);
		GameManager::Instance().AddDebugStr(L"PlayerRotation.z", m_transform->GetRotation().z);
	}

	void Player::ReSpawn()
	{
		// 落ちるときの位置
		float fallPosition = 0.0f;
		// リスポーンする位置
		float reSpawnPositionX = .0f;
		float reSpawnPositionY = 61.2f;
		float reSpawnPositionZ = 0.0f;

		auto transPos = m_transform->GetPosition();
		if (transPos.y <= fallPosition)
		{
			transPos.x = reSpawnPositionX;
			transPos.y = reSpawnPositionY;
			transPos.z = reSpawnPositionZ;

			m_transform->SetPosition(transPos.x, transPos.y, transPos.z);
		}
	}

	// --- 当たり判定 ---
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		float Power = 6.0f;
		auto transPos = m_transform->GetPosition();
		// バブル
		if (Other->FindTag(L"Ground"))
		{
			m_isJumping = false;
			m_Velocity = 0.0f;
		}

		if (Other->FindTag(L"Bubble"))
		{
			if (Other->GetComponent<Transform>()->GetPosition().y < transPos.y)
			{
				m_isJumping = false;
				m_Velocity = 0.0f;
			}
		}

		if (Other->FindTag(L"TranmpolineBase"))
		{
			if (Other->GetComponent<Transform>()->GetPosition().y < transPos.y)
			{
				m_isJumping = true;
				m_Velocity = m_JumpPower * Power;
			}
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

