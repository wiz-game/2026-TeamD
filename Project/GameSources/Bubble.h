#pragma once
#include "stdafx.h"
namespace basecross
{
	class Bubble : public GameObject
	{
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		weak_ptr<GameObject> m_parent;
		Vec3 m_parentForward;

	public :
		Bubble::Bubble(const shared_ptr<Stage>& stage,const shared_ptr<GameObject>& parent);
		Bubble::~Bubble();

		void OnCreate() override;
		void OnUpdate() override;
	};
}
