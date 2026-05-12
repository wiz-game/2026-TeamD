#pragma once
#include "stdafx.h"

namespace basecross
{
	class Mushroom : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Mushroom
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Mushroom();

		virtual void OnCreate() override;

	};
}
