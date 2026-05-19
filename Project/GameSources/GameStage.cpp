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

			AddGameObject<SkyBox>();
			CreateViewLight();
			CreatePlayer();
			AddGameObject<EnemyAlpaca>(STRUCT_ObjectParam(ENUM_ObjectID::Mushroom, Vec3(0.3f), Quat(), Vec3(1.0f, 1.0f, 1.0f)));
			AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 600.0f);
			
			// ステージの作成
			StageEditor::Instance().ReadStageData(m_stageNum + ".bin", GetThis<GameStage>());
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
		auto player = AddGameObject<Player>(Vec3(0.0f, 0.0f, 0.0f));
		SetSharedGameObject(L"Player", player);
	}

	void GameStage::SetCollRange()
	{
		// プレイヤーの位置を中心に当たり判定の範囲を設定する
		m_collManagerPos = GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		AABB collRange = AABB(m_collManagerPos - m_collisionRange, m_collManagerPos + m_collisionRange);
		GetCollisionManager()->SetRootAABB(collRange);
	}
}