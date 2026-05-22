/*!
@file GameOverStage.h
@brief ゲームオーバーステージ
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class GameOverStage : public Stage
	{
	private:
	public:
		GameOverStage() : Stage() {}
		virtual ~GameOverStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
