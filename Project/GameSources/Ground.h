/*!
@file Ground.h
@brief 地面
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Ground : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		// コンストラクタ
		Ground
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~Ground();

		virtual void OnCreate() override;
	};
}