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
		drawComp->SetMeshResource(L"M_Awapaka");
		drawComp->SetTextureResource(L"T_Awapaka");
		drawComp->SetDrawActive(true);

		// 当たり判定のコンポーネント
		auto obb = AddComponent<CollisionObb>();
		//obb->SetAfterCollision(AfterCollision::Auto);

		// 重力のコンポーネント
		m_gravity = AddComponent<Gravity>();

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"M_Awapaka");
		shadowComp->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PIDIV2, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		drawComp->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat);


		// バブルのコンポーネント
		//auto fbComp = AddComponent<FurBubble>(GetStage());
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		ReSpawn();
		Jump();
		PlayerDied();
		LaunchofBubble();
		DebugString();
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
				m_gravity->StartJump(m_VecJumpPower);
				m_isJumping = true;
			}
		}
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

		const float ZERO = 0.0f;

		if (m_BubblePowerCoolDown == true)
		{
			m_initCoolDown = 0.3f;
		}
		else
		{
			m_initCoolDown = 0.6f;
		}

		if (control[0].bRightTrigger && m_Bresing == false)
		{
			m_pBubble = stage->AddGameObject<Bubble>(GetThis<Player>(), Vec3(0.5f), 5.0f, m_Attack);
			m_pBubble->ShootBubble();
			m_Bresing = true;
			m_cooldown = m_initCoolDown;
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

	void Player::PlayerDied()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		if (scene == nullptr)
		{
			return;
		}

		auto stage = scene->GetActiveStage();
		if (stage == nullptr)
		{
			return;
		}

		if (m_isDead == true)
		{
			return;
		}

		// 死んだときの体力
		const float DIED_HP = 0.0f;

		// 死亡したらゲームオーバー画面にいかせる
		if (m_PlayerHP <= DIED_HP && m_isDead == false)
		{
			m_isDead = true;
			GameManager::Instance().SetGameMode(ENUM_GameMode::GameOver);
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
		GameManager::Instance().AddDebugStr(L"PlayerHP", m_PlayerHP);
		GameManager::Instance().AddDebugStr(L"Attack", m_Attack);
		GameManager::Instance().AddDebugStr(L"Cooldown", m_BubblePowerCoolDown);
	}

	void Player::ReSpawn()
	{
		// 落ちるときの位置
		float fallPosition = -20.0f;
		// リスポーンする位置
		float reSpawnPositionX = 0.0f;
		float reSpawnPositionY = 0.0f;
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
	// 入ったとき
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		InputManager* i = &InputManager::Instance();
		auto slowness = i->GetMoveSpeed() / 2;
		float slowness2 = 0.9f;

		// ダート（汚れ）
		if (Other->FindTag(L"Dirt"))
		{
			m_PlayerHP -= 1.0f;
			i->SetMoveSpeed(slowness2);
		}

		float Power = 6.0f;
		auto transPos = m_transform->GetPosition();
		// 床
		if (Other->FindTag(L"Ground"))
		{
			m_isJumping = false;
		}

		// バブル
		if (Other->FindTag(L"Bubble"))
		{
			if (Other->GetComponent<Transform>()->GetPosition().y < transPos.y)
			{
				m_isJumping = false;
			}
		}

		// トランポリンバブル
		if (Other->FindTag(L"TrampolineBubbles"))
		{
			if (Other->GetComponent<Transform>()->GetPosition().y < transPos.y)
			{
				m_gravity->StartJump(m_VecJumpPower);
			}
		}

		if (Other->FindTag(L"Soap"))
		{
			m_iseatSoap = true;
			m_pBubble->BubbleAddAblity(BubbleAbility::RideBubble);
			m_pBubble->BubbleAddAblity(BubbleAbility::TranpolineBubble);
		}

		if (Other->FindTag(L"Enemy"))
		{
			m_PlayerHP -= 1.0f;
		}
	}

	// 入っているとき
	void Player::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{
	}

	// 出たとき
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
		InputManager* i = &InputManager::Instance();
		// 1.0fだと半分のままなので、2倍を掛けてあげることによって通常の速度にする
		float normalSpeed = i->GetMoveSpeed() * 2.0f;
		float normalSpeed2 = 1.8f;

		if (Other->FindTag(L"Dirt"))
		{
			i->SetMoveSpeed(normalSpeed2);
		}
	}

	void Player::CreateBubble()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		m_pBubble = stage->AddGameObject<Bubble>(GetThis<Player>(), Vec3(0.5f), 5.0f, m_Attack);
		m_pBubble->ShootBubble();
	}
}
//end basecross

