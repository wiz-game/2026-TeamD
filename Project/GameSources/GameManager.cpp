#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameManager::ExitGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
				gameObject->SetUpdateActive(true);
			break;
		default:
			break;
		}
	}

	void GameManager::EnterGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{	
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec()) 
				gameObject->SetUpdateActive(false);
			break;
		default:
			break;
		}
	}

	void GameManager::Initialize()
	{
	}

	void GameManager::SetGameMode(ENUM_GameMode gameMode)
	{
		ExitGameMode(m_gameMode);
		m_gameMode = gameMode;
		EnterGameMode(m_gameMode);
	}
}