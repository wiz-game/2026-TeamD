#pragma once
#include "stdafx.h"

namespace basecross
{
	class Slope : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Slope
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Slope();

		virtual void OnCreate() override;
	};
}
