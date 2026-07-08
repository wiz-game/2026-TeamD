/*!
@file SoilWall.h
@brief 土の壁(Long)
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class SoilWallLong : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;

	public:
		SoilWallLong
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~SoilWallLong();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}