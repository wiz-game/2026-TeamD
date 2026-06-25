/*!
@file SoilWall.h
@brief 土の壁
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class SoilWall : public GameObject
	{
	private:
		shared_ptr<Transform> m_sPtrTrans;
		vector<VertexPositionNormalTexture> m_vertices;

		void SetVertices();
	public:
		SoilWall
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		virtual ~SoilWall();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void UpdateTexture();
	};
}