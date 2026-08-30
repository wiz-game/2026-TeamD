/*!
@file GameOverStage.cpp
@brief ゲームオーバーステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void GameOverStage::OnCreate()
	{
		try
		{
			// カメラの設定
			auto camera = ObjectFactory::Create<Camera>();
			camera->SetEye(Vec3(0.0f, 0.0f, -8.0f));
			camera->SetAt(Vec3(0.0f, 0.7f, 0.0f));

			// ビューにカメラを設定
			auto view = CreateView<SingleView>();
			view->SetCamera(camera);

			// マルチライトの作成
			auto light = CreateLight<MultiLight>();
			light->SetDefaultLighting();

			AddGameObject<SkyBox>();
			AddGameObject<Ground>(STRUCT_ObjectParam(ENUM_ObjectID::Ground, Vec3(200.0f, 1.0f, 200.0f), Quat(), Vec3(0.0f, -1.0f, 0.0f)));
			AddGameObject<Awapaka>(Vec3(-0.72f, 0.0f, -5.8f), Vec3(0.0f, 2.4f, 0.0f), L"GameOver");

			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_GameOver", Vec3(0.0f, 150.0f, 0.0f), 0.7f));

			m_uigameovers.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
				L"UI_OptionButton_5",
				Vec3(250.0f, -165.0f, 0.0f),
				0.25f,
				Col4(1.0f))));

			m_uigameovers.push_back(AddGameObject<UIBase>(STRUCT_UIParam(
				L"UI_OptionButton_4",
				Vec3(250.0f, -305.0f, 0.0f),
				0.25f,
				Col4(1.0f))));

			MenuManager::Instance().SetUIGameOvers(m_uigameovers);


		}
		catch (...)
		{
			throw;
		}
	}

	void GameOverStage::OnUpdate()
	{
		MenuManager::Instance().ChangeUIParam();
	}
}
