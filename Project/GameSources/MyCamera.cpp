
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

	void MyCamera::PlayModeMove()
	{
		// InputManagerからの入力を受け取り、カメラを操作する処理
		// InputManagerのMoveCamera()から呼び出される想定
		// 引数は必要に応じ追加してください
		// Lトリガー（現在でいうXボタン）を押している時の処理はメンバ変数のm_isAimingで切り替えるようにしてください
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
	}
	
	void MyCamera::UpdateMenuMode()
	{
	}

	void MyCamera::UpdateEditorMode()
	{
	}
}