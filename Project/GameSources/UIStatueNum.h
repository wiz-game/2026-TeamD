/*!
@file SoilWall.h
@brief 綺麗にしたあわパカ像
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UIStatueNum : public UIBase
	{
	private:
		int m_statuenum = 0;

	public:
		UIStatueNum(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam);
		virtual ~UIStatueNum() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void SetStatueNum(int num) { m_statuenum = num; }
	};

}