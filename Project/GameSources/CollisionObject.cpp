#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	CollisionObbObject::CollisionObbObject
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	};
	
	CollisionObbObject::~CollisionObbObject()
	{
	}

	void CollisionObbObject::OnCreate()
	{
		AddTag(L"CollisionObbObject");
		SetIsEditorSave(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(false);
		col->SetFixed(true);
	}

	CollisionSphereObject::CollisionSphereObject
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	};

	CollisionSphereObject::~CollisionSphereObject()
	{
	}

	void CollisionSphereObject::OnCreate()
	{
		AddTag(L"CollisionSphereObject");
		SetIsEditorSave(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto col = AddComponent<CollisionSphere>();
		col->SetDrawActive(false);
		col->SetFixed(true);
	}

	CollisionCapsuleObject::CollisionCapsuleObject
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	};

	CollisionCapsuleObject::~CollisionCapsuleObject()
	{
	}

	void CollisionCapsuleObject::OnCreate()
	{
		AddTag(L"CollisionCapsuleObject");
		SetIsEditorSave(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto col = AddComponent<CollisionCapsule>();
		col->SetDrawActive(false);
		col->SetFixed(true);
	}
}