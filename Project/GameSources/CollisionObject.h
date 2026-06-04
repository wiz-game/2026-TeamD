#pragma once
#include "stdafx.h"

namespace basecross
{
	class CollisionObject : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
		bool m_isMeshDraw;
	public:
		CollisionObject
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam,
			const bool& isMeshDraw = false
		);
		virtual ~CollisionObject();

		virtual void OnCreate() override;
	};
}