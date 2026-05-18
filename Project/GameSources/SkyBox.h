/*!
@file SkyBox.h
@brief スカイボックス
@copyright 吉田 智貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class SkyBox : public GameObject
	{
	public:
		SkyBox(const shared_ptr<Stage>& stage);
		virtual ~SkyBox();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}