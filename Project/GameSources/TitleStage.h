/*!
@file TitleStage.h
@brief タイトルステージ
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class TitleStage : public Stage
	{
	private:
	public:
		TitleStage() : Stage() {}
		virtual ~TitleStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
