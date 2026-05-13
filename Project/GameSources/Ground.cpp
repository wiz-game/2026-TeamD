/*!
@file Ground.cpp
@brief 地面
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Ground::Ground
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{}

	Ground::~Ground()
	{
	}

	void Ground::OnCreate()
	{
		AddTag(L"Ground");
		SetIsEditorSave(true);
		SetAlphaActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		auto ptrShadow = AddComponent<Shadowmap>();
		m_sPtrTrans = AddComponent<Transform>();

		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"Ground_Sozai");

		ptrDraw->SetOwnShadowActive(true);
		
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");

		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());
		
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetDrawActive(true);
	}
}
