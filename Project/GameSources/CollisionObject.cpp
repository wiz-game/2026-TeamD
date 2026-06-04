#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	CollisionObject::CollisionObject
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam,
		const bool& isMeshDraw
	) :
		GameObject(StagePtr, objectParam),
		m_isMeshDraw(isMeshDraw)
	{
	};
	
	CollisionObject::~CollisionObject()
	{
	}

	void CollisionObject::OnCreate()
	{
		AddTag(L"CollisionObject");
		SetIsEditorSave(true);
		SetAlphaActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(false);
		col->SetFixed(true);

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDrawActive(m_isMeshDraw);
		draw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.25f));
	}
}