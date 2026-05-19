#pragma once
#include "stdafx.h"

namespace basecross
{
	class Stone : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Stone
		(
			const shared_ptr<Stage>& stage,
			const STRUCT_ObjectParam& objectParam
		);
		~Stone();

		virtual void OnCreate() override;
	};
}