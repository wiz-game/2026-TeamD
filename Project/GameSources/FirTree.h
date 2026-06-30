#pragma once
#include "stdafx.h"

namespace basecross
{
	class FirTree : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		FirTree
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~FirTree();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}