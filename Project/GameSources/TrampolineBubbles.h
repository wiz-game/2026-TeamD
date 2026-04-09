#pragma once
#include "stdafx.h"

namespace basecross
{
	class TrampolineBubbles : public GameObject
	{
	private:
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_chargeDraw;
		shared_ptr<PCTStaticInstanceDraw> m_activeDraw;
		shared_ptr<CollisionSphere> m_col;

		bool m_isTrampolineActive;
		bool m_isInstanceCreated;
		int m_bubbleCount;
		Vec3 m_pos;

		void CreateActiveInstances();
	
	public:
		TrampolineBubbles::TrampolineBubbles(const shared_ptr<Stage>& stage,const Vec3& Position);
		TrampolineBubbles::~TrampolineBubbles();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);
	};
}