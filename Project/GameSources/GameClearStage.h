/*!
@file GameClearStage.h
@brief ゲームクリアステージ
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class GameClearStage : public Stage
	{
	private:
		vector<shared_ptr<UIBase>> m_uiclears;

	public:
		GameClearStage() : Stage() {}
		virtual ~GameClearStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
