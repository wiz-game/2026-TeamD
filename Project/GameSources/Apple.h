#pragma once
#include "stdafx.h"

namespace basecross
{
	class Apple : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Apple
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Apple();

		virtual void OnCreate() override;
	};
}