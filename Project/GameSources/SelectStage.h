/*!
@file SelectStage.h
@brief セレクトステージ
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class SelectStage : public Stage
	{
	private:
	public:
		SelectStage() : Stage() {}
		virtual ~SelectStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
