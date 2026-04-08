#pragma once
#include "stdafx.h"

namespace basecross
{
	class TrampolineBubbles : public GameObject
	{
	private:
		shared_ptr<PNTStaticDraw> m_draw;
		shared_ptr<CollisionSphere> m_col;

		bool m_isTrampolineActive;
		int m_bubbleCount;
	
	public:
		TrampolineBubbles::TrampolineBubbles(const shared_ptr<Stage>& stage);
		TrampolineBubbles::~TrampolineBubbles();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);
	};
}