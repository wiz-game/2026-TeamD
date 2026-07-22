/*!
@file GameClearStage.cpp
@brief ゲームクリアステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void GameClearStage::OnCreate()
	{
		try
		{
			// カメラの設定
			auto camera = ObjectFactory::Create<Camera>();
			camera->SetEye(Vec3(0.0f, 0.25f, -8.5f));
			camera->SetAt(Vec3(0.0f, 0.8f, 0.0f));

			// ビューにカメラを設定
			auto view = CreateView<SingleView>();
			view->SetCamera(camera);

			// マルチライトの作成
			auto light = CreateLight<MultiLight>();
			light->SetDefaultLighting();

			AddGameObject<SkyBox>();
			AddGameObject<Ground>(STRUCT_ObjectParam(ENUM_ObjectID::Ground, Vec3(500.0f, 1.0f, 500.0f), Quat(), Vec3(0.0f, -1.0f, 0.0f)));
			AddGameObject<Awapaka>(Vec3(-0.72f, 0.0f, -5.8f), Vec3(0.0f, 2.4f, 0.0f), L"GameClear");

			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_GameClear", Vec3(0.0f, 160.0f, 0.0f), 0.7f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_OptionButton_5", Vec3(250.0f, -150.0f, 0.0f), 0.25f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_OptionButton_4", Vec3(250.0f, -300.0f, 0.0f), 0.25f));
		}
		catch (...)
		{
			throw;
		}
	}

	void GameClearStage::OnUpdate()
	{
	}
}
