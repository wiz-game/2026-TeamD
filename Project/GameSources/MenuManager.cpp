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
		auto gameObjects = stage->GetGameObjectVec();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetUpdateActive(false);
		}

	}

}