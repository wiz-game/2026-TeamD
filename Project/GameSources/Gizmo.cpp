#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Gizmo::Gizmo(const std::shared_ptr<Stage>& stage, const ENUM_Axis& axis, const shared_ptr<Transform>& selectObjTrans)
		: GameObject(stage),
		m_axis(axis),
		m_selectObjTrans(selectObjTrans)
	{
	}

	void Gizmo::OnCreate()
	{
		m_sPtrTransform = AddComponent<Transform>();
		m_sPtrTransform->SetPosition(m_selectObjTrans->GetPosition());

		m_sPtrDraw = AddComponent<PCTStaticDraw>();
		m_sPtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		SetDrawActive(true);
	}

	void Gizmo::OnUpdate()
	{
	}
}