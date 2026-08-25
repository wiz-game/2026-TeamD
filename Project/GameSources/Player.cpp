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
		m_pntDraw = AddComponent<PNTBoneModelDraw>();
		m_pntDraw->SetMeshResource(L"AwaPaka");
		m_pntDraw->SetTextureResource(L"T_AwaPaka_Body");
		m_pntDraw->SetDrawActive(true);
		m_pntDraw->SetOwnShadowActive(true);

		// 当たり判定のコンポーネント
		auto obb = AddComponent<CollisionObb>();
		//obb->SetDrawActive(true);
		//obb->SetAfterCollision(AfterCollision::Auto);

		// 重力のコンポーネント
		// m_gravity = AddComponent<Gravity>();

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"AwaPaka");
		shadowComp->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.1f, 0.075f, 0.075f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		m_pntDraw->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat) ;

		auto loopFlag = true;
		m_pntDraw->AddAnimation(L"Idle",  0,  65, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Walk", 70, 80, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Bubble", 155, 30, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"GameOver", 190, 60, !loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"GameClear", 255, 45, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Eat", 305, 50, !loopFlag, 30.0f);
		m_pntDraw->AddAnimation(L"ALL", 0, 355, loopFlag, 60.0f);

		m_pntDraw->ChangeCurrentAnimation(L"Idle");

		// バブルのコンポーネント
		//auto fbComp = AddComponent<FurBubble>(GetStage());
	}

	// プレイヤーの更新処理
	void Player::OnUpdate()
	{
		ReSpawn();
		// Jump();
		PlayerDied();
		LaunchofBubble();
		DebugString();
		PlayerAnimation();

		switch (m_playerState)
		{
		case PlayerState::Default:
			break;
		case PlayerState::PowerUp :
			EffectManager::Instance().SetPosition(m_effectHandle, GetComponent<Transform>()->GetPosition());			if (m_timer.TimeCount(App::GetApp()->GetElapsedTime(), false))
			{
				SetPlayerState(PlayerState::Default);
			}
			break;
		case PlayerState::Dead:
			break;
		default:
			break;
		}

		// 現在のアニメと終了判定を毎フレーム取得
		wstring current = m_pntDraw->GetCurrentAnimation();
		bool isEnd = m_pntDraw->IsTargetAnimeEnd();

		// Bubble の終了検出
		if (current == L"Bubble" && isEnd)
		{
			m_isBubbleAnimationEnd = true;
		}

		auto draw = GetComponent<PNTBoneModelDraw>();
		auto animTime = draw->GetCurrentAnimationTime();
		if (!GetBresing() && current == L"Bubble" && animTime >= 0.4f)
		{
			CreateBubble();
		}
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
			SetPlayerState(PlayerState::Dead);
		}
	}

	// デバッグ用の文字列
	void Player::DebugString()
	{
		//GameManager::Instance().AddDebugStr(L"CameraAngle", GetStickRY());
		//GameManager::Instance().AddDebugStr(L"PlayerPosition.x", m_transform->GetPosition().x);
		//GameManager::Instance().AddDebugStr(L"PlayerPosition.y", m_transform->GetPosition().y);
		//GameManager::Instance().AddDebugStr(L"PlayerPosition.z", m_transform->GetPosition().z);
		//GameManager::Instance().AddDebugStr(L"PlayerRotation.x", m_transform->GetRotation().x);
		//GameManager::Instance().AddDebugStr(L"PlayerRotation.y", m_transform->GetRotation().y);
		//GameManager::Instance().AddDebugStr(L"PlayerRotation.z", m_transform->GetRotation().z);
		//GameManager::Instance().AddDebugStr(L"PlayerHP", m_PlayerHP);
		//GameManager::Instance().AddDebugStr(L"Attack", m_Attack);
		//GameManager::Instance().AddDebugStr(L"Cooldown", m_BubblePowerCoolDown);
		//GameManager::Instance().AddDebugStr(L"PlayerPowerUp", m_isPlayerPowerUp);
		//GameManager::Instance().AddDebugStr(L"PowerUpTimer", m_timer.GetCounter());
		//GameManager::Instance().AddDebugStr(L"PlayerPowerUp", m_isPlayerPowerUp);
		// GameManager::Instance().AddDebugStr(L"CurrentAnimation", m_draw->GetCurrentAnimation());
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
			//m_pBubble->BubbleAddAblity(BubbleAbility::RideBubble);
			//m_pBubble->BubbleAddAblity(BubbleAbility::TranpolineBubble);
			SoundManager::Instance().PlaySE(L"GetItem_SE");

			SetPlayerState(PlayerState::PowerUp);
		}

		if (Other->FindTag(L"Enemy"))
		{
			m_PlayerHP -= 10.0f;
		}
	}

	// 入っているとき
	void Player::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{
	}

	// 出たとき
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other)
	{
	}

	void Player::CreateBubble()
	{
		if (GetMoveStopFlag()) return;

		OnRTriggerInput();
		BubbleEffect();

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage(); 
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		auto attack = GetAttack();
		auto playerPowerUp = GetPlayerPowerUp();
		auto bubble = GetHaveBubble();
		auto initCoolDown = 0.3f;
		bubble = stage->AddGameObject<Bubble>(player, Vec3(0.5f), 5.0f, attack, playerPowerUp);
		bubble->ShootBubble();
		SetBresing(true);
		SetCoolDown(initCoolDown);
		SoundManager::Instance().PlaySE(L"Bubble_SE");
	}

	void Player::SetPlayerState(PlayerState state)
	{
		ExitPlayerState(m_playerState);
		m_playerState = state;
		EnterPlayerState(m_playerState);
	}

	void Player::EnterPlayerState(PlayerState state)
	{
		switch (state)
		{
		case PlayerState::Default:
			m_isPlayerPowerUp = false;

			SoundManager::Instance().PlayBGM(L"GameStage_BGM");
			break;
		case PlayerState::PowerUp:
			m_timer = Timer(12.0f);
			m_timer.SetCounter();
			SoundManager::Instance().PlayBGM(L"Invincible_BGM");
			m_effectHandle = EffectManager::Instance().PlayEffect(L"PowerUp", GetComponent<Transform>()->GetPosition());
			EffectManager::Instance().SetScale(m_effectHandle, Vec3(0.7f));
			m_isPlayerPowerUp = true;
			break;
		case PlayerState::Dead:
			GameManager::Instance().SetGameMode(ENUM_GameMode::GameOverMovie);
			break;
		default:
			break;
		}
	}

	void Player::ExitPlayerState(PlayerState state)
	{
		switch (state)
		{
		case PlayerState::Default:
			SoundManager::Instance().StopBGM();
			break;
		case PlayerState::PowerUp:
			SoundManager::Instance().StopBGM();
			EffectManager::Instance().StopEffect(m_effectHandle);
			break;
		case PlayerState::Dead:
			break;
		default:
			break;
		}
	}

	void Player::PlayerAnimation()
	{
		m_pntDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
	}
	
	void Player::PlayerChangeAnimation(const wstring& newAnim, bool forceRestart)
	{
		if (newAnim.empty())return;
				
		wstring current = m_pntDraw->GetCurrentAnimation();

		if (!forceRestart && nowAnimation == newAnim) return;

		m_pntDraw->ChangeCurrentAnimation(newAnim.c_str());

		nowAnimation = newAnim;
	}

	void Player::OnMoveInput()
	{
		if (GetDeadFlag()) return;
		if (GetMoveStopFlag()) return;

		wstring current = m_pntDraw->GetCurrentAnimation();
		bool isEnd = m_pntDraw->IsTargetAnimeEnd();

		if (m_isBubbleAnimationEnd)
		{
			if (current != L"Walk")
			{
				PlayerChangeAnimation(L"Walk");
			}
		}
	}

	void Player::OnRTriggerInput()
	{
		if (GetDeadFlag()) return;
	
		 m_isBubbleAnimationEnd = false;
		 PlayerChangeAnimation(L"Bubble");
	}

	void Player::OnRTriggerRelese()
	{
		wstring current = m_pntDraw->GetCurrentAnimation();

		if (current == L"Bubble")
		{
			m_isBubbleAnimationEnd = true;
		}
	}

	void Player::PlayGameAnimation()
	{
		if (m_isStartStop)
		{
			PlayerChangeAnimation(L"Idle");
			m_isStartStop = false;
		}
	}

	void Player::BubbleEffect()
	{
		// エフェクトの回転
		auto pos = GetComponent<Transform>()->GetPosition();
		auto forward = GetComponent<Transform>()->GetForward();
		forward.normalize();
		auto baseforward = Vec3(1.0f, 0.0f, 0.0f);
		Vec3 axis = baseforward;
		axis.cross(forward);
		axis.normalize();
		float dot = baseforward.dot(forward);
		dot = clamp(dot, -1.0f, 1.0f);
		float angle = acos(dot);
		Quat rot = Quat(axis, angle);
		Quat offset(Vec3(0, 1, 0), XMConvertToRadians(13.0f));
		Quat finalRot = offset * rot;

		EffectHandle effHandle;
		if (m_isPlayerPowerUp)
		{
			effHandle = EffectManager::Instance().PlayEffect(L"MightBubble_2", pos + forward);
			EffectManager::Instance().SetScale(effHandle, Vec3(0.4f));
			EffectManager::Instance().SetRotationFromQuaternion(effHandle, finalRot);
		}
		else
		{
			effHandle = EffectManager::Instance().PlayEffect(L"Bubble", pos + forward);
			EffectManager::Instance().SetScale(effHandle, Vec3(0.3f));
			EffectManager::Instance().SetRotationFromQuaternion(effHandle, finalRot);
		}
	}
}
//end basecross

