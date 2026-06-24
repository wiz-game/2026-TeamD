/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	GameStage::GameStage(const wstring& stageNum)
		: Stage()
	{
		m_stageNum = to_string(stageNum);
	}

	void GameStage::OnCreate()
	{
		try 
		{
			m_jphManger.Initialize();

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

		int dirtNum = 0;
		auto gameObjectVec = GetGameObjectVec();
		for (auto& gameObject : gameObjectVec)
		{
			auto dirt = dynamic_pointer_cast<Dirt>(gameObject);
			if (dirt) dirtNum++;
		}

		if (dirtNum <= 0)
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

		MenuManager::Instance().ChangeUIDiffuse();
		MenuManager::Instance().SetUIDefaults(uidefaults);
		MenuManager::Instance().UIDrawActive(false, uidefaults);
	}

	void GameStage::SetCollRange()
	{
		// プレイヤーの位置を中心に当たり判定の範囲を設定する
		m_collManagerPos = GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		AABB collRange = AABB(m_collManagerPos - m_collisionRange, m_collManagerPos + m_collisionRange);
		GetCollisionManager()->SetRootAABB(collRange);
	}
}