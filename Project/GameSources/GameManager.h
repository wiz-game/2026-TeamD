/*!
@file GameManager.h
@brief ゲームマネージャー 
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	enum class ENUM_GameMode
	{
		Play,
		Menu,
		Editor
	};

	class GameManager
	{
	private:
		ENUM_GameMode m_gameMode = ENUM_GameMode::Play;
	private:
		GameManager() {}
		virtual ~GameManager() {}
		
		void ExitGameMOde(ENUM_GameMode gameMode);
	public:
		static GameManager& Instance()
		{
			static GameManager instance;
			return instance;
		}

		void Initialize();

		// アクセサー
		ENUM_GameMode GetGameMode() const { return m_gameMode; }
		void SetGameMode(ENUM_GameMode gameMode);
	};

}
