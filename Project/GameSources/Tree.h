#pragma once
#include "stdafx.h"

namespace basecross
{
	class Tree : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
		Vec3 m_scale;
		Quat m_quaternion;
		Vec3 m_position;

	public:
		Tree
		(	
			const shared_ptr<Stage>& StagePtr,
			const int& Id,
			const Vec3& Scale,
			const Quat& Quaternion,
			const Vec3& Position
		);
		virtual ~Tree();
		
		virtual void OnCreate() override;

	};
}