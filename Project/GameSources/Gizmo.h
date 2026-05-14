/*!
@file Gizmo.h
@brief ギズモ
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_Axis
	{
		X,
		Y,
		Z,
		Max
	};

	class Gizmo : public GameObject
	{
	private:
		ENUM_Axis m_axis = ENUM_Axis::X;
		shared_ptr<Transform> m_sPtrTransform;
		shared_ptr<Transform> m_selectObjTrans;
		shared_ptr<PNTStaticDraw> m_sPtrDraw;

		Vec3 m_nowScale;
		float m_axisNum;
	public:
		Gizmo(const std::shared_ptr<Stage>& stage, const ENUM_Axis& axis, const shared_ptr<Transform>& selectObjTrans);

		void OnCreate() override;
		void OnUpdate() override;

		ENUM_Axis GetAxis() const { return m_axis; }
	};
}