/*!
@file Scene.cpp
@brief シーン実装
*/

#include "stdafx.h"
#include "Project.h"
#include "DXModelResource.h"
#include "JoltManager.h"
#include "Scene.h"

namespace basecross
{
	void Scene::OnCreate()
	{
		try 
		{
			// フルスクリーンモードにする
			App::GetApp()->SetFullScreenMode();

			// 各マネージャーを初期化
			JoltManager::StaticInitialize();
			GameManager::Instance().Initialize(true);
			InputManager::Instance().Initialize();
			StageEditor::Instance().Initialize();
			EffectManager::Instance().CreateEfkInterface(20000);
			MovieManager::Instance().Initialize();

			// 背景色を設定
			SetClearColor(Col4(0.0f, 0.3f, 0.6f, 1.0f));
			
			// リソース作成
			LoadMedia loadMedia;
			loadMedia.RegisterMediaFiles(App::GetApp()->GetDataDirWString());
		}
		catch (...) 
		{
			throw;
		}
	}

	void Scene::OnCreate2()
	{
		GameManager::Instance().SetGameMode(ENUM_GameMode::Play);// Title);
	}

	void Scene::OnUpdate()
	{
		// InputManagerを先に更新する
		InputManager::Instance().Update();

		SceneBase::OnUpdate();

		MovieManager::Instance().OnUpdate();
	}


	Scene::~Scene()
	{
		JoltManager::StaticTerminate();
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		GameManager::Instance().RemoveDebugLog();
		EffectManager::Instance().StopAllEffect();

		// タイトルステージ
		if (event->m_MsgStr == L"TitleStage")
		{
			ResetActiveStage<TitleStage>();
		}
		// ステージセレクトステージ
		else if (event->m_MsgStr == L"SelectStage")
		{
			ResetActiveStage<SelectStage>();
		}
		// ゲームクリアステージ
		else if (event->m_MsgStr == L"GameClearStage")
		{
			ResetActiveStage<GameClearStage>();
		}
		// ゲームオーバーステージ
		else if (event->m_MsgStr == L"GameOverStage")
		{
			ResetActiveStage<GameOverStage>();
		}
		// ゲームステージ1
		else if (event->m_MsgStr == L"GameStage_1") 
		{
			ResetActiveStage<GameStage>(L"GameStage_1");
		}
		// ゲームステージ2
		else if (event->m_MsgStr == L"GameStage_2") 
		{
			ResetActiveStage<GameStage>(L"GameStage_2");
		}
		// ゲームステージ3
		else if (event->m_MsgStr == L"GameStage_3") 
		{
			ResetActiveStage<GameStage>(L"GameStage_3");
		}
		// ゲームステージ4
		else if (event->m_MsgStr == L"GameStage_4") 
		{
			ResetActiveStage<GameStage>(L"GameStage_4");
		}
		// ゲームステージ5
		else if (event->m_MsgStr == L"GameStage_5") 
		{
			ResetActiveStage<GameStage>(L"GameStage_5");
		}

		// 現在のステージ名を更新
		m_nowStageName = event->m_MsgStr;
	}
}