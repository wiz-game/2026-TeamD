#pragma once
#include "stdafx.h"

namespace basecross
{
	class LongMushroom : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		LongMushroom
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~LongMushroom();

		virtual void OnCreate() override;
	};
}
