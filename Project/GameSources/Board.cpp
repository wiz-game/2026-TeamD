#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Board::Board
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	};

	Board::~Board()
	{
	}

	void Board::OnCreate()
	{
		AddTag(L"Board");
		SetIsEditorSave(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"M_Board");
		ptrDraw->SetTextureResource(L"T_Board");

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrDraw->SetOwnShadowActive(true);
		ptrShadow->SetMeshResource(L"M_Board");

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);
	}
}