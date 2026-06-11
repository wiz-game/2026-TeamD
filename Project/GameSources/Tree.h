#pragma once
#include "stdafx.h"

namespace basecross
{
	class Tree : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		Tree
		(	
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Tree();
		
		virtual void OnCreate() override;
		virtual void OnUpdate() {};
	};
}