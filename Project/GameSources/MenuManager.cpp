#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MenuManager& MenuManager::Instance()
	{
		static MenuManager instance;
		return instance;
	}

	void MenuManager::Pause()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		stage->AddGameObject<UIBase>(STRUCT_UIParam(L"MenuBack", Vec3(0.0f, 0.0f, 0.0f), 1.0f, Col4(1.0f, 1.0f, 1.0f, 0.25f)));

		SetAllUpdateActive(false);
	}

	void MenuManager::ClosePause()
	{
		SetAllUpdateActive(true);
	}

	void MenuManager::SetAllUpdateActive(const bool& isUpdateActive)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjects = stage->GetGameObjectVec();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetUpdateActive(isUpdateActive);
		}

	}

}