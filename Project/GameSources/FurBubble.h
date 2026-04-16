#pragma once
#include "stdafx.h"

namespace basecross
{
	class FurBubble : public Component
	{
	private:
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		shared_ptr<Stage> m_stage;
		vector<Vec3> *m_meshPosition;

	public:
		FurBubble(const shared_ptr<GameObject>& gameObject,const shared_ptr<Stage>& stage);
		virtual ~FurBubble();

		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnDraw();

		void CreateBubble();

		void GetStaticMeshWorldPositions(vector<Vec3> *vertices);
	};
}
