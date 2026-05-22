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
	public:
		GameClearStage() : Stage() {}
		virtual ~GameClearStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
