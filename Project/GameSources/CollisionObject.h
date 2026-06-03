#pragma once
#include "stdafx.h"

namespace basecross
{
	class CollisionObbObject : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		CollisionObbObject
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~CollisionObbObject();

		virtual void OnCreate() override;
	};

	class CollisionSphereObject : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		CollisionSphereObject
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~CollisionSphereObject();

		virtual void OnCreate() override;
	};

	class CollisionCapsuleObject : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
	public:
		CollisionCapsuleObject
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~CollisionCapsuleObject();

		virtual void OnCreate() override;
	};
}