#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameManager::EnterGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{	
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			SetAllGameObjectsUpdateActive(false);
			StageEditor::Instance().StartEditor();
			InputManager::Instance().ResetWheelDelta();
			break;
		default:
			break;
		}
	}

	void GameManager::ExitGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			SetAllGameObjectsUpdateActive(true);
			StageEditor::Instance().EndEditor();
			InputManager::Instance().ResetWheelDelta();
			break;
		default:
			break;
		}
	}
	
	void GameManager::Initialize()
	{
	}

	void GameManager::AddDebugStr(const wstring& debugStr)
	{
		if (m_sPtrDebugLog)
		{
			m_sPtrDebugLog->AddDebugStr(debugStr);
		}
		else 
		{
			m_sPtrDebugLog = App::GetApp()->GetScene<Scene>()->GetActiveStage()->AddGameObject<DebugLog>();
			AddDebugStr(debugStr);
		}
	}

	void GameManager::RemoveDebugLog()
	{
		if (m_sPtrDebugLog) 
		{
			m_sPtrDebugLog->DestroyGameObject();
			m_sPtrDebugLog = nullptr;
		}
	}

	void GameManager::SetGameMode(ENUM_GameMode gameMode)
	{
		ExitGameMode(m_gameMode);
		m_gameMode = gameMode;
		EnterGameMode(m_gameMode);
	}
}