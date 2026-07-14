/*!
@file TitleStage.cpp
@brief タイトルステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TitleStage::OnCreate()
	{
		try
		{
			// カメラの設定
			auto camera = ObjectFactory::Create<Camera>();
			camera->SetEye(Vec3(0.0f, 0.0f, -8.0f));
			camera->SetAt(Vec3(0.0f, 1.0f, 0.0f));

			// ビューにカメラを設定
			auto view = CreateView<SingleView>();
			view->SetCamera(camera);

			// マルチライトの作成
			auto light = CreateLight<MultiLight>();
			light->SetDefaultLighting();
			
			AddGameObject<Ground>(STRUCT_ObjectParam(ENUM_ObjectID::Ground, Vec3(200.0f, 1.0f, 200.0f), Quat(), Vec3(0.0f, -1.0f, 0.0f)));
			AddGameObject<SkyBox>();
			//AddGameObject<Player>(Vec3(-1.2f, 0.25f, -5.0f));

			AddGameObject<UIBlinking>(STRUCT_UIParam(L"Awa", Vec3(300.0f, 200.0f, 0.0f), 0.3f, Col4(1.0f)), 0.1f);
			auto startButton = AddGameObject<UIBlinking>(STRUCT_UIParam(L"StartButton", Vec3(200.0f, -260.0f, 0.0f), 0.4f), 0.8f);
			SetSharedGameObject(L"StartButton", startButton);
			AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 600.0f);
		}
		catch (...) 
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
	}
}
