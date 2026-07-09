/*!
@file SoilWall.cpp
@brief 土の壁(Long)
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SoilWallLong::SoilWallLong
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	}

	SoilWallLong::~SoilWallLong()
	{

	}

	void SoilWallLong::OnCreate()
	{
		AddTag(L"SoilWallLong");
		AddTag(L"Fade");
		AddTag(L"Wall");
		SetIsEditorSave(true);
		SetAlphaActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetMeshResource(L"M_SoilWall_2");
		ptrDraw->SetTextureResource(L"T_SoilWall");
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"M_SoilWall_2");

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.063f, 0.27f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.56f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}

	void SoilWallLong::OnUpdate()
	{
		m_stageObjectFade.UpdateFade(GetComponent<PNTStaticDraw>(), App::GetApp()->GetElapsedTime());
	}
}