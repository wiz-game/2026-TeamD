#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Gizmo::Gizmo(const std::shared_ptr<Stage>& stage, const ENUM_Axis& axis, const shared_ptr<Transform>& selectObjTrans)
		: GameObject(stage),
		m_axis(axis),
		m_selectObjTrans(selectObjTrans)
	{
		m_axisNum = 5.0f;
	}

	void Gizmo::OnCreate()
	{
		m_sPtrTransform = AddComponent<Transform>();
		m_sPtrTransform->SetPosition(m_selectObjTrans->GetPosition());

		m_sPtrDraw = AddComponent<PNTStaticDraw>();
		m_sPtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_sPtrDraw->SetDiffuse(Vec4(1.0f, 1.0f, 1.0f, 0.5f));
		SetDrawActive(true);

		Mat4x4 spanMat;
		switch (m_axis)
		{
		case ENUM_Axis::X:
			m_sPtrDraw->SetTextureResource(L"XArrow");
			m_sPtrTransform->SetScale(Vec3(m_axisNum, 0.5f, 0.5f));
			spanMat.affineTransformation
			(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.5f, 0.0f, 0.0f)
			);
			break;
		case ENUM_Axis::Y:
			m_sPtrDraw->SetTextureResource(L"YArrow");
			m_sPtrTransform->SetScale(Vec3(0.5f, m_axisNum, 0.5f));
			spanMat.affineTransformation
			(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.5f, 0.0f)
			);
			break;
		case ENUM_Axis::Z:
			m_sPtrDraw->SetTextureResource(L"ZArrow");
			m_sPtrTransform->SetScale(Vec3(0.5f, 0.5f, m_axisNum));
			spanMat.affineTransformation
			(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.5f)
			);
			break;
		default:
			break;
		}
		m_sPtrDraw->SetMeshToTransformMatrix(spanMat);

		SetDrawLayer(100);
		m_sPtrDraw->SetDepthStencilState(DepthStencilState::None);
	}

	void Gizmo::OnUpdate()
	{
	}
}