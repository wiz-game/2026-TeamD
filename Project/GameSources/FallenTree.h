#pragma once
#include "stdafx.h"

namespace basecross
{
	class FallenTree : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;

	public:
		FallenTree
		(
			const shared_ptr<Stage>& stage,
			const STRUCT_ObjectParam& objectParam
		);
		~FallenTree();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}