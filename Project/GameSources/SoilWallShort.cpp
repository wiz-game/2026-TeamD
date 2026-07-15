/*!
@file SoilWall.cpp
@brief 土の壁(Short)
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SoilWallShort::SoilWallShort
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	}

	SoilWallShort::~SoilWallShort()
	{

	}

	void SoilWallShort::OnCreate()
	{
		AddTag(L"SoilWallShort");
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
		ptrDraw->SetMeshResource(L"M_SoilWall_1");
		ptrDraw->SetTextureResource(L"T_SoilWall");
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"M_SoilWall_1");

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.125f, 0.25f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}

	void SoilWallShort::OnUpdate()
	{
		m_stageObjectFade.UpdateFade(GetComponent<PNTStaticDraw>(), App::GetApp()->GetElapsedTime());
	}
}