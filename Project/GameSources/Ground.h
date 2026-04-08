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

		Vec3 m_scale;
		Quat m_quaternion;
		Vec3 m_position;
	public:
		// コンストラクタ
		Ground
		(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Quaternion,
			const Vec3& Position
		);
		virtual ~Ground();

		virtual void OnCreate() override;
	};
}