/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	GameStage::GameStage(const wstring& stageNum)
		: Stage(),
		m_isGameClear(false)
	{
		m_stageNum = to_string(stageNum);
	}


	void GameStage::OnCreate()
	{
		try 
		{
			CreateViewLight();
			CreatePlayer();
			CreateMenu();
			CreateGameUI();

			// カウントをリセット
			GameManager::Instance().ResetDirtNum();
			// ステージの作成
			StageEditor::Instance().ReadStageData(m_stageNum + ".bin", GetThis<GameStage>());
			uiAwasSlide = AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 900.0f);
			AddGameObject<EffectUpdateDrawManager>();
			MovieManager::Instance().SetUISlide(uiAwasSlide);
			MovieManager::Instance().SetStage(GetThis<GameStage>());
			GameManager::Instance().SetGameMode(ENUM_GameMode::PlayMovie);
		}
		catch (...)
		{
			throw;
		}
	}

	void GameStage::OnUpdate()
	{
		SetCollRange();

		if (GameManager::Instance().GetGameMode() == ENUM_GameMode::Editor)
		{
			StageEditor::Instance().AddEditorMenuLog(L"FPS", 1.0f / App::GetApp()->GetElapsedTime());
		}
		GameManager::Instance().AddDebugStr(L"FPS", 1.0f / App::GetApp()->GetElapsedTime());
		
		if (GameManager::Instance().GetDirtNum() <= 0)
		{	
			if (!m_isGameClear)
			{
				GameManager::Instance().SetGameMode(ENUM_GameMode::GameClearMovie);
				m_isGameClear = true;
			}
		}

		// メニュー画面のボタンの切り替え
		MenuManager::Instance().ChangeUIParam();
	}

	void GameStage::CreateViewLight() 
	{
		// カメラの設定
		auto camera = ObjectFactory::Create<MyCamera>();
		camera->SetEye(Vec3(0.0f, 0.0f, -8.0f));
		camera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		// ビューにカメラを設定
		auto view = CreateView<SingleView>();
		view->SetCamera(camera);

		// マルチライトの作成
		auto light = CreateLight<MultiLight>();
		light->SetDefaultLighting(); // デフォルトのライティングを指定

		MovieManager::Instance().SetCamera(camera);

		// スカイボックスの作成
		AddGameObject<SkyBox>();
	}

	// Playerを作成する
	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(0.0f, 0.6f, 0.0f));
		SetSharedGameObject(L"Player", player);
		MovieManager::Instance().SetPlayer(player);
	}

	// Menuを作成する
	void GameStage::CreateMenu()
	{
		// メニューのフレーム
		m_uiframes.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionFade",
			Vec3(0.0f, 0.0f, 0.0f), 
			1.0f,
			Col4(1.0f))));
		m_uiframes.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionFrame",//OptionFrame",
			Vec3(0.0f, 0.0f, 0.0f), 
			0.75f,
			Col4(1.0f))));

		MenuManager::Instance().SetUIFrames(m_uiframes);
		MenuManager::Instance().UIDrawActive(false, m_uiframes);


		// メニューのUI
		m_uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_1",
			Vec3(0.0f, 150.0f, 0.0f),
			m_selectScale,
			m_uiDiffuse)));
		m_uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_2",
			Vec3(0.0f, 25.0f, 0.0f),
			m_selectScale,
			m_uiDiffuse)));
		m_uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_3",
			Vec3(0.0f, -100.0f, 0.0f),
			m_selectScale,
			m_uiDiffuse)));
		m_uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_4",
			Vec3(0.0f, -225.0f, 0.0f),
			m_selectScale,
			m_uiDiffuse)));

		MenuManager::Instance().SetUIDefaults(m_uidefaults);
		MenuManager::Instance().UIDrawActive(false, m_uidefaults);

		// オプション画面のUI
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_BGMLogo",
			Vec3(0.0f, 100.0f, 0.0f),
			0.3f,
			Col4(1.0f))));
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"Awa",
			Vec3(0.0f, 100.0f, 0.0f),
			0.05f,
			Col4(1.0f))));
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_SELogo",
			Vec3(0.0f, -50.0f, 0.0f),
			0.3f,
			Col4(1.0f))));
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"Awa",
			Vec3(0.0f, -50.0f, 0.0f),
			0.05f,
			Col4(1.0f))));
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_SoundButton_1",
			Vec3(-150.0f, -220.0f, 0.0f),
			0.25f,
			Col4(1.0f))));
		m_uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_SoundButton_2",
			Vec3(150.0f, -220.0f, 0.0f),
			0.25f,
			Col4(1.0f))));

		MenuManager::Instance().SetUISettings(m_uisettings);
		MenuManager::Instance().UIDrawActive(false, m_uisettings);


		// あそびかた画面のUI
		m_uihowtoplays.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionHowtoplay",
			Vec3(0.0f, 0.0f, 0.0f),
			0.50f,
			Col4(1.0f))));

		MenuManager::Instance().SetUIHowtoplays(m_uihowtoplays);
		MenuManager::Instance().UIDrawActive(false, m_uihowtoplays);
	}

	void GameStage::CreateGameUI()
	{
		auto uistatue0 = AddGameObject<UIStatueNum>(STRUCT_UIParam(
			L"UI_AwaPaka3",
			Vec3(600.0f, 320.0f, 0.0f),
			0.12f,
			Col4(1.0f)));
		uistatue0->SetStatueNum(0);
		m_uistatues.push_back(uistatue0);

		auto uistatue1 = AddGameObject<UIStatueNum>(STRUCT_UIParam(
			L"UI_AwaPaka3",
			Vec3(550.0f, 320.0f, 0.0f),
			0.12f,
			Col4(1.0f)));
		uistatue1->SetStatueNum(1);
		m_uistatues.push_back(uistatue1);

		auto uistatue2 = AddGameObject<UIStatueNum>(STRUCT_UIParam(
			L"UI_AwaPaka3",
			Vec3(500.0f, 320.0f, 0.0f),
			0.12f,
			Col4(1.0f)));
		uistatue2->SetStatueNum(2);
		m_uistatues.push_back(uistatue2);

	}

	void GameStage::SetCollRange()
	{
		// プレイヤーの位置を中心に当たり判定の範囲を設定する
		m_collManagerPos = GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		AABB collRange = AABB(m_collManagerPos - m_collisionRange, m_collManagerPos + m_collisionRange);
		GetCollisionManager()->SetRootAABB(collRange);
	}
}