#pragma once
#include "stdafx.h"

namespace basecross
{
	class TrampolineBubbles : public GameObject
	{
	private:
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_chargeDraw;
		shared_ptr<PNTStaticInstanceDraw> m_activeDraw;
		shared_ptr<CollisionSphere> m_col;

		bool m_isTrampolineActive;
		bool m_isInstanceCreated;
		int m_bubbleCount;
		Vec3 m_pos;
		Vec3 m_scale;
		Vec3 m_modelScale;
		bool m_isCountedThisFrame;

		void CreateActiveInstances();
	
	public:
		TrampolineBubbles::TrampolineBubbles(const shared_ptr<Stage>& stage,const Vec3& Position);
		TrampolineBubbles::~TrampolineBubbles();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);

		void AddbubbleCount()
		{
			m_bubbleCount++;
		}

		int GetbubbleCount()
		{
			return m_bubbleCount;
		}
	};
}