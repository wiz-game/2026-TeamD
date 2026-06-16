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
		auto ui = stage->AddGameObject<UIBase>(STRUCT_UIParam(L"MenuBack", Vec3(0.0f, 0.0f, 0.0f), 1.0f, Col4(1.0f, 1.0f, 1.0f, 0.25f)));
		SetAllUpdateActive(false);

	}

	void MenuManager::ClosePause()
	{
		SetAllUpdateActive(true);
	}

	//ステージの全オブジェクトのUpdateを管理する
	void MenuManager::SetAllUpdateActive(const bool& isUpdateActive)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjects = stage->GetGameObjectVec();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetUpdateActive(isUpdateActive);
		}
	}

	void MenuManager::UIDrawActive(bool isActive)
	{
		for (auto& uipointer : m_uipointers)
		{
			uipointer->GetComponent<PCTSpriteDraw>()->SetDrawActive(isActive);
		}
	}

	void MenuManager::SetUIPointers(const vector<shared_ptr<UIBase>>& uipointers)
	{
		m_uipointers = uipointers;
	}
}