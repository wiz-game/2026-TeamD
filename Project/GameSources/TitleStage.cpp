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
			
			AddGameObject<SkyBox>();
			AddGameObject<Ground>(STRUCT_ObjectParam(ENUM_ObjectID::Ground, Vec3(200.0f, 1.0f, 200.0f), Quat(), Vec3(0.0f, -1.0f, 0.0f)));
			AddGameObject<Awapaka>(Vec3(-0.72f, 0.0f, -5.8f), Vec3(0.0f, 2.4f, 0.0f), L"Idle");

			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_TitleLogo", Vec3(220.0f, 135.0f, 0.0f), 0.45f, Col4(1.0f)));
			auto startButton = AddGameObject<UIBlinking>(STRUCT_UIParam(L"UI_Start", Vec3(220.0f, -220.0f, 0.0f), 0.3f), 0.5f);
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
