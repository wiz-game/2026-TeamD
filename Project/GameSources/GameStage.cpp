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
		m_timer(0.0f),
		m_isGameClear(false),
		m_isStartStop(false),
		m_isGameStageMovie(true)
	{
		m_stageNum = to_string(stageNum);
	}

	void GameStage::OnCreate()
	{
		try 
		{
			m_jphManger.Initialize();
			m_timer = Timer(2.0f);

			CreateViewLight();
			CreatePlayer();
			CreateMenu();
			AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 600.0f);

			// ステージの作成
			StageEditor::Instance().ReadStageData(m_stageNum + ".bin", GetThis<GameStage>());
			
			AddGameObject<EffectUpdateDrawManager>();
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
		
		auto player = GetSharedGameObject<Player>(L"Player");

		if (m_isGameStageMovie && m_timer.TimeCount(App::GetApp()->GetElapsedTime(), false))
		{
			MovieManager::Instance().Initialize();
			MovieManager::Instance().PlayMovie(MovieType::Title);
			m_isGameStageMovie = false;
			m_isStartStop = true;
			m_timer.SetCounter();
		}

		if (m_isStartStop)
		{
			if (m_timer.TimeCount(App::GetApp()->GetElapsedTime(), false))
			{
				player->SetMoveStopFlag(false);
				player->PlayGameAnimation();
				m_isStartStop = false;
				m_timer.SetCounter();
			}
		}

		if (GameManager::Instance().GetDirtNum() <= 0)
		{	
			if (!m_isGameClear)
			{
				MovieManager::Instance().Initialize();
				MovieManager::Instance().PlayMovie(MovieType::GameClear);
				player->SetMoveStopFlag(true);
				player->PlayerChangeAnimation(L"GameClear",false);
				m_timer.SetCounter();
				m_isGameClear = true;
			}
		}

		if (m_isGameClear && m_timer.TimeCount(App::GetApp()->GetElapsedTime(), false))
		{
			GameManager::Instance().SetGameMode(ENUM_GameMode::GameClear);
		}

		// メニュー画面のボタンの切り替え
		MenuManager::Instance().ChangeUIDiffuse();
	}

	void GameStage::OnUpdate2()
	{
		m_jphManger.Update(1.0f / 60.0f);
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
	}

	// Playerを作成する
	void GameStage::CreatePlayer()
	{
		auto player = AddGameObject<Player>(Vec3(0.0f, 0.75f, 0.0f));
		SetSharedGameObject(L"Player", player);
		MovieManager::Instance().SetPlayer(player);
	}

	void GameStage::CreateMenu()
	{
		Col4 blackOutColor = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		vector<shared_ptr<UIBase>> uiframes;
		vector<shared_ptr<UIBase>> uidefaults;
		vector<shared_ptr<UIBase>> uisettings;
		vector<shared_ptr<UIBase>> uihowtoplays;

		uiframes.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionFade",
			Vec3(0.0f, 0.0f, 0.0f), 
			1.0f,
			Col4(1.0f))));
		uiframes.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionFrame",
			Vec3(0.0f, 0.0f, 0.0f), 
			0.75f,
			Col4(1.0f))));

		MenuManager::Instance().SetUIFrames(uiframes);
		MenuManager::Instance().UIDrawActive(false, uiframes);


		uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_1",
			Vec3(0.0f, 150.0f, 0.0f),
			0.25f,
			blackOutColor)));
		uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_2",
			Vec3(0.0f, 25.0f, 0.0f),
			0.25f,
			blackOutColor)));
		uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_3",
			Vec3(0.0f, -100.0f, 0.0f),
			0.25f,
			blackOutColor)));
		uidefaults.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionButton_4",
			Vec3(0.0f, -225.0f, 0.0f),
			0.25f,
			blackOutColor)));

		MenuManager::Instance().SetUIDefaults(uidefaults);
		MenuManager::Instance().UIDrawActive(false, uidefaults);


		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionSound",
			Vec3(0.0f, 150.0f, 0.0f),
			0.1f,
			Col4(1.0f))));
		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionBGM",
			Vec3(0.0f, 25.0f, 0.0f),
			0.1f,
			Col4(1.0f))));
		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionSE",
			Vec3(0.0f, -100.0f, 0.0f),
			0.1f,
			Col4(1.0f))));
		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionBaymax",
			Vec3(0.0f, -40.0f, 0.0f),
			0.25f,
			Col4(1.0f))));
		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionBaymax",
			Vec3(0.0f, -165.0f, 0.0f),
			0.25f,
			Col4(1.0f))));
		uisettings.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionReturn",
			Vec3(0.0f, -220.0f, 0.0f),
			0.1f,
			Col4(1.0f))));

		MenuManager::Instance().SetUISettings(uisettings);
		MenuManager::Instance().UIDrawActive(false, uisettings);


		uihowtoplays.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
			L"UI_OptionHowtoplay",
			Vec3(0.0f, 0.0f, 0.0f),
			0.50f,
			Col4(1.0f))));

		MenuManager::Instance().SetUIHowtoplays(uihowtoplays);
		MenuManager::Instance().UIDrawActive(false, uihowtoplays);





		MenuManager::Instance().ChangeUIDiffuse();
	}

	void GameStage::SetCollRange()
	{
		// プレイヤーの位置を中心に当たり判定の範囲を設定する
		m_collManagerPos = GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		AABB collRange = AABB(m_collManagerPos - m_collisionRange, m_collManagerPos + m_collisionRange);
		GetCollisionManager()->SetRootAABB(collRange);
	}
}