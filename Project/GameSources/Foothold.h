#pragma once
#include "stdafx.h"

namespace basecross
{
	class Foothold : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Foothold
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Foothold();

		virtual void OnCreate() override;
	};
}

