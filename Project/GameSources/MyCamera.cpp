
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