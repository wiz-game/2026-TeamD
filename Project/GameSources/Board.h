#pragma once
#include "stdafx.h"

namespace basecross
{
	class Board : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Board
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Board();

		virtual void OnCreate() override;
	};
}
