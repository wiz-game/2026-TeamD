#pragma once
#include "stdafx.h"

namespace basecross
{
	class Fence : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Fence
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Fence();

		virtual void OnCreate() override;
	};
}