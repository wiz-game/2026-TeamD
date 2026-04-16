#pragma once
#include "stdafx.h"

namespace basecross
{
	class Mushroom : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
		Vec3 m_scale;
		Quat m_quaternion;
		Vec3 m_position;

	public:
		Mushroom
		(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Quaternion,
			const Vec3& Position
		);
		virtual ~Mushroom();

		virtual void OnCreate() override;

	};
}
