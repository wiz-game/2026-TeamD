
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MyCamera::MyCamera()
	{
	}

	MyCamera::~MyCamera()
	{
	}

	void MyCamera::OnCreate()
	{
		Camera::OnCreate();
	}
	
	void MyCamera::OnUpdate()
	{
		switch (GameManager::Instance().GetGameMode())
		{
		default:
			break;
		case ENUM_GameMode::Play:
			UpdatePlayMode();
			break;
		case ENUM_GameMode::Menu:
			UpdateMenuMode();
			break;
		case ENUM_GameMode::Editor:
			UpdateEditorMode();
			break;
		}
		
		// CameraクラスのUpdateを最後に呼ぶ
		Camera::OnUpdate();
	}

	void MyCamera::PlayModeMove
	(
		float stickrx,
		float stickry,
		float sticklx,
		float stickly
	)
	{
		// InputManagerからの入力を受け取り、カメラを操作する処理
		// InputManagerのMoveCamera()から呼び出される想定
		// 引数は必要に応じ追加してください
		// Lトリガー(LT)（現在でいうXボタン）を押している時の処理はメンバ変数のm_isAimingで切り替えるようにしてください

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!stage)
		{
			return;
		}

		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (!player)
		{
			return;
		}

		auto playerComp = player->GetComponent<Transform>();
		auto targetPos = playerComp->GetPosition();
		auto targetMode = player->GetTargetMode();

		static float stickRX = 3.25f;
		static float stickRY = 1.0f;
		static float stickLX = 0.0f;
		static float stickLY = 0.0f;
		float cameraMoveSpeed = 2.0f;
		stickRX -= stickrx * App::GetApp()->GetElapsedTime() * cameraMoveSpeed;
		stickRY -= stickry * App::GetApp()->GetElapsedTime() * cameraMoveSpeed;
		stickLX = sticklx * App::GetApp()->GetElapsedTime() * cameraMoveSpeed;
		stickLY = stickly * App::GetApp()->GetElapsedTime() * cameraMoveSpeed;

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

		// エイムモードが有効なとき
		if (m_isAiming == true)
		{
			float cameraDistance = 2.0f;

			// カメラの注視点（At）とカメラの位置（Eye）を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3
			(
				cosf(stickRX) * cameraDistance,
				stickRY,
				sinf(stickRX) * cameraDistance
			);
			Vec3 forward = at - eye;
			// ターゲット時はカメラの向きに合わせてプレイヤーを回転させる
			float angle = atan2f(forward.x, forward.z);
			playerComp->SetRotation(0.0f, angle, 0.0f);

			at += Vec3(forward.z, 0.0f, -forward.x) * 0.25f;
			eye += Vec3(forward.z, 0.0f, -forward.x) * 0.25f;

			// カメラに設定を反映
			this->SetAt(at);
			this->SetEye(eye);
		}
		else if (m_isAiming == false)
		{
			// 傾き具合
			float slope = 5.5f;

			// カメラの注視点（At）とカメラの位置（Eye）を計算
			Vec3 at = targetPos + Vec3(0.0f, 1.0f, 0.0f);
			Vec3 eye = targetPos + Vec3
			(
				cosf(stickRX) * slope,
				stickRY * 2.0f,
				sinf(stickRX) * slope
			);
			Vec3 forward = at - eye;

			float vectorx = 0.0f, vectorz = 0.0f;

			Vec3 forwardMove = Vec3(forward.z, 0.0f, -forward.x);

			// 前方移動
			vectorx += forward.x * stickLY;
			vectorz += forward.z * stickLY;

			// 左右移動
			vectorx += forwardMove.x * stickLX;
			vectorz += forwardMove.z * stickLX;

			//スティック入力がある場合、カメラではなくプレイヤー側を回転させる
			if (fabsf(sticklx) > 0.1f || fabsf(stickly) > 0.1f)
			{
				float angle = atan2f(vectorx, vectorz);
				playerComp->SetRotation(0.0f, angle, 0.0f);
			}

			// カメラに設定を反映
			this->SetAt(at);
			this->SetEye(eye);
		}
	}

	void MyCamera::FocusFixedViewPointMove(const Point2D<int> mousePoint)
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		m_yaw += mousePoint.x * elapsedTime;
		m_pitch += mousePoint.y * elapsedTime;
		
		float radius = 5.5f;
		float cosP = cosf(m_pitch);

		auto offset = Vec3(radius * cosP * cosf(m_yaw), radius * sinf(m_pitch), radius * cosP * sinf(m_yaw));
		auto eye = GetAt() + offset;

		SetEye(eye);
	}

	void MyCamera::CameraFixedViewPointMove(const Point2D<int> mousePoint)
	{

	}
	
	void MyCamera::UpdatePlayMode()
	{
		// プレイヤーに追従する処理
		PlayModeMove(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	void MyCamera::UpdateMenuMode()
	{
	}

	void MyCamera::UpdateEditorMode()
	{
	}
}