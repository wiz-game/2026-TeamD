/*!
@file SoilWall.h
@brief 土の壁(Short)
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class SoilWallShort : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;

	public:
		SoilWallShort
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~SoilWallShort();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}